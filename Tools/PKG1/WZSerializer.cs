using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using SharpCompress.Compressors;
using SharpCompress.Compressors.Deflate;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using GZipStream = System.IO.Compression.GZipStream;

namespace PKG1;

public class WzSerializer
{
    public ushort _version;
    private readonly Queue<Tuple<long, WzProperty>> _pending = new();
    private readonly ConcurrentDictionary<WzProperty, uint> _imgLocations = new();
    private readonly uint _versionKey;
    private readonly byte _versionHash;

    public WzSerializer(ushort version, bool normalWz = false)
    {
        _version = version;
        NormalWz = normalWz;
        _versionHash = CalcVersionHash(version, out _versionKey);
    }

    public bool NormalWz { get; }

    public static byte[] DeflateZLib(byte[] data)
    {
        using (var str = new MemoryStream(data))
        using (var result = new MemoryStream())
        using (var inflate = new ZlibStream(str, CompressionMode.Compress, CompressionLevel.BestCompression))
        {
            inflate.CopyTo(result);
            result.Position = 0;
            return result.ToArray();
        }
    }

    public static byte[] Deflate(byte[] data)
    {
        using (var result = new MemoryStream())
        {
            using (var deflate = new GZipStream(result, System.IO.Compression.CompressionLevel.Optimal, true))
                deflate.Write(data, 0, data.Length);

            result.Position = 0;
            return result.ToArray();
        }
    }

    internal static byte CalcVersionHash(ushort version, out uint versionKey)
    {
        var key = version.ToString().Select(c => (uint) c).Aggregate((uint) 0, (result, next) =>
        {
            result <<= 5;
            result += next + 1;
            return result;
        });
        versionKey = key;
        return BitConverter.GetBytes(key).Aggregate((byte) 0xFF, (result, next) => (byte) (result ^ next));
    }

    private static uint Rotl(uint value, byte shiftTimes)
    {
        return (value << shiftTimes) | (value >> (32 - shiftTimes));
    }

    public void Image(BinaryWriter writer, WzProperty self)
    {
        //if (!imgLocations.ContainsKey(self))
        //    imgLocations.Add(self, (uint)writer.BaseStream.Position);
        //else
        //    imgLocations[self] = (uint)writer.BaseStream.Position; // Pretty sure this should never happen

        //using (WZReader reader = self.FileContainer.GetContentReader(null, self))
        //{
        //    reader.BaseStream.Seek(self.Offset, SeekOrigin.Begin);
        //    writer.Write(reader.ReadBytes((int)self.Size));
        //}
        writer.Write((byte) 0x73); // Regular img
        GetWzStringBytes(writer, "Property");
        writer.Write((short) 0);
        if (self._nameWithoutExtension != "zmap" && self._nameWithoutExtension == "smap")
            PropertyList(writer, self.Children.OrderBy(c => c._name));
        else
            PropertyList(writer, self.Children);
    }

    public void Lua(BinaryWriter writer, WzProperty self)
    {
        writer.Write((byte) 0x01);
        var s = ((IWzPropertyVal<string>) self).Value;
        var isAscii = s.All(c => c <= byte.MaxValue);
        var length = s.Length * (isAscii ? -1 : 1);

        IEnumerable<byte> lengthBytes;
        if (length >= 127)
            lengthBytes = new byte[] {127}.Concat(BitConverter.GetBytes(length));
        else if (length <= -128)
            lengthBytes = new byte[] {0x80}.Concat(BitConverter.GetBytes(length * -1));
        else
            lengthBytes = new[] {(byte) length};
        writer.Write(lengthBytes.ToArray());

        if (isAscii) writer.Write(Encoding.ASCII.GetBytes(s).Select((c, i) => (byte) (c ^ WzReader._kmsKey[i])).ToArray());
        else writer.Write(Encoding.Unicode.GetBytes(s).Select((c, i) => (byte) (c ^ WzReader._kmsKey[i])).ToArray());
    }

    public void PropertyList(BinaryWriter writer, IEnumerable<WzProperty> childEnum, bool orderByName = true)
    {
        var childrenFiltered = childEnum.Where(c => c != null);

        if (orderByName)
            childrenFiltered = childrenFiltered.OrderBy(c => c._name);

        var children = childrenFiltered.ToArray();

        WzIntToByte(writer, children.Length);

        foreach (var c in children)
        {
            GetStringToBlock(writer, c._name);

            if (c is WzPropertyVal<sbyte>)
                writer.Write((byte) 0x10);
            else if (c is WzPropertyVal<byte>)
                writer.Write((byte) 0x11);
            else if (c is WzPropertyVal<ushort>)
            {
                writer.Write((byte) 0x12);
                writer.Write(BitConverter.GetBytes(c.ResolveFor<ushort>().Result.Value));
            }
            else if (c is WzPropertyVal<int>)
            {
                writer.Write((byte) 3);
                WzIntToByte(writer, c.ResolveFor<int>().Result.Value);
            }
            else if (c is WzPropertyVal<Rgba32>)
            {
                writer.Write((byte) 19);
                WzIntToByte(writer, (int) c.ResolveFor<Rgba32>().Result.Value.Rgba);
            }
            else if (c is WzPropertyVal<float>)
            {
                writer.Write((byte) 4);
                WzSingleToByte(writer, c.ResolveFor<float>().Result.Value);
            }
            else if (c is WzPropertyVal<double>)
            {
                writer.Write((byte) 5);
                writer.Write(BitConverter.GetBytes(c.ResolveFor<double>().Result.Value));
            }
            else if (c is WzPropertyVal<string> && c.Type != PropertyType.Lua && c.Type != PropertyType.Uol)
            {
                writer.Write((byte) 8);
                GetStringToBlock(writer, c.ResolveForOrNull<string>()?.Result ?? "");
            }
            else if (c is WzPropertyVal<long>)
            {
                writer.Write((byte) 20);
                WzLongToByte(writer, c.ResolveFor<long>().Result.Value);
            }
            else if (c is WzPropertyVal<ulong>)
            {
                writer.Write((byte) 21);
                WzLongToByte(writer, (long) c.ResolveFor<ulong>().Result.Value);
            }
            else if (c.Type == PropertyType.Null)
                writer.Write((byte) 0);
            else
            {
                writer.Write((byte) 9);
                writer.Write(0); // BlockLen
                var position = writer.BaseStream.Position;
                Resolve(writer, c);
                var length = (uint) (writer.BaseStream.Position - position);
                var newPosition = writer.BaseStream.Position;
                writer.BaseStream.Position = position - 4;
                writer.Write(length);
                writer.BaseStream.Position = newPosition;
            }
        }
    }

    public void DirectoryChildren(BinaryWriter writer, WzProperty self)
    {
        var children = self.GetChildren().OrderBy(c => c._name).ToArray();
        WzIntToByte(writer, children.Length);

        foreach (var child in children)
        {
            if (child.Type == PropertyType.Directory)
                writer.Write((byte) 3);
            else if (child.Type == PropertyType.Image || child.Type == PropertyType.Lua)
                writer.Write((byte) 4);
            else throw new InvalidOperationException("Only directories, img, and lua can be directly below a directory object");
            GetWzStringBytes(writer, child._name);
            var position = writer.BaseStream.Position;
            _pending.Enqueue(new Tuple<long, WzProperty>(position, child));
            WzIntToByte(writer, 123456789);
            WzIntToByte(writer, 123456789);
            writer.Write((uint) 0); // Offset
        }
    }

    public void Audio(BinaryWriter writer, WzProperty self)
    {
        var unk = (byte) (self.HasDefinedMeta && self.Meta.ContainsKey("unk") ? self.Meta["unk"] : 0);
        var duration = (int) (self.HasDefinedMeta && self.Meta.ContainsKey("duration") ? self.Meta["duration"] : 0);
        var length = (int) self._size;

        writer.Write(unk);
        WzIntToByte(writer, length);
        WzIntToByte(writer, duration);
        writer.Write(self.ResolveForOrNull<byte[]>().Result);
    }

    public void ConvexChildren(BinaryWriter writer, WzProperty self)
    {
        var children = self.GetChildren().OrderBy(c => c._name).ToArray();
        WzIntToByte(writer, children.Length);

        foreach (var child in children) Resolve(writer, child);
    }

    public void Vector(BinaryWriter writer, WzProperty self)
    {
        var pos = self.ResolveFor<Point>().Result ?? Point.Empty;
        WzIntToByte(writer, pos.X);
        WzIntToByte(writer, pos.Y);
    }

    public void Uol(BinaryWriter writer, WzPropertyVal<string> self)
    {
        GetStringToBlock(writer, self.Value);
    }

    public void Resolve(BinaryWriter writer, WzProperty parent)
    {
        switch (parent.Type)
        {
            case PropertyType.Directory:
                DirectoryChildren(writer, parent);
                break;
            case PropertyType.Image:
                Image(writer, parent);
                break;
            case PropertyType.Lua:
                Lua(writer, parent);
                break;
            case PropertyType.SubProperty:
                GetStringToBlock(writer, "Property");
                SubProperty(writer, parent);
                break;
            case PropertyType.Canvas:
                GetStringToBlock(writer, "Canvas");
                Canvas(writer, parent);
                break;
            case PropertyType.Vector2:
                GetStringToBlock(writer, "Shape2D#Vector2D");
                Vector(writer, parent);
                break;
            case PropertyType.Convex:
                GetStringToBlock(writer, "Shape2D#Convex2D");
                ConvexChildren(writer, parent);
                break;
            case PropertyType.Audio:
                GetStringToBlock(writer, "Sound_DX8");
                Audio(writer, parent);
                break;
            case PropertyType.Uol:
                GetStringToBlock(writer, "UOL");
                writer.Write((byte) 0);
                Uol(writer, (WzPropertyVal<string>) parent);
                break;

            case PropertyType.File:
                GetStringToBlock(writer, "RawData");
                WzFile(writer, (WzPropertyVal<byte[]>) parent);
                break;
        }
    }

    public uint GetWzOffset(uint positionFromStart, uint contentsStart, uint unencryptedOffset)
    {
        var offset = positionFromStart ^ 0xFFFFFFFF;
        offset *= _versionKey;
        offset -= 0x581C3F6D;
        offset = Rotl(offset, (byte) (offset & 0x1F));
        offset ^= unencryptedOffset - contentsStart * 2;

        return offset;
    }

    public void Serialize(BinaryWriter writer, WzProperty mainDirectory)
    {
        if (!_imgLocations.TryAdd(mainDirectory, (uint) writer.BaseStream.Position)) return;
        writer.Write((ushort) _versionHash);
        Resolve(writer, mainDirectory);

        var resolved = new ConcurrentBag<Tuple<long, WzProperty, byte[]>>();

        var running = false;
        var serializing = 0;
        var serialized = 0;
        var flushed = 0;
        var serializeThread = new Thread(() =>
        {
            running = true;
            while (_pending.Count > 0)
            {
                var thisBatch = new Tuple<long, WzProperty>[_pending.Count];
                for (var i = 0; i < thisBatch.Length && _pending.TryDequeue(out var entry); ++i)
                    thisBatch[i] = entry;
                Interlocked.Add(ref serializing, thisBatch.Length);

                if (Package._logging != null) Package._logging.LogDebug($"Serializing {thisBatch.Length} images");
                Parallel.ForEach(thisBatch, pendingNode =>
                {
                    if (pendingNode.Item2 == null) return;

                    using (var str = new MemoryStream())
                    using (var subWriter = new BinaryWriter(str))
                    {
                        Resolve(subWriter, pendingNode.Item2);
                        try
                        {
                            resolved.Add(new Tuple<long, WzProperty, byte[]>(pendingNode.Item1, pendingNode.Item2, str.ToArray()));
                        }
                        catch (Exception ex)
                        {
                            if (Package._logging != null) Package._logging.LogDebug($"Error serializing {pendingNode.Item2._path}, {ex.Message} @ {ex.StackTrace}");
                        }

                        Interlocked.Increment(ref serialized);
                    }
                });
                if (Package._logging != null) Package._logging.LogDebug("Serializing complete, checking for more");
            }

            if (Package._logging != null) Package._logging.LogDebug("No more images to serialize");
            running = false;
        });

        serializeThread.Start();

        while (running || _pending.Count > 0 || resolved.Count > 0)
        {
            if (resolved.Count == 0)
            {
                serializeThread.Join(5000);
                continue;
            }

            var resolvedThisBatch = new Tuple<long, WzProperty, byte[]>[Math.Min(resolved.Count, 256)];
            for (var i = 0; i < resolvedThisBatch.Length && resolved.TryTake(out var entry); ++i)
                resolvedThisBatch[i] = entry;

            if (Package._logging != null) Package._logging.LogDebug($"Flushing {resolvedThisBatch.Length} images \t {serialized}\t{flushed}\t{serializing}\t{(serialized * 100d / serializing).ToString("F2")}%\t{(flushed * 100d / serializing).ToString("F2")}%");

            for (var i = 0; i < resolvedThisBatch.Length; ++i)
            {
                var pendingResolvedNode = resolvedThisBatch[i];

                var position = (uint) writer.BaseStream.Position; // If it's over 4gb, you need to break that up. Sorry fam.
                writer.Write(pendingResolvedNode.Item3);
                var size = (uint) writer.BaseStream.Position - position;
                writer.BaseStream.Position = pendingResolvedNode.Item1 + 1;
                writer.Write(size);
                writer.BaseStream.Position = pendingResolvedNode.Item1 + 10;
                var container = pendingResolvedNode.Item2._container;
                var contentsStart = _imgLocations[container ?? mainDirectory];
                var positionFromStart = (uint) (writer.BaseStream.Position - contentsStart);
                writer.Write(GetWzOffset(positionFromStart, contentsStart, position)); // Is position supposed to be relative to contentsStart?
                writer.BaseStream.Position = position + size;
            }

            Interlocked.Add(ref flushed, resolvedThisBatch.Length);
            writer.Flush();

            if (resolved.Count > 256) continue;
            serializeThread.Join(5000);
        }
    }

    public void Serialize(string file, WzProperty mainDirectory)
    {
        using (var dest = File.OpenWrite(file))
        using (var writer = new BinaryWriter(dest))
        {
            writer.Write(Encoding.ASCII.GetBytes("PKG1"));
            // File size
            writer.Write((long) 0);
            // ContentsStartLocation
            writer.Write(0);
            var desc = Encoding.ASCII.GetBytes("Minimized WZ / MapleStory.IO");
            writer.Write(desc);
            writer.Write((byte) 0);
            var contentsStartLocation = (uint) dest.Position;
            dest.Position = 12;
            writer.Write(contentsStartLocation);
            dest.Position = contentsStartLocation;

            Serialize(writer, mainDirectory);
        }
    }

    private void GetStringToBlock(BinaryWriter writer, string s)
    {
        // TODO: Implement string de-duping
        writer.Write((byte) 0); // Not de-duped
        GetWzStringBytes(writer, s);
    }

    private void GetWzStringBytes(BinaryWriter writer, string s)
    {
        var isAscii = s.All(c => c <= byte.MaxValue);
        var length = s.Length * (isAscii ? -1 : 1);

        IEnumerable<byte> lengthBytes;
        if (length >= 127)
        {
            lengthBytes = new byte[] {127}.Concat(BitConverter.GetBytes(length));
        }
        else if (length <= -128)
        {
            lengthBytes = new byte[] {0x80}.Concat(BitConverter.GetBytes(length * -1));
        }
        else lengthBytes = new[] {(byte) length};

        writer.Write(lengthBytes.ToArray());
        lengthBytes = null;

        byte[] textData;
        if (isAscii)
        {
            textData = Encoding.ASCII.GetBytes(s);
            byte asciiMask = 0xAA;
            for (var i = 0; i < textData.Length; ++i) textData[i] = (byte) (textData[i] ^ asciiMask++);
        }
        else
        {
            textData = Encoding.Unicode.GetBytes(s);
            ushort unicodeMask = 0xAAAA;
            for (var i = 0; i < textData.Length; i += 2)
            {
                textData[i] ^= (byte) (unicodeMask & 0xFF);
                textData[i + 1] ^= (byte) ((unicodeMask >> 8) & 0xFF);

                unicodeMask++;
            }
        }

        writer.Write(textData);
    }

    private void SubProperty(BinaryWriter writer, WzProperty self)
    {
        writer.Write((short) 0);
        PropertyList(writer, self.GetChildren());
    }

    private void WzIntToByte(BinaryWriter writer, int val)
    {
        if (val > sbyte.MaxValue || val <= sbyte.MinValue)
            writer.Write(new byte[] {0x80}.Concat(BitConverter.GetBytes(val)).ToArray());
        else writer.Write((byte) val);
    }

    private void WzSingleToByte(BinaryWriter writer, float val)
    {
        if (val > sbyte.MaxValue || val <= sbyte.MinValue || Math.Round(val, 0) != val)
            writer.Write(new byte[] {0x80}.Concat(BitConverter.GetBytes(val)).ToArray());
        else writer.Write((byte) val);
    }

    private void WzLongToByte(BinaryWriter writer, long val)
    {
        if (val > sbyte.MaxValue || val <= sbyte.MinValue)
            writer.Write(new byte[] {0x80}.Concat(BitConverter.GetBytes(val)).ToArray());
        else writer.Write((byte) val);
    }

    private void WzFile(BinaryWriter writer, WzPropertyVal<byte[]> parent)
    {
        writer.Write(parent.Value);
    }

    private void Canvas(BinaryWriter writer, WzProperty self)
    {
        Image<Rgba32> toDispose = null;
        var img = (self is IWzPropertyVal<Image<Rgba32>> selfImg ? selfImg.Value : null) ?? (toDispose = new Image<Rgba32>(1, 1));
        var children = self.GetChildren().OrderBy(c => c._name).ToArray();
        byte[] pixelData = null;
        byte[] deflated = null;

        byte format = 7;

        if (NormalWz)
            using (var tmp = img.CloneAs<Bgra32>())
            {
                if (tmp.TryGetSinglePixelSpan(out var span))
                {
                    pixelData = span.ToArray().SelectMany(c => BitConverter.GetBytes(c.PackedValue)).ToArray();
                    format = 2;
                }
                else throw new Exception("Something went wrong");
            }
        else if (img.TryGetSinglePixelSpan(out var span))
            pixelData = span.ToArray().SelectMany(c => BitConverter.GetBytes(c.PackedValue)).ToArray();
        else throw new Exception("Something went wrong");

        if (pixelData.Length != img.Width * img.Height * 4)
            throw new InvalidDataException("Pixel data isn't the expected size");

        if (format == 7)
        {
            deflated = Deflate(pixelData);
            var gzipBad = false;
            using (var mem = new MemoryStream(deflated))
                gzipBad = WzReader.InflateGZip(mem).Length != pixelData.Length;

            var zlibBad = false;
            if (gzipBad)
            {
                format = 8;
                deflated = DeflateZLib(pixelData);
                using (var mem = new MemoryStream(deflated))
                    zlibBad = WzReader.Inflate(mem, (uint) pixelData.Length).Length != pixelData.Length;
            }

            if (gzipBad && zlibBad)
            {
                format = 5; // Raw RGBA32, which is gigantic and horrible.
                deflated = pixelData;
            }
        }

        writer.Write((byte) 0);
        if (children.Length > 0)
        {
            writer.Write(new byte[] {1, 0, 0});
            PropertyList(writer, children);
        }
        else writer.Write((byte) 0);

        WzIntToByte(writer, img.Width);
        WzIntToByte(writer, img.Height);
        writer.Write(new byte[] {0, format});

        writer.Write(new byte[4]); // Unk
        writer.Write(BitConverter.GetBytes(deflated.Length + 1));
        writer.Write((byte) 0);
        writer.Write(deflated);

        toDispose?.Dispose();
    }
}