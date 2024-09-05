using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace PKG1;

public class VersionGuesser
{
    public static ILogger _logging;

    [JsonIgnore]
    public WzReader _r;

    public ushort _versionId;
    public uint _versionKey;
    public byte _versionHash;
    public EncryptionType _isEncrypted;
    public string _packagePath;
    public byte[] _rawData;

    public VersionGuesser() { }

    public VersionGuesser(WzReader reader, string path, ushort? hint = null, bool hasHash = true)
    {
        _r = reader;
        _packagePath = path;
        GuessVersion(hint, hasHash);
    }

    public VersionGuesser(WzReader reader, byte[] rawData, string path, ushort? hint = null)
    {
        _r = reader;
        _rawData = rawData;
        _packagePath = path;
        GuessVersion(hint);
    }

    private void GuessVersion(ushort? hint = null, bool hasHash = true)
    {
        _r.BaseStream.Seek(_r._contentsStart, SeekOrigin.Begin);

        short? ver = null;
        if (hasHash)
        {
            ver = _r.ReadInt16();
        }

        bool success;
        var oldPosition = _r.BaseStream.Position;
        var offset = TryFindImageInDir(out success);
        _logging?.LogDebug($"Found img at {offset} {success}");
        if (success)
        {
            success = GuessVersionWithImageOffsetAt(hint, ver, offset);
            _r.BaseStream.Seek(_r._contentsStart, SeekOrigin.Begin);
            if (success) return;
        }

        for (var v = hint ?? 0; v < ushort.MaxValue; v++)
        {
            var vHash = v.ToString()
                         .Aggregate<char, uint>(0, (current, t) => 32 * current + t + 1);
            if ((0xFF ^ (vHash >> 24) ^ ((vHash << 8) >> 24) ^ ((vHash << 16) >> 24) ^ ((vHash << 24) >> 24)) != ver) continue;
            _versionKey = vHash;
            _versionId = v;
            _r._versionKey = vHash;
            _r.BaseStream.Position = oldPosition;
            if (DepthFirstImageSearch(out offset)) break;
        }

        if (!GuessVersionWithImageOffsetAt(hint, ver, offset)) throw new KeyNotFoundException("Unable to guess WZ version.");
        _r.BaseStream.Seek(_r._contentsStart, SeekOrigin.Begin);
    }

    private bool DepthFirstImageSearch(out long offset)
    {
        var success = false;
        offset = -1;
        var count = _r.ReadWzInt();
        for (var i = 0; i < count; i++)
        {
            var type = _r.ReadByte();
            _logging?.LogDebug(type.ToString());
            switch (type)
            {
                case 1:
                    _r.BaseStream.Seek(10, SeekOrigin.Current);
                    continue;
                case 2:
                    var x = _r.ReadInt32();
                    type = _r.PeekFor(() =>
                    {
                        _r.BaseStream.Seek(x + _r._contentsStart, SeekOrigin.Begin);
                        return _r.ReadByte();
                    });
                    break;
                case 3:
                case 4:
                    _r.ReadWzString();
                    break;
                default:
                    throw new Exception("Unknown object type in WzDirectory.");
            }

            _r.ReadWzInt();
            _r.ReadWzInt();
            offset = _r.BaseStream.Position;
            if (type == 4)
            {
                success = true;
                break;
            }

            if (type == 3)
            {
                try
                {
                    offset = _r.PeekFor(() =>
                    {
                        _r.BaseStream.Seek(_r.ReadWzOffset(), SeekOrigin.Begin);
                        long o;
                        success = DepthFirstImageSearch(out o);
                        return o;
                    });
                    break;
                }
                catch { }
            }

            _r.BaseStream.Seek(4, SeekOrigin.Current);
        }

        return success;
    }

    private long TryFindImageInDir(out bool success)
    {
        var count = _r.ReadWzInt();
        _logging?.LogDebug($"Count: {count}");
        if (count == 0) throw new Exception("WZ file has no entries!");
        long offset = 0;
        offset = TryFindImageOffset(count, offset, out success);
        return offset;
    }

    private long TryFindImageOffset(int count, long offset, out bool success)
    {
        success = false;
        _logging?.LogDebug($"Trying to find img at {offset}");
        for (var i = 0; i < count; i++)
        {
            var type = _r.ReadByte();
            _logging?.LogDebug($"Child type {type}");
            switch (type)
            {
                case 1:
                    _r.BaseStream.Seek(10, SeekOrigin.Current);
                    continue;
                case 2:
                    var x = _r.ReadInt32();
                    type = _r.PeekFor(() =>
                    {
                        _r.BaseStream.Seek(x + _r._contentsStart, SeekOrigin.Begin);
                        return _r.ReadByte();
                    });
                    break;
                case 3:
                case 4:
                    _logging?.LogDebug(_r.ReadWzString());
                    break;
                default:
                    throw new Exception("Unknown object type in WzDirectory.");
            }

            _r.ReadWzInt();
            _r.ReadWzInt();
            offset = _r.BaseStream.Position;
            _r.BaseStream.Seek(4, SeekOrigin.Current);
            if (type != 4) continue;

            success = true;
            break;
        }

        return offset;
    }

    private bool GuessVersionWithImageOffsetAt(ushort? hint, short? ver, long offset)
    {
        var success = false;
        for (var v = hint ?? 0; v < ushort.MaxValue; v++)
        {
            var vHash = v.ToString()
                         .Aggregate<char, uint>(0, (current, t) => 32 * current + t + 1);

            if (ver.HasValue)
            {
                if ((0xFF ^ (vHash >> 24) ^ ((vHash << 8) >> 24) ^ ((vHash << 16) >> 24) ^ ((vHash << 24) >> 24)) != ver) continue;
            }

            _r._versionKey = vHash;
            _versionKey = vHash;
            _versionId = v;
            _r.BaseStream.Seek(offset, SeekOrigin.Begin);

            try
            {
                var currentOffset = _r.BaseStream.Position;
                _r.BaseStream.Seek(_r.ReadWzOffset(), SeekOrigin.Begin);
                var childType = _r.ReadByte();
                if (childType == 0x73)
                {
                    var oldPosition = _r.BaseStream.Position;
                    if (!_r.ReadWzStringExpecting(out _isEncrypted, "Property"))
                    {
                        _r.BaseStream.Position = oldPosition;
                        continue;
                    }

                    success = true;
                    _r.BaseStream.Position = oldPosition;
                    break;
                }

                if (childType == 0x01)
                {
                    // Lua script, impossible to guarantee found valid version
                    var luaScript = _r.ReadLuaScript();
                    if (luaScript.StartsWith("!version") || luaScript.StartsWith("!s3"))
                    {
                        var encTesting = EncryptionType.None;
                        var wrongEncryption = false;
                        do
                        {
                            var col = new PackageCollection();
                            using (var p = _rawData != null ? new Package(col, _rawData, _packagePath, v) : !string.IsNullOrEmpty(_packagePath) ? new Package(col, _packagePath, v) : throw new NullReferenceException())
                            {
                                p._mainDirectory._encrypted = encTesting;
                                var children = p._mainDirectory.Children;
                                WzProperty firstImg;
                                do
                                {
                                    firstImg = children.FirstOrDefault(c => c.Type == PropertyType.Image);
                                    wrongEncryption = children.Any(c => c.Name.Contains("?"));
                                    if (firstImg == null && !wrongEncryption) children = children.Where(c => c != null).SelectMany(c => c.Children ?? new WzProperty[0]).Where(c => c != null).ToArray();
                                } while (firstImg == null && children.Count() > 0 && !wrongEncryption);

                                if (firstImg != null)
                                {
                                    // No imgs???
                                    if (encTesting == EncryptionType.None)
                                        encTesting = _isEncrypted = firstImg._encrypted;
                                    else
                                        _isEncrypted = encTesting;

                                    success = true;
                                    break;
                                }

                                if (wrongEncryption) encTesting = (EncryptionType) ((int) encTesting + 1);
                                else break;
                                if ((int) encTesting == 3) break;
                            }
                        } while (wrongEncryption);

                        if (!wrongEncryption && !success)
                        {
                            _isEncrypted = encTesting;
                            success = true;
                        }

                        break;
                    }
                }
            }
            catch
            {
                success = false;
            }
        }

        return success;
    }
}