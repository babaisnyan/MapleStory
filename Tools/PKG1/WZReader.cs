using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace PKG1;

public class WzReader : BinaryReader
{
    public static byte[] _gmsKey;
    public static byte[] _kmsKey;
    private static readonly byte[] AesKey = {0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00};
    private static readonly byte[] Gmsiv = {0x4D, 0x23, 0xC7, 0x2B, 0x4D, 0x23, 0xC7, 0x2B, 0x4D, 0x23, 0xC7, 0x2B, 0x4D, 0x23, 0xC7, 0x2B};
    private static readonly byte[] Kmsiv = {0xB9, 0x7D, 0x63, 0xE9, 0xB9, 0x7D, 0x63, 0xE9, 0xB9, 0x7D, 0x63, 0xE9, 0xB9, 0x7D, 0x63, 0xE9};

    public uint _versionKey;
    public byte? _versionHash;
    public uint _contentsStart;
    public Package _package;
    public WzProperty _container;

    public WzReader(Package package, WzProperty container, Stream str, uint versionKey, byte? versionHash, uint contentsStart) : base(str)
    {
        _versionHash = versionHash;
        _versionKey = versionKey;
        _contentsStart = contentsStart;
        _package = package;
        _container = container;
    }

    public WzReader(Package package, WzProperty container, Stream str, Encoding encoding, uint versionKey, byte? versionHash, uint contentsStart) : base(str, encoding)
    {
        _versionHash = versionHash;
        _versionKey = versionKey;
        _contentsStart = contentsStart;
        _package = package;
        _container = container;
    }

    /// <summary>
    ///     This enum is used to specify the WZ key to be used.
    /// </summary>
    public enum WzVariant
    {
        /// <summary>
        ///     MapleStory SEA
        /// </summary>
        Msea = 0,

        /// <summary>
        ///     Korea MapleStory
        /// </summary>
        Kms = 0,

        /// <summary>
        ///     Korea MapleStory (Tespia)
        /// </summary>
        Kmst = 0,

        /// <summary>
        ///     Japan MapleStory
        /// </summary>
        Jms = 0,

        /// <summary>
        ///     Japan MapleStory (Tespia)
        /// </summary>
        Jmst = 0,

        /// <summary>
        ///     Europe MapleStory
        /// </summary>
        Ems = 0,

        /// <summary>
        ///     Global MapleStory
        /// </summary>
        Gms = 1,

        /// <summary>
        ///     Global MapleStory (Tespia)
        /// </summary>
        Gmst = 1,

        /// <summary>
        ///     Taiwan MapleStory
        /// </summary>
        Tms = 1,

        /// <summary>
        ///     Brazil MapleStory
        /// </summary>
        Bms = 2,

        /// <summary>
        ///     Classic MapleStory (Data.wz)
        /// </summary>
        Classic = 2
    }

    public long PositionFromStart => BaseStream.Position - _contentsStart;

    public static void InitializeKeys()
    {
        Task.WaitAll(
            Task.Run(() =>
            {
                if (File.Exists("gms.aes")) _gmsKey = File.ReadAllBytes("gms.aes");
                else File.WriteAllBytes("gms.aes", _gmsKey = GetWzKey(WzVariant.Gms));
            }),
            Task.Run(() =>
            {
                if (File.Exists("kms.aes")) _kmsKey = File.ReadAllBytes("kms.aes");
                else File.WriteAllBytes("kms.aes", _kmsKey = GetWzKey(WzVariant.Kms));
            })
        );
    }

    public static byte[] InflateGZip(Stream data)
    {
        using (var result = new MemoryStream())
        {
            using (var inflate = new GZipStream(data, CompressionMode.Decompress, true))
                inflate.CopyTo(result);

            result.Position = 0;
            return result.ToArray();
        }
    }

    public static byte[] Inflate(Stream data, uint dataLength)
    {
        long length = 512 * 1024;
        length = Math.Max(dataLength, length);
        var dec = new byte[length];
        using (var deflator = new DeflateStream(data, CompressionMode.Decompress, true))
        using (var @out = new MemoryStream(dec.Length * 2))
        {
            int len;
            while ((len = deflator.Read(dec, 0, dec.Length)) > 0) @out.Write(dec, 0, len);
            return @out.ToArray();
        }
    }

    public static byte[] GetPixelDataDxt3(byte[] rawData, int width, int height)
    {
        var pixel = new byte[width * height * 4];

        var colorTable = new Rgba32[4];
        var colorIdxTable = new int[16];
        var alphaTable = new byte[16];
        for (var y = 0; y < height; y += 4)
        {
            for (var x = 0; x < width; x += 4)
            {
                var off = x * 4 + y * width;
                ExpandAlphaTableDxt3(alphaTable, rawData, off);
                var u0 = BitConverter.ToUInt16(rawData, off + 8);
                var u1 = BitConverter.ToUInt16(rawData, off + 10);
                ExpandColorTable(colorTable, u0, u1);
                ExpandColorIndexTable(colorIdxTable, rawData, off + 12);

                for (var j = 0; j < 4; j++)
                {
                    for (var i = 0; i < 4; i++)
                    {
                        SetPixel(pixel,
                            x + i,
                            y + j,
                            width,
                            colorTable[colorIdxTable[j * 4 + i]],
                            alphaTable[j * 4 + i]);
                    }
                }
            }
        }

        return pixel;
    }

    public static byte[] GetPixelDataDxt5(byte[] rawData, int width, int height)
    {
        var pixel = new byte[width * height * 4];

        var colorTable = new Rgba32[4];
        var colorIdxTable = new int[16];
        var alphaTable = new byte[8];
        var alphaIdxTable = new int[16];
        for (var y = 0; y < height; y += 4)
        {
            for (var x = 0; x < width; x += 4)
            {
                var off = x * 4 + y * width;
                ExpandAlphaTableDxt5(alphaTable, rawData[off + 0], rawData[off + 1]);
                ExpandAlphaIndexTableDxt5(alphaIdxTable, rawData, off + 2);
                var u0 = BitConverter.ToUInt16(rawData, off + 8);
                var u1 = BitConverter.ToUInt16(rawData, off + 10);
                ExpandColorTable(colorTable, u0, u1);
                ExpandColorIndexTable(colorIdxTable, rawData, off + 12);

                for (var j = 0; j < 4; j++)
                {
                    for (var i = 0; i < 4; i++)
                    {
                        SetPixel(pixel,
                            x + i,
                            y + j,
                            width,
                            colorTable[colorIdxTable[j * 4 + i]],
                            alphaTable[alphaIdxTable[j * 4 + i]]);
                    }
                }
            }
        }

        return pixel;
    }

    private static byte[] GetWzKey(WzVariant version)
    {
        switch ((int) version)
        {
            case 0:
                return GenerateKey(Kmsiv, AesKey);
            case 1:
                return GenerateKey(Gmsiv, AesKey);
            case 2:
                return new byte[0x10000];
            default:
                throw new ArgumentException("Invalid WZ variant passed.", "version");
        }
    }

    private static byte[] GenerateKey(byte[] iv, byte[] aesKey)
    {
        using (var memStream = new MemoryStream(0x100000))
        using (var aem = Aes.Create())
        {
            aem.KeySize = 256;
            aem.Key = aesKey;
            aem.Mode = CipherMode.ECB;
            using (var cStream = new CryptoStream(memStream, aem.CreateEncryptor(), CryptoStreamMode.Write))
            {
                cStream.Write(iv, 0, 16);
                for (var i = 0; i < 0x100000 - 16; i += 16)
                    cStream.Write(memStream.ToArray(), i, 16);
                cStream.Flush();
                return memStream.ToArray();
            }
        }
    }

    private static void SetPixel(byte[] pixelData, int x, int y, int width, Rgba32 color, byte alpha)
    {
        var offset = (y * width + x) * 4;
        pixelData[offset + 0] = color.B;
        pixelData[offset + 1] = color.G;
        pixelData[offset + 2] = color.R;
        pixelData[offset + 3] = alpha;
    }

    private static void ExpandColorTable(Rgba32[] color, ushort c0, ushort c1)
    {
        color[0] = Rgb565ToColor(c0);
        color[1] = Rgb565ToColor(c1);
        if (c0 > c1)
        {
            color[2] = new Rgba32((byte) ((color[0].R * 2f + color[1].R + 1) / 3f), (byte) ((color[0].G * 2f + color[1].G + 1) / 3f), (byte) ((color[0].B * 2f + color[1].B + 1) / 3f), 255);
            color[3] = new Rgba32((byte) ((color[0].R + color[1].R * 2f + 1) / 3f), (byte) ((color[0].G + color[1].G * 2f + 1) / 3f), (byte) ((color[0].B + color[1].B * 2f + 1) / 3f), 255);
        }
        else
        {
            color[2] = new Rgba32((byte) ((color[0].R + color[1].R) / 2f), (byte) ((color[0].G + color[1].G) / 2f), (byte) ((color[0].B + color[1].B) / 2f), 255);
            color[3] = new Rgba32(0, 0, 0f);
        }
    }

    private static void ExpandColorIndexTable(int[] colorIndex, byte[] rawData, int offset)
    {
        for (var i = 0; i < 16; i += 4, offset++)
        {
            colorIndex[i + 0] = rawData[offset] & 0x03;
            colorIndex[i + 1] = (rawData[offset] & 0x0c) >> 2;
            colorIndex[i + 2] = (rawData[offset] & 0x30) >> 4;
            colorIndex[i + 3] = (rawData[offset] & 0xc0) >> 6;
        }
    }

    private static void ExpandAlphaTableDxt3(byte[] alpha, byte[] rawData, int offset)
    {
        for (var i = 0; i < 16; i += 2, offset++)
        {
            alpha[i + 0] = (byte) (rawData[offset] & 0x0f);
            alpha[i + 1] = (byte) ((rawData[offset] & 0xf0) >> 4);
        }

        for (var i = 0; i < 16; i++)
            alpha[i] = (byte) (alpha[i] | (alpha[i] << 4));
    }

    private static void ExpandAlphaTableDxt5(byte[] alpha, byte a0, byte a1)
    {
        alpha[0] = a0;
        alpha[1] = a1;
        if (a0 > a1)
            for (var i = 2; i < 8; i++)
                alpha[i] = (byte) (((8 - i) * a0 + (i - 1) * a1 + 3) / 7);
        else
        {
            for (var i = 2; i < 6; i++)
                alpha[i] = (byte) (((6 - i) * a0 + (i - 1) * a1 + 2) / 5);
            alpha[6] = 0;
            alpha[7] = 255;
        }
    }

    private static void ExpandAlphaIndexTableDxt5(int[] alphaIndex, byte[] rawData, int offset)
    {
        for (var i = 0; i < 16; i += 8, offset += 3)
        {
            var flags = rawData[offset]
                      | (rawData[offset + 1] << 8)
                      | (rawData[offset + 2] << 16);
            for (var j = 0; j < 8; j++)
            {
                var mask = 0x07 << (3 * j);
                alphaIndex[i + j] = (flags & mask) >> (3 * j);
            }
        }
    }

    private static Rgba32 Rgb565ToColor(ushort val)
    {
        const int rgb565MaskR = 0xf800;
        const int rgb565MaskG = 0x07e0;
        const int rgb565MaskB = 0x001f;
        var r = (val & rgb565MaskR) >> 11;
        var g = (val & rgb565MaskG) >> 5;
        var b = val & rgb565MaskB;
        var c = new Rgba32(
            (byte) ((r << 3) | (r >> 2)),
            (byte) ((g << 2) | (g >> 4)),
            (byte) ((b << 3) | (b >> 2)));
        return c;
    }

    private static uint Rotl(uint value, byte shiftTimes)
    {
        return (value << shiftTimes) | (value >> (32 - shiftTimes));
    }

    public string ReadString(int length)
    {
        return new string(ReadChars(length));
    }

    public string ReadNulTerminatedString(int maxLength)
    {
        var builder = new StringBuilder();
        byte lastChar;
        do
        {
            lastChar = ReadByte();
            if (lastChar != 0) builder.Append((char) lastChar);
            if (builder.Length == maxLength) break;
        } while (lastChar != 0);

        return builder.ToString();
    }

    public byte[] GetWzStringBlockBytes(EncryptionType encrypted = EncryptionType.None)
    {
        var type = ReadByte();
        switch (type)
        {
            case 0:
            case 0x73:
                return GetWzStringBytes(false, encrypted);
            case 1:
            case 0x1B:
                return GetDeDupedStringBytes(false, encrypted);
            case 0xBF:
                BaseStream.Seek(-1, SeekOrigin.Current);
                _ = ReadBytes(100);
                throw new Exception($"Unknown type ({type}) of string block");
            default:
                throw new Exception($"Unknown type ({type}) of string block");
        }
    }

    public string ReadWzStringBlock(EncryptionType encrypted = EncryptionType.None)
    {
        var type = ReadByte();
        switch (type)
        {
            case 0:
            case 0x73:
                return ReadWzString(false, encrypted);
            case 1:
            case 0x1B:
                return ReadDeDupedString(false, encrypted);
            case 0xBF:
                BaseStream.Seek(-1, SeekOrigin.Current);
                _ = ReadBytes(100);
                throw new Exception($"Unknown type ({type}) of string block");
            default:
                throw new Exception($"Unknown type ({type}) of string block");
        }
    }

    public byte[] GetLuaScriptBytes(bool readByte = false)
    {
        if (readByte && ReadByte() == 0) return new byte[] {0};
        int length = ReadSByte();
        var isUnicode = length > 0;
        if (isUnicode)
        {
            if (length == 127) length = ReadInt32();
            length *= 2;
        }
        else
        {
            if (length == -128) length = ReadInt32();
            else length *= -1;
        }

        var textData = DecryptBytes(ReadBytes(length), _kmsKey);

        return textData.ToArray();
    }

    public string ReadLuaScript(bool readByte = false)
    {
        if (readByte && ReadByte() == 0) return "";
        int length = ReadSByte();
        var isUnicode = length > 0;
        if (isUnicode)
        {
            if (length == 127) length = ReadInt32();
            length *= 2;
        }
        else
        {
            if (length == -128) length = ReadInt32();
            else length *= -1;
        }

        var textData = DecryptBytes(ReadBytes(length), _kmsKey);

        if (!isUnicode) return Encoding.ASCII.GetString(textData.ToArray());
        return Encoding.Unicode.GetString(textData.ToArray());
    }

    public byte[] GetWzStringBytes(bool readByte, EncryptionType encrypted)
    {
        if (readByte && ReadByte() == 0) return new byte[0];
        var encryptionKey = encrypted == EncryptionType.None ? null : encrypted == EncryptionType.Gms ? _gmsKey : _kmsKey;
        int length = ReadSByte();
        var isUnicode = length > 0;
        if (isUnicode)
        {
            if (length == 127) length = ReadInt32();
            length *= 2;
        }
        else
        {
            if (length == -128) length = ReadInt32();
            else length *= -1;
        }

        var textData = ReadBytes(length);
        byte asciiMask = 0xAA;
        ushort unicodeMask = 0xAAAA;

        if (!isUnicode)
        {
            var asciiBytes = textData.Select(c => (byte) (c ^ asciiMask++)).ToArray();
            if (encryptionKey != null)
                asciiBytes = asciiBytes.Select((c, i) => (byte) (c ^ encryptionKey[i])).ToArray();
            return asciiBytes;
        }

        for (var i = 0; i < length; i += 2)
        {
            textData[i] ^= (byte) (unicodeMask & 0xFF);
            textData[i + 1] ^= (byte) ((unicodeMask >> 8) & 0xFF);

            if (encryptionKey != null)
            {
                textData[i] ^= encryptionKey[i];
                textData[i + 1] ^= encryptionKey[i + 1];
            }

            unicodeMask++;
        }

        return textData;
    }

    public bool ReadWzStringExpecting(out EncryptionType encryptedWith, string expecting, bool readByte = false)
    {
        if (readByte && ReadByte() == 0)
        {
            encryptedWith = EncryptionType.None;
            return false;
        }

        int length = ReadSByte();
        var isUnicode = length > 0;
        if (isUnicode)
        {
            if (length == 127) length = ReadInt32();
            length *= 2;
        }
        else
        {
            if (length == -128) length = ReadInt32();
            else length *= -1;
        }

        var textData = ReadBytes(length);

        EncryptionType[] types = {EncryptionType.None, EncryptionType.Gms, EncryptionType.Kms};
        foreach (var encrypted in types)
        {
            byte asciiMask = 0xAA;
            ushort unicodeMask = 0xAAAA;

            string found = null;
            var encryptionKey = encrypted == EncryptionType.None ? null : encrypted == EncryptionType.Gms ? _gmsKey : _kmsKey;
            if (!isUnicode)
            {
                var asciiBytes = textData.Select(c => (byte) (c ^ asciiMask++)).ToArray();
                if (encryptionKey != null)
                    asciiBytes = asciiBytes.Select((c, i) => (byte) (c ^ encryptionKey[i])).ToArray();
                found = Encoding.ASCII.GetString(asciiBytes);
                if (found.Equals(expecting))
                {
                    encryptedWith = encrypted;
                    return true;
                }

                continue;
            }

            for (var i = 0; i < length; i += 2)
            {
                textData[i] ^= (byte) (unicodeMask & 0xFF);
                textData[i + 1] ^= (byte) ((unicodeMask >> 8) & 0xFF);

                if (encryptionKey != null)
                {
                    textData[i] ^= encryptionKey[i];
                    textData[i + 1] ^= encryptionKey[i + 1];
                }

                unicodeMask++;
            }

            found = Encoding.Unicode.GetString(textData);
            if (found.Equals(expecting))
            {
                encryptedWith = encrypted;
                return true;
            }
        }

        encryptedWith = EncryptionType.None;
        return false;
    }

    public string ReadWzString(bool readByte = false, EncryptionType encrypted = EncryptionType.None /*, int retryCount = 0*/)
    {
        if (readByte && ReadByte() == 0) return "";
        var encryptionKey = encrypted == EncryptionType.None || encrypted == (EncryptionType.Kms | EncryptionType.Gms) ? null : encrypted == EncryptionType.Gms ? _gmsKey : _kmsKey;
        var oldPos = BaseStream.Position;
        int length = ReadSByte();
        var isUnicode = length > 0;
        if (isUnicode)
        {
            if (length == 127) length = ReadInt32();
            length *= 2;
        }
        else
        {
            if (length == -128) length = ReadInt32();
            else length *= -1;
        }

        var textData = ReadBytes(length);
        byte asciiMask = 0xAA;
        ushort unicodeMask = 0xAAAA;

        if (!isUnicode)
        {
            var asciiBytes = textData.Select(c => (byte) (c ^ asciiMask++)).ToArray();
            if (encryptionKey != null)
                asciiBytes = asciiBytes.Select((c, i) => (byte) (c ^ encryptionKey[i])).ToArray();

            var res = Encoding.ASCII.GetString(asciiBytes);
            // Hackish fix for Kimchi
            //if (res.Any(c => c > 126 || (c < 45 && c != 32)) && retryCount <= 3)
            //{
            //    BaseStream.Position = oldPos;
            //    string subRes = ReadWZString(readByte, (EncryptionType)((((int)encrypted) + retryCount) % 3), ++retryCount);

            //    // If the sub result is better, return that instead.
            //    if (!subRes.Any(c => c > 126 || (c < 45 && c != 32))) return subRes;
            //}
            // If we detect no abnormalities, or can't figure out what to try next, just return as is.
            return res;
        }

        for (var i = 0; i < length; i += 2)
        {
            textData[i] ^= (byte) (unicodeMask & 0xFF);
            textData[i + 1] ^= (byte) ((unicodeMask >> 8) & 0xFF);

            if (encryptionKey != null)
            {
                textData[i] ^= encryptionKey[i];
                textData[i + 1] ^= encryptionKey[i + 1];
            }

            unicodeMask++;
        }

        return Encoding.Unicode.GetString(textData);
    }

    public byte[] GetDeDupedStringBytes(bool readByte = false, EncryptionType encrypted = EncryptionType.None)
    {
        return GetWzStringAtOffsetBytes(ReadInt32(), readByte, encrypted);
    }

    public string ReadDeDupedString(bool readByte = false, EncryptionType encrypted = EncryptionType.None)
    {
        var offset = ReadInt32();

        if (_package._deDupedStrings.TryGetValue((_container._pseudoContentStart ?? _container._containerStartLocation) + offset, out var cached)) return cached;
        var result = ReadWzStringAtOffset(offset, readByte, encrypted);
        while (!_package._deDupedStrings.TryGetValue(_container._containerStartLocation + offset, out cached))
        {
            try
            {
                if (!_package._deDupedStrings.TryAdd(_container._containerStartLocation + offset, result) && _package._deDupedStrings.TryGetValue(_container._containerStartLocation + offset, out cached))
                    return cached;
            }
            catch (Exception) { } // Apparently the TryAdd can still throw exceptions for duplicate keys that bubble up and get caught outside of here
        }

        if (cached != result) throw new InvalidOperationException("Invalid cached de-duped string in place");
        return result;
    }

    public byte[] GetWzStringAtOffsetBytes(long offset, bool readByte = false, EncryptionType encrypted = EncryptionType.None)
    {
        var currentOffset = BaseStream.Position;

        BaseStream.Seek(_container._containerStartLocation + offset, SeekOrigin.Begin);
        var result = GetWzStringBytes(readByte, encrypted);
        BaseStream.Seek(currentOffset, SeekOrigin.Begin);

        return result;
    }

    public string ReadWzStringAtOffset(long offset, bool readByte = false, EncryptionType encrypted = EncryptionType.None)
    {
        var currentOffset = BaseStream.Position;

        BaseStream.Seek(_container._containerStartLocation + offset, SeekOrigin.Begin);
        var result = ReadWzString(readByte, encrypted);
        BaseStream.Seek(currentOffset, SeekOrigin.Begin);

        return result;
    }

    public byte[] GetWzSingleBytes()
    {
        var t = ReadByte();
        return t == 0x80 ? new[] {t, ReadByte(), ReadByte(), ReadByte(), ReadByte()} : new[] {t};
    }

    public float ReadWzSingle()
    {
        var t = ReadByte();
        if (t == 0x80)
            return ReadSingle();
        if (t == 0)
            return t;
        return t; // This seems to contradict latest versions, but v55 seems to have some singles that aren't 0 but return as a single byte
    }

    public byte[] GetWzIntBytes()
    {
        var t = ReadByte();
        return t == 0x80 ? new[] {t}.Concat(ReadBytes(4)).ToArray() : new[] {t};
    }

    public int ReadWzInt()
    {
        var possible = ReadSByte();
        if (possible == -128) return ReadInt32();
        return possible;
    }

    public byte[] GetWzLongBytes()
    {
        var t = ReadByte();
        return t == 0x80 ? new[] {t}.Concat(ReadBytes(8)).ToArray() : new[] {t};
    }

    public long ReadWzLong()
    {
        var possible = ReadSByte();
        if (possible == -128) return ReadInt64();
        return possible;
    }

    /// This shit's fucked up, yo.
    public uint ReadWzOffset()
    {
        var offset = (uint) (PositionFromStart ^ 0xFFFFFFFF);
        offset *= _versionKey;
        offset -= 0x581C3F6D;
        offset = Rotl(offset, (byte) (offset & 0x1F));
        var encryptedOffset = ReadUInt32();
        offset ^= encryptedOffset;
        offset += _contentsStart * 2;

        return offset;
    }

    internal IEnumerable<IEnumerable<byte>> DecryptPngNested(Stream inData, uint length, byte[] wzKey = null)
    {
        var expectedEndPosition = inData.Position + length;
        using (var reader = new BinaryReader(inData, Encoding.ASCII, true))
        {
            while (inData.Position < expectedEndPosition)
            {
                var blockLen = reader.ReadInt32();
                yield return DecryptBytes(reader.ReadBytes(blockLen), wzKey);
            }
        }
    }

    internal byte[] GetPngBytes(int width, int height, int format, bool isEncrypted, uint imageLength, byte[] encryptionKey = null)
    {
        // For use restoring position if we need to switch to KMS Key, by default we use GMS Key
        var position = BaseStream.Position;
        var sourceData = isEncrypted ? DecryptPng(BaseStream, imageLength, encryptionKey).ToArray() : ReadBytes((int) imageLength);
        using (Stream sourceDataFromStream = new MemoryStream(sourceData, 2, sourceData.Length - 2))
        {
            try
            {
                sourceData = Inflate(sourceDataFromStream, imageLength);
            }
            catch (Exception)
            {
                if (isEncrypted && encryptionKey == null)
                {
                    BaseStream.Position = position;
                    return GetPngBytes(width, height, format, isEncrypted, imageLength, _kmsKey);
                }
            }

            return sourceData;
        }
    }

    internal Image<Rgba32> ParsePng(int width, int height, int format, bool isEncrypted, uint imageLength, byte[] encryptionKey = null)
    {
        // For use restoring position if we need to switch to KMS Key, by default we use GMS Key
        var position = BaseStream.Position;
        var sourceData = isEncrypted ? DecryptPng(BaseStream, imageLength, encryptionKey).ToArray() : ReadBytes((int) imageLength);
        // Not sure why I need this fix, but k
        using (Stream sourceDataFromStream = format == 6 ? new MemoryStream(sourceData) :
               format == 7 ? new MemoryStream(sourceData) : new MemoryStream(sourceData, 2, sourceData.Length - 2))
        {
            try
            {
                if (format == 6 || format == 7)
                    sourceData = InflateGZip(sourceDataFromStream);
                else if (format != 5)
                    sourceData = Inflate(sourceDataFromStream, imageLength);
            }
            catch (Exception)
            {
                if (isEncrypted && encryptionKey == null)
                {
                    BaseStream.Position = position;
                    return ParsePng(width, height, format, isEncrypted, imageLength, _kmsKey); // Should I try supporting GMSKey here? Not sure I've ever seen it needed.
                }

                throw new InvalidOperationException("Encrypted with unknown key");
            }
        }

        var sourceDataLength = sourceData.Length;

        switch (format)
        {
            case 1: // Transform
                var destinationData = new byte[width * height * 4];
                for (var i = 0; i < sourceDataLength; ++i)
                {
                    destinationData[i * 2] = (byte) ((sourceData[i] & 0x0F) * 0x11);
                    destinationData[i * 2 + 1] = (byte) (((sourceData[i] & 0xF0) >> 4) * 0x11);
                }

                sourceDataLength *= 2;
                sourceData = destinationData;
                goto case 2;
            case 2:
                if (sourceDataLength != width * height * 4)
                {
                    var proper = new byte[width * height * 4];
                    Buffer.BlockCopy(sourceData, 0, proper, 0, Math.Min(proper.Length, sourceDataLength));
                    sourceData = proper;
                }

                using (var tmp = Image.LoadPixelData<Bgra32>(sourceData, width, height))
                    return tmp.CloneAs<Rgba32>();
            case 4:
            case 5:
            case 6:
            case 7:
                if (sourceDataLength != width * height * 4)
                {
                    var proper = new byte[width * height * 4];
                    Buffer.BlockCopy(sourceData, 0, proper, 0, Math.Min(proper.Length, sourceDataLength));
                    sourceData = proper;
                }

                return Image.LoadPixelData<Rgba32>(sourceData, width, height);
            case 257:
                using (var tmp = Image.LoadPixelData<Bgra5551>(sourceData, width, height))
                    return tmp.CloneAs<Rgba32>();
            case 513:
                if (sourceDataLength != width * height * 2)
                {
                    var proper = new byte[width * height * 2];
                    Buffer.BlockCopy(sourceData, 0, proper, 0, Math.Min(proper.Length, sourceDataLength));
                    sourceData = proper;
                }

                using (var tmp = Image.LoadPixelData<Bgr565>(sourceData, width, height))
                    return tmp.CloneAs<Rgba32>();
            case 517:
                width >>= 4;
                height >>= 4;
                goto case 513;
            case 1026: //dxt3
                destinationData = GetPixelDataDxt3(sourceData, width, height);
                using (var tmp = Image.LoadPixelData<Bgra32>(destinationData, width, height))
                    return tmp.CloneAs<Rgba32>();
            case 2050:
                destinationData = GetPixelDataDxt5(sourceData, width, height);
                using (var tmp = Image.LoadPixelData<Bgra32>(destinationData, width, height))
                    return tmp.CloneAs<Rgba32>();
            default:
                throw new InvalidOperationException("Unknown canvas format");
        }
    }

    /// <summary>
    ///     Executes a delegate of type <see cref="System.Action" /> , then sets the position of the backing stream back to the
    ///     original value.
    /// </summary>
    /// <param name="result"> The delegate to execute. </param>
    internal void PeekFor(Action result)
    {
        var orig = BaseStream.Position;
        try
        {
            result();
        }
        finally
        {
            BaseStream.Position = orig;
        }
    }

    /// <summary>
    ///     Executes a delegate of type <see cref="System.Func{TResult}" /> , then sets the position of the backing stream back
    ///     to the original value.
    /// </summary>
    /// <typeparam name="T"> The return type of the delegate. </typeparam>
    /// <param name="result"> The delegate to execute. </param>
    /// <returns> The object returned by the delegate. </returns>
    internal T PeekFor<T>(Func<T> result)
    {
        var orig = BaseStream.Position;
        try
        {
            return result();
        }
        finally
        {
            BaseStream.Position = orig;
        }
    }

    private IEnumerable<byte> DecryptBytes(IEnumerable<byte> bytes, byte[] wzKey = null)
    {
        // Assume GMS for now, need to pull out into global collection
        wzKey = wzKey ?? _gmsKey;
        return bytes.Select((a, i) => (byte) (a ^ wzKey[i]));
    }

    private IEnumerable<byte> DecryptPng(Stream inData, uint length, byte[] wzKey = null)
    {
        return DecryptPngNested(inData, length, wzKey).SelectMany(c => c);
    }
}