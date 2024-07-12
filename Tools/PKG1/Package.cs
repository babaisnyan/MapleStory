using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace PKG1;

public class Package : IDisposable
{
    public static ILogger _logging;

    [JsonIgnore]
    public PackageCollection _collection;

    public string _fileName;
    public string _filePath;
    public long _fileSize;
    public uint _contentsStartLocation;
    public string _fileDescription;
    public ushort _versionId;
    public byte? _versionHash;
    public uint _versionKey;
    public bool _versionMatches;
    public WzProperty _mainDirectory;
    public IDictionary<long, string> _deDupedStrings;
    private readonly StreamFactory _streamFactory;

    public Package() { }

    public Package(PackageCollection parent)
    {
        _collection = parent;
    }

    public Package(PackageCollection parent, byte[] data, string fileLocation, ushort version = ushort.MinValue, bool? hasVersionHash = null)
    {
        _collection = parent;
        _filePath = fileLocation;
        _fileName = Path.GetFileNameWithoutExtension(fileLocation).Replace(".rebuilt", "");
        _streamFactory = new StreamFactory(() =>
        {
            var res = new MemoryStream(data);
            return res;
        });
        hasVersionHash = hasVersionHash ?? true;

        Initialize(version, hasVersionHash);
    }

    public Package(PackageCollection parent, string fileLocation, ushort version = ushort.MinValue, bool? hasVersionHash = null, string overwriteName = null)
    {
        _collection = parent;
        _filePath = fileLocation;
        _fileName = overwriteName ?? Path.GetFileNameWithoutExtension(_filePath);
        _streamFactory = new StreamFactory(() => File.Open(fileLocation, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
        hasVersionHash = hasVersionHash ?? true;

        var dedupedPath = $"{Path.GetDirectoryName(_filePath)}/{Path.GetFileNameWithoutExtension(_filePath)}.deduped";
        if (File.Exists(dedupedPath))
            _deDupedStrings = JsonConvert.DeserializeObject<Dictionary<long, string>>(Encoding.Unicode.GetString(File.ReadAllBytes(dedupedPath)));
        else
            _deDupedStrings = new ConcurrentDictionary<long, string>();
        Initialize(version, hasVersionHash);
    }

    public Task<WzProperty> Resolve(string v)
    {
        return _mainDirectory.Resolve(v);
    }

    public bool UpdateVersion(ushort version)
    {
        if (_versionMatches = CheckVersionMatch(version, out _versionKey) || !_versionHash.HasValue) _versionId = version;
        var correctText = _versionMatches ? "correct" : "incorrect";
        if (_versionHash.HasValue && _logging != null) _logging.LogInformation($"{_fileName} - {version} is the {correctText} Version Id ( {_versionHash}, {_versionKey} )");
        if (_versionMatches || !_versionHash.HasValue)
            if (_logging != null)
                _logging.LogInformation($"{_fileName} has {_mainDirectory.Children.Count()} children");
        return _versionMatches;
    }

    public byte CalcVersionHash(ushort version, out uint versionKey)
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

    public WzReader GetRawReader(Encoding encoding = null, WzProperty container = null)
    {
        return new WzReader(this, container ?? _mainDirectory, GetRawStream(), encoding ?? Encoding.ASCII, _versionKey, _versionHash, _contentsStartLocation);
    }

    public WzReader GetContentReader(Encoding encoding = null, WzProperty container = null)
    {
        return new WzReader(this, container ?? _mainDirectory, GetContentStream(container), encoding ?? Encoding.ASCII, _versionKey, _versionHash, _contentsStartLocation);
    }

    public Stream GetRawStream()
    {
        return _streamFactory.GetStream();
    }

    public Stream GetContentStream(WzProperty container = null)
    {
        var str = GetRawStream();
        str.Seek((container ?? _mainDirectory)._containerStartLocation, SeekOrigin.Begin);
        return str;
    }

    public void Dispose()
    {
        _streamFactory.Dispose();
    }

    private void Initialize(ushort version, bool? hasVersionHash)
    {
        using (var file = GetRawReader())
        {
            if (!file.ReadString(4).Equals("PKG1", StringComparison.CurrentCultureIgnoreCase)) throw new InvalidOperationException("Can not run on non-PKG1 files.");
            _fileSize = file.ReadInt64();
            _contentsStartLocation = file.ReadUInt32();
            _fileDescription = file.ReadNulTerminatedString(100);

            if (_logging != null)
                _logging.LogDebug($"Loaded package {_fileName} ({_fileSize}, {_contentsStartLocation}) with Description: {_fileDescription}");

            _mainDirectory = new WzProperty(Path.GetFileNameWithoutExtension(_fileName), _fileName, this, PropertyType.Directory, null, (uint) _fileSize, -1, _contentsStartLocation + (uint) (hasVersionHash.Value ? 2 : 0));
            file.BaseStream.Seek(_contentsStartLocation, SeekOrigin.Begin);

            if (hasVersionHash.Value)
                _versionHash = (byte) file.ReadUInt16();

            var hashValue = hasVersionHash.Value ? _versionHash.ToString() : "No version hash";
            if (_logging != null)
                _logging.LogDebug($"{_fileName} - Version Hash: {hashValue}");

            try
            {
                if (!UpdateVersion(version) && _versionHash.HasValue)
                    if (_logging != null)
                        _logging.LogWarning("Warning: Version is not right, expect corrupted / malformed data.");
                    else if (_logging != null)
                        _logging.LogDebug($"{_fileName} has {_mainDirectory.Children.Count()} children");
            }
            catch (Exception ex)
            {
                _logging.LogWarning("Exception thrown when trying to initialize package, likely very wrong version. {0} ({1}) @ {2}", ex.Message, ex.GetType().FullName, ex.StackTrace);
            }
        }
    }

    private bool CheckVersionMatch(ushort version, out uint versionKey)
    {
        return _versionHash == CalcVersionHash(version, out versionKey);
    }
}