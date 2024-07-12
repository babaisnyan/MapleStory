using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using Microsoft.Extensions.Logging;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using Utilities;

namespace PKG1;

public static class PropertyResolvers
{
    private static readonly int[] ValidDirectoryChildrenTypes = {1, 2, 3, 4};

    public static WzProperty Image(WzReader reader, WzProperty self)
    {
        if (self._offset >= reader.BaseStream.Length) return self;
        self._containerStartLocation = self._offset;
        reader._container = self;
        var imgType = reader.ReadByte();
        if (imgType == 1)
            return new WzPropertyWeak<string>(() =>
            {
                using (reader = GetContentReader(null, self))
                {
                    reader.BaseStream.Seek(self._offset, SeekOrigin.Begin);
                    reader.ReadByte();
                    return reader.ReadLuaScript();
                }
            }, self._name, self._path, self._fileContainer, PropertyType.Lua, self._parent, self._size, self._checksum, self._offset);

        if (imgType != 0x73)
        {
            reader.BaseStream.Position--;
            var imgData = reader.ReadBytes((int) self._size);

            throw new InvalidOperationException("Unknown image type, not supported!");
        }

        var oldPosition = reader.BaseStream.Position;
        if (!reader.ReadWzString(false, self._encrypted).Equals("Property"))
        {
            reader.BaseStream.Position = oldPosition;
            if (!reader.ReadWzStringExpecting(out self._encrypted, "Property"))
                throw new InvalidOperationException("Unknown encryption method");
        }

        if (reader.ReadInt16() != 0) throw new InvalidOperationException("Image needs to have 0 as part of header");

        return self;
    }

    public static IEnumerable<WzProperty> ImageChildren(WzProperty self)
    {
        using (var reader = GetContentReader(null, self))
        {
            if (self._offset >= reader.BaseStream.Length) yield break;
            reader.BaseStream.Seek(self._offset, SeekOrigin.Begin);

            if (reader.ReadByte() != 0x73) throw new InvalidOperationException("Image should have header of 0x73");
            var oldPosition = reader.BaseStream.Position;
            if (!reader.ReadWzString(false, self._encrypted).Equals("Property"))
            {
                reader.BaseStream.Position = oldPosition;
                if (!reader.ReadWzStringExpecting(out self._encrypted, "Property"))
                    throw new InvalidOperationException("Unknown encryption method");
            }

            if (reader.ReadInt16() != 0) throw new InvalidOperationException("Image needs to have 0 as part of header");

            foreach (var prop in PropertyList(reader, self).ToArray()) yield return prop;
        }
    }

    public static IEnumerable<WzProperty> SubPropChildren(WzProperty self)
    {
        using (var reader = GetContentReader(null, self))
        {
            reader.BaseStream.Seek(self._offset, SeekOrigin.Begin);
            foreach (var prop in PropertyList(reader, self).ToArray()) yield return prop;
        }
    }

    public static IEnumerable<WzProperty> PropertyList(WzReader reader, WzProperty parent)
    {
        if (reader.BaseStream.Position + 4 >= reader.BaseStream.Length) return new WzProperty[0];
        return Enumerable.Range(0, reader.ReadWzInt()).Select(i =>
        {
            var position = (uint) reader.BaseStream.Position;
            if (position >= reader.BaseStream.Length) return null;
            var name = reader.ReadWzStringBlock(parent?._encrypted | parent?._container?._encrypted ?? EncryptionType.None);
            var type = reader.ReadByte();
            switch (type)
            {
                case 0:
                    return new WzProperty(name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Null, parent, 0, 0, position);
                case 0x10:
                    return new WzPropertyVal<sbyte>(reader.ReadSByte(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.UInt16, parent, 1, 0, position);
                case 0x11:
                    return new WzPropertyVal<byte>(reader.ReadByte(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.UInt16, parent, 1, 0, position);
                case 0x0B:
                case 2:
                case 0x12:
                    return new WzPropertyVal<ushort>(reader.ReadUInt16(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.UInt16, parent, 2, 0, position);
                case 3:
                    return new WzPropertyVal<int>(reader.ReadWzInt(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Int32, parent, 4, 0, position);
                case 19:
                    return new WzPropertyVal<Rgba32>(new Rgba32((uint) reader.ReadWzInt()), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Int32, parent, 4, 0, position);
                case 4:
                    return new WzPropertyVal<float>(reader.ReadWzSingle(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Single, parent, 4, 0, position);
                case 5:
                    return new WzPropertyVal<double>(reader.ReadDouble(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Double, parent, 8, 0, position);
                case 8:
                    return new WzPropertyVal<string>(reader.ReadWzStringBlock(parent?._encrypted | parent?._container?._encrypted ?? EncryptionType.None), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.String, parent, 0, 0, position);
                case 9:
                    var blockLen = reader.ReadUInt32();
                    var result = reader.PeekFor(() => ParseExtendedProperty(reader, parent, name, blockLen));
                    reader.BaseStream.Seek(blockLen, SeekOrigin.Current);
                    return result;
                case 20:
                    return new WzPropertyVal<long>(reader.ReadWzLong(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Int64, parent, 8, 0, position);
                case 21:
                    return new WzPropertyVal<ulong>((ulong) reader.ReadWzLong(), name, PathCombine(parent._path, name), parent._fileContainer, PropertyType.Int64, parent, 8, 0, position);
                default:
                    throw new Exception("Unknown property type at ParsePropertyList");
            }
        });
    }

    public static byte[] ResolveHash(WzProperty prop)
    {
        using (var reader = GetContentReader(null, prop))
        using (var sha = SHA512.Create())
        {
            reader.BaseStream.Position = prop._offset;
            switch (prop.Type)
            {
                case PropertyType.Image:
                    reader.BaseStream.Position = prop._containerStartLocation;
                    var imgType = reader.ReadByte();
                    if (imgType == 1)
                    {
                        var unk = reader.ReadByte();
                        return reader.GetLuaScriptBytes().ToArray();
                    }

                    return new byte[0];
                case PropertyType.Directory: return new byte[0];
                case PropertyType.SubProperty: return new byte[0];
                case PropertyType.Convex: return new byte[0];
                case PropertyType.Vector2: return sha.ComputeHash(reader.GetWzIntBytes().Concat(reader.GetWzIntBytes()).ToArray());
                case PropertyType.Uol: return sha.ComputeHash(reader.GetWzStringBlockBytes(prop._encrypted | prop._container._encrypted));
                case PropertyType.Audio:
                    using (Stream sub = new SubStream(reader.BaseStream, (int) reader.BaseStream.Position, prop._size))
                        return sha.ComputeHash(sub);
                case PropertyType.Canvas:
                    reader.BaseStream.Seek(1, SeekOrigin.Current);
                    byte[][] childHashes;
                    if (reader.ReadByte() == 1)
                    {
                        reader.BaseStream.Seek(2, SeekOrigin.Current);
                        childHashes = PropertyList(reader, prop).ToArray().Select(c => ResolveHash(c)).ToArray();
                    }
                    else
                        childHashes = new byte[0][];

                    var width = reader.ReadWzInt(); // width
                    var height = reader.ReadWzInt(); // height
                    var format1 = reader.ReadWzInt(); // format 1
                    var format2 = reader.ReadByte(); // format 2
                    reader.BaseStream.Seek(4, SeekOrigin.Current);
                    var blockLen = (uint) reader.ReadInt32();
                    var header = reader.PeekFor(() => reader.ReadUInt16());
                    var imageHash = new byte[0];
                    using (Stream sub = new SubStream(reader.BaseStream, reader.BaseStream.Position, blockLen - 1))
                        imageHash = sha.ComputeHash(sub);

                    if (childHashes.Length > 0)
                        return CalcAggregateHash(sha, childHashes.Prepend(imageHash).ToArray());
                    return imageHash;
                default:
                    var nameHash = sha.ComputeHash(reader.GetWzStringBlockBytes(prop._encrypted | prop._container._encrypted));
                    var type = reader.ReadByte();
                    switch (type)
                    {
                        case 0: return nameHash;
                        case 0x10: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.ReadBytes(1)));
                        case 0x11: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.ReadBytes(1)));
                        case 0x0B:
                        case 2:
                        case 0x12: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.ReadBytes(2)));
                        case 3: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzIntBytes()));
                        case 19: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzIntBytes()));
                        case 4: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzSingleBytes()));
                        case 5: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.ReadBytes(8)));
                        case 8: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzStringBlockBytes(prop._container._encrypted)));
                        case 9: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.ReadBytes(reader.ReadInt32())));
                        case 20: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzLongBytes()));
                        case 21: return CalcAggregateHash(sha, nameHash, sha.ComputeHash(reader.GetWzLongBytes()));
                        default:
                            Debugger.Break();
                            Console.WriteLine("Wow");
                            if (type > 100)
                                throw new Exception("Unknown property type at ParsePropertyList");
                            return new byte[0];
                    }
            }
        }
    }

    public static IEnumerable<WzProperty> DirectoryChildren(WzProperty self)
    {
        using (var reader = GetContentReader(null, self))
        {
            reader.BaseStream.Seek(self._offset, SeekOrigin.Begin);
            var count = reader.ReadWzInt();
            var children = new WzProperty[count];
            string name = null;
            var encryption = self._encrypted | self._container._encrypted;
            long? restorePosition = null;
            for (var i = 0; i < count; ++i)
            {
                var type = reader.ReadByte();

                if (Array.IndexOf(ValidDirectoryChildrenTypes, type) == -1)
                    throw new InvalidOperationException("Invalid property type");

                switch (type)
                {
                    case 1:
                        reader.ReadBytes(10);
                        continue;
                    case 2:
                        var dedupedAt = reader.ReadInt32();

                        reader.PeekFor(() =>
                        {
                            reader.BaseStream.Position = reader._contentsStart + dedupedAt;
                            type = reader.ReadByte();
                            name = AttemptReadString(reader, ref encryption, i);

                            if (Array.IndexOf(ValidDirectoryChildrenTypes, type) == -1)
                            {
                                reader.BaseStream.Position = reader._contentsStart + dedupedAt + 1;
                                type = reader.ReadByte();
                                name = AttemptReadString(reader, ref encryption, i);
                            }
                        });

                        break;
                    case 3:
                    case 4:
                        name = AttemptReadString(reader, ref encryption, i);
                        break;
                    default:
                        throw new Exception("Unknown child type");
                }

                if (name == null) throw new InvalidOperationException("Found a property without a name, this shouldn't be possible.");

                var size = (uint) reader.ReadWzInt();
                var checksum = reader.ReadWzInt();
                var offset = reader.ReadWzOffset();
                var childProperty = new WzProperty(
                    name,
                    self != null ? PathCombine(self._path, name) : name,
                    reader._package,
                    type == 3 ? PropertyType.Directory : type == 4 ? PropertyType.Image : throw new InvalidOperationException($"Not sure what this is, but I don't handle it ({self._parent} -- {self._path}/{name} -- {type})"),
                    self,
                    size,
                    checksum,
                    offset
                );
                // These can be lazy loaded
                yield return Resolve(childProperty);
            }
        }
    }

    public static WzProperty Audio(WzReader reader, WzProperty self)
    {
        var unk = reader.ReadByte();
        var length = reader.ReadWzInt();
        var duration = reader.ReadWzInt();

        WzProperty result = new WzPropertyWeak<byte[]>(
            () =>
            {
                if (Package._logging != null) Package._logging.LogDebug($"{self._path} (Audio) - {unk}");
                using (reader = GetContentReader(null, self))
                {
                    reader.BaseStream.Seek(self._offset + 1, SeekOrigin.Begin);
                    if (length > sbyte.MaxValue || length <= sbyte.MinValue) reader.BaseStream.Seek(5, SeekOrigin.Current);
                    else reader.BaseStream.Seek(1, SeekOrigin.Current);
                    if (duration > sbyte.MaxValue || duration <= sbyte.MinValue) reader.BaseStream.Seek(5, SeekOrigin.Current);
                    else reader.BaseStream.Seek(1, SeekOrigin.Current);
                    return reader.ReadBytes(length);
                }
            }, self
        );

        result._size = (uint) length;
        result.Meta.Add("duration", duration);
        result.Meta.Add("unk", unk);

        return result;
    }

    public static IEnumerable<WzProperty> ConvexChildren(WzProperty self)
    {
        using (var reader = GetContentReader(null, self))
        {
            reader.BaseStream.Position = self._offset;
            var count = reader.ReadWzInt();
            for (var i = 0; i < count; ++i) yield return ParseExtendedProperty(reader, self, i.ToString(), 0, true);
        }
    }

    public static WzProperty Vector(WzReader reader, WzProperty self)
    {
        return new WzPropertyVal<Point>(new Point(reader.ReadWzInt(), reader.ReadWzInt()), self);
    }

    public static WzProperty Uol(WzReader reader, WzProperty self)
    {
        return new WzPropertyVal<string>(reader.ReadWzStringBlock(self._encrypted | self._container?._encrypted ?? EncryptionType.None), self);
    }

    public static WzProperty Resolve(WzProperty self)
    {
        // Determine lazy loading here
        using (var reader = GetContentReader(null, self))
        {
            reader.BaseStream.Seek(self._offset, SeekOrigin.Begin);
            return Resolve(self, reader);
        }
    }

    public static WzProperty Resolve(WzProperty self, WzReader reader)
    {
        switch (self.Type)
        {
            case PropertyType.Directory:
                return self;
            case PropertyType.Image:
                return Image(reader, self);
            case PropertyType.SubProperty:
                return self;
            case PropertyType.Convex:
                return self;
            case PropertyType.Vector2:
                return Vector(reader, self);
            case PropertyType.Audio:
                return Audio(reader, self);
            case PropertyType.Canvas:
                return Canvas(reader, self);
            case PropertyType.Uol:
                return Uol(reader, self);
            case PropertyType.File:
                return File(reader, self);
            default:
                return self;
        }
    }

    private static WzReader GetContentReader(Encoding encoding, WzProperty self)
    {
        if (self.RawData != null)
        {
            var underlying = new MemoryStream(self.RawData);
            if (self._fileContainer == null)
            {
                uint versionKey;
                var versionHash = WzSerializer.CalcVersionHash(1, out versionKey);
                return new WzReader(self._fileContainer, self, underlying, versionKey, versionHash, 0);
            }

            return new WzReader(self._fileContainer, self, underlying, self._fileContainer._versionKey, self._fileContainer._versionHash, 0);
        }

        if (self._container.RawData != null)
        {
            var underlying = new MemoryStream(self._container.RawData);

            if (self._fileContainer == null)
            {
                uint versionKey;
                var versionHash = WzSerializer.CalcVersionHash(1, out versionKey);
                return new WzReader(self._fileContainer, self._container, underlying, versionKey, versionHash, 0);
            }

            return new WzReader(self._fileContainer, self._container, underlying, self._fileContainer._versionKey, self._fileContainer._versionHash, 0);
        }

        return self._fileContainer.GetContentReader(encoding, self);
    }

    private static string PathCombine(params string[] paths)
    {
        return Path.Combine(paths).Replace('\\', '/');
    }

    private static byte[] CalcAggregateHash(SHA512 sha = null, params byte[][] hashes)
    {
        hashes = hashes.Where(c => c.Length != 0).ToArray();
        if (hashes.Length == 0) return new byte[0];
        var firstHash = hashes.First();
        var allHashes = new byte[firstHash.Length * (hashes.Length + 1)];
        Buffer.BlockCopy(firstHash, 0, allHashes, 0, firstHash.Length);
        var i = 0;
        foreach (var childHash in hashes) Buffer.BlockCopy(childHash, 0, allHashes, ++i * firstHash.Length, firstHash.Length);
        return sha.ComputeHash(allHashes);
    }

    private static WzProperty ParseExtendedProperty(WzReader reader, WzProperty parent, string name, uint length = uint.MinValue, bool maintainReader = false)
    {
        var type = reader.ReadWzStringBlock(parent?._encrypted | parent?._container?._encrypted ?? EncryptionType.None);
        PropertyType propType;
        switch (type)
        {
            case "Property":
                reader.BaseStream.Seek(2, SeekOrigin.Current);
                propType = PropertyType.SubProperty;
                break;
            case "Canvas":
                propType = PropertyType.Canvas;
                break;
            case "Shape2D#Vector2D":
                propType = PropertyType.Vector2;
                break;
            case "Shape2D#Convex2D":
                propType = PropertyType.Convex;
                break;
            case "Sound_DX8":
                propType = PropertyType.Audio;
                break;
            case "UOL":
                reader.BaseStream.Seek(1, SeekOrigin.Current);
                propType = PropertyType.Uol;
                break;
            case "RawData":
                if (length == uint.MinValue) throw new InvalidOperationException("RawData without a size isn't supported :(");
                propType = PropertyType.File;
                break;
            default:
                throw new InvalidOperationException($"Unknown ExtendedProperty type {type}");
        }

        var result = new WzProperty(
            name,
            PathCombine(parent._path, name),
            parent._fileContainer,
            propType,
            parent,
            length,
            0,
            (uint) reader.BaseStream.Position
        );

        if (maintainReader)
            return Resolve(result, reader);
        return Resolve(result);
    }

    private static string AttemptReadString(WzReader reader, ref EncryptionType encryption, int i)
    {
        string name;
        if (i == 0)
        {
            var position = reader.BaseStream.Position;
            name = reader.ReadWzString(false, encryption);

            if (Encoding.ASCII.GetBytes(name).Any(c => c < 32 || c >= 127))
            {
                encryption = EncryptionType.Gms;
                reader.BaseStream.Position = position;
                name = reader.ReadWzString(false, encryption);
            }

            if (Encoding.ASCII.GetBytes(name).Any(c => c < 32 || c >= 127))
            {
                encryption = EncryptionType.Kms;
                reader.BaseStream.Position = position;
                name = reader.ReadWzString(false, encryption);
            }

            if (Encoding.ASCII.GetBytes(name).Any(c => c < 32 || c >= 127))
            {
                encryption = EncryptionType.None;
                reader.BaseStream.Position = position;
                name = reader.ReadWzString(false, encryption);
            }
        }
        else
            name = reader.ReadWzString(false, encryption);

        return name;
    }

    private static WzProperty File(WzReader reader, WzProperty self)
    {
        return new WzPropertyVal<byte[]>(reader.ReadBytes((int) self._size), self);
    }

    private static WzProperty Canvas(WzReader reader, WzProperty self)
    {
        // Define the variables ahead of time that way we can come back to them
        int width = 0, height = 0, format1 = 0, format2 = 0;
        uint blockLen = 0, position = 0;

        // Define what well be doing once we come back
        WzProperty result = new WzPropertyWeak<Image<Rgba32>>(
            () =>
            {
                using (reader = GetContentReader(null, self))
                {
                    reader.BaseStream.Seek(position + 1, SeekOrigin.Begin);
                    var header = reader.PeekFor(() => reader.ReadUInt16());

                    var img = reader.ParsePng(
                        width,
                        height,
                        format1 + format2,
                        header != 0x9C78 && header != 0xDA78 && header != 0x178 && format1 + format2 != 6 && format1 + format2 != 7 && format1 + format2 != 8 && format1 + format2 != 5,
                        // Hackish fix for me not increasing blockLen by 1
                        format1 + format2 == 6 ? blockLen : blockLen - 1
                    );

                    if (self._container._alsoDispose != null)
                        self._container._alsoDispose.Add(img);

                    return img;
                }
            }, self
        );

        reader.BaseStream.Seek(1, SeekOrigin.Current);
        if (reader.ReadByte() == 1) // Has children
        {
            reader.BaseStream.Seek(2, SeekOrigin.Current);
            result.Children = PropertyList(reader, result).ToArray();
        }
        else
        {
            result.Children = new WzProperty[0];
        }

        width = reader.ReadWzInt(); // width
        height = reader.ReadWzInt(); // height
        format1 = reader.ReadWzInt(); // format 1
        format2 = reader.ReadByte(); // format 2
        reader.BaseStream.Seek(4, SeekOrigin.Current);
        blockLen = (uint) reader.ReadInt32();
        result._size = blockLen;
        position = (uint) reader.BaseStream.Position;

        return result;
    }
}