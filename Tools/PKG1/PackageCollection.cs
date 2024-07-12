using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace PKG1;

public class PackageCollection
{
    public static ILogger _logging;

    public static bool _alwaysGuessVersion;
    public string _folder;
    public string _baseFilePath;
    public ConcurrentDictionary<string, object> _versionCache;
    public Func<WzProperty, IEnumerable<WzProperty>, IEnumerable<WzProperty>> _childrenMutate;
    internal Region _wzRegion;

    public PackageCollection()
    {
        _versionCache = new ConcurrentDictionary<string, object>();
        Packages = new Dictionary<string, Package>();
    }

    public PackageCollection(string baseFilePath, ushort? versionId = null)
    {
        var watchGlobal = Stopwatch.StartNew();
        if (_logging != null) _logging.LogDebug($"Took {watchGlobal.ElapsedMilliseconds}ms to initialize resolver");
        _folder = Path.GetDirectoryName(baseFilePath);
        _versionCache = new ConcurrentDictionary<string, object>();

        _wzRegion = Region.Gms;
        _baseFilePath = baseFilePath;
        BasePackage = new Package(this, baseFilePath, versionId ?? ushort.MinValue);
        var hasHash = true;
        if (!BasePackage._versionMatches || versionId == null || _alwaysGuessVersion)
        {
            var watch = Stopwatch.StartNew();
            try
            {
                using var reader = BasePackage.GetContentReader();
                var guesser = new VersionGuesser(reader, baseFilePath);
                watch.Stop();
                if (_logging != null) _logging.LogDebug($"Guessed version in {watch.ElapsedMilliseconds}ms");
                watch.Restart();
                BasePackage._mainDirectory._encrypted = guesser._isEncrypted;
                BasePackage.UpdateVersion(guesser._versionId);
                // Force any existing children to get flushed from cache
                BasePackage._mainDirectory.Children = null;
                watch.Stop();
                if (_logging != null) _logging.LogDebug($"Done initializing in {watch.ElapsedMilliseconds}ms");
            }
            catch (Exception)
            {
                var valid = false;
                ushort previousLastVersionId = 0;
                ushort lastVersionId = 0;
                BasePackage = new Package(this, baseFilePath, versionId ?? ushort.MinValue, false);
                while (!valid)
                {
                    if (previousLastVersionId > lastVersionId) throw new ArithmeticException("Ran out of possible IDs");
                    previousLastVersionId = lastVersionId;
                    try
                    {
                        using var reader = BasePackage.GetContentReader();
                        var guesser = new VersionGuesser(reader, baseFilePath, lastVersionId, false);
                        watch.Stop();
                        if (_logging != null) _logging.LogDebug($"Guessed version in {watch.ElapsedMilliseconds}ms");
                        lastVersionId = guesser._versionId;
                        watch.Restart();
                        BasePackage._mainDirectory._encrypted = guesser._isEncrypted;
                        BasePackage.UpdateVersion(guesser._versionId);
                        // Force any existing children to get flushed from cache
                        BasePackage._mainDirectory.Children = null;
                        BasePackage._mainDirectory.Children = BasePackage._mainDirectory.Children.ToArray();
                        watch.Stop();
                        if (_logging != null) _logging.LogDebug($"Done initializing in {watch.ElapsedMilliseconds}ms");
                        hasHash = false;

                        var smap = BasePackage._mainDirectory.Children.Single(child => child._name == "smap.img");
                        var zmap = BasePackage._mainDirectory.Children.Single(child => child._name == "zmap.img");
                        var smapChildrenCount = smap.Children.Count();
                        var zmapChildrenCount = zmap.Children.Count();
                        var difference = zmapChildrenCount - smapChildrenCount;

                        // We expect ZMap to have more children than SMap as zmap describes all layers while smap describes most
                        if (smapChildrenCount > 100 && zmapChildrenCount > 100 && difference > 0)
                        {
                            if (_logging != null) _logging.LogDebug("Brute force version appears valid {0}", lastVersionId);
                            valid = true;
                            versionId = lastVersionId;
                        }
                        else
                        {
                            if (_logging != null) _logging.LogDebug($"Error: Invalid count found / wrong difference, trying another ID (id: {lastVersionId}, smap: {smapChildrenCount}, zmap: {zmapChildrenCount}, diff: {difference})");
                            lastVersionId++;
                        }
                    }
                    catch (Exception ex)
                    {
                        if (_logging != null) _logging.LogDebug($"Failed to guess version for {_baseFilePath}");
                        lastVersionId++;
                    }
                }
            }
        }

        Packages = BasePackage._mainDirectory.Children
                              .Where(c => c.Type == PropertyType.Directory || c.Type == PropertyType.Image || c.Type == PropertyType.Lua)
                              .Select(c =>
                              {
                                  Package res = null;

                                  if ((c.Type == PropertyType.Directory || c.Type == PropertyType.Lua) && c._size < 50)
                                  {
                                      // I don't know the exact size off hand, I'm assuming it's less than 50 bytes.
                                      if (File.Exists(Path.Combine(_folder, $"{c._nameWithoutExtension}.wz")))
                                          res = new Package(this, Path.Combine(_folder, $"{c._nameWithoutExtension}.wz"), BasePackage._versionId, hasHash);
                                      else
                                          res = new Package(this) // Create a "ghost" package where the MainDirectory is just the Img
                                          {
                                              _mainDirectory = c
                                          };
                                  }
                                  else
                                      res = new Package(this) // Create a "ghost" package where the MainDirectory is just the Img
                                      {
                                          _mainDirectory = c
                                      };

                                  res._mainDirectory._encrypted = BasePackage._mainDirectory._encrypted;
                                  // Flush children cache to ensure encryption bubbles down
                                  res._mainDirectory.Children = null;

                                  return res;
                              })
                              .ToDictionary(c => c._fileName ?? c._mainDirectory._nameWithoutExtension, c => c);

        foreach (var file in Directory.GetFiles(_folder).Where(file => file.EndsWith(".wz")))
        {
            var fileName = Path.GetFileNameWithoutExtension(file);

            if (Packages.ContainsKey(fileName)) continue;

            try
            {
                var res = new Package(this, file, BasePackage._versionId, hasHash);
                res._mainDirectory._encrypted = BasePackage._mainDirectory._encrypted;
                Packages.Add(fileName, res);
            }
            catch (Exception ex)
            {
                if (_logging != null) _logging.LogDebug($"Error loading non-specified file: {fileName} ([{ex.GetType().Name}]{ex.Message})");
            }
        }

        if (!Packages.ContainsKey("Map002"))
        {
            var map002Path = Path.Combine(_folder, "Map002.wz");
            if (File.Exists(map002Path))
            {
                var res = new Package(this, map002Path, BasePackage._versionId, hasHash);
                res._mainDirectory._encrypted = BasePackage._mainDirectory._encrypted;
                Packages.Add("Map002", res);
            }
        }

        var characterPackage = Packages["Character"];
        var shoes = characterPackage.Resolve("Shoes").GetAwaiter().GetResult();
        var idImgMatch = new Regex("[0-9]{8}\\.img");
        WzProperty invalidShoe;
        if ((invalidShoe = shoes.Children.FirstOrDefault(c => !idImgMatch.IsMatch(c._name))) != null)
        {
            _logging.LogError("Invalid name found in shoes");
            throw new InvalidDataException("Invalid name found in shoes");
        }

        watchGlobal.Stop();

        if (_logging != null) _logging.LogDebug($"Took {watchGlobal.ElapsedMilliseconds}ms total");
    }

    public PackageCollection(string dataFolderPath)
        : this()
    {
        BuildNewPackageCollection(dataFolderPath);
    }

    [JsonIgnore]
    public virtual Package BasePackage { get; protected set; }

    public virtual IDictionary<string, Package> Packages { get; protected set; }

    public Task<WzProperty> Resolve(string path)
    {
        var forwardSlashPosition = path.IndexOf('/');
        var backSlashPosition = path.IndexOf('\\', 0, forwardSlashPosition == -1 ? path.Length : forwardSlashPosition);
        int firstSlash;
        if (forwardSlashPosition == -1) firstSlash = backSlashPosition;
        else if (backSlashPosition == -1) firstSlash = forwardSlashPosition;
        else firstSlash = Math.Min(forwardSlashPosition, backSlashPosition);

        if (firstSlash == -1) firstSlash = path.Length;
        var wzName = path.Substring(0, firstSlash);

        if (Packages.ContainsKey(wzName)) return Packages[wzName].Resolve(path.Substring(Math.Min(firstSlash + 1, path.Length)));

        return null;
    }

    public void Dispose()
    {
        foreach (var p in Packages.Values)
            p.Dispose();
    }

    private void BuildNewPackageCollection(string path)
    {
        ushort versionId = 0;
        var hasVersionHash = true;

        var expectedBaseFilePath = Path.Combine(path, "Base/Base_000.wz");
        var valid = false;

        ushort? lastVersionId = ushort.MinValue;
        ushort? previousLastVersionId = 0;
        while (!valid)
        {
            if (previousLastVersionId.HasValue && lastVersionId.HasValue && previousLastVersionId > lastVersionId) throw new ArithmeticException("Ran out of possible IDs");
            previousLastVersionId = lastVersionId;
            Package testPackage = null;

            if (hasVersionHash)
            {
                try
                {
                    testPackage = new Package(this, expectedBaseFilePath);
                    using (var reader = testPackage.GetContentReader())
                    {
                        // We expect to find a version hash in the file here
                        var guesser = new VersionGuesser(reader, expectedBaseFilePath, lastVersionId);
                        versionId = guesser._versionId;
                    }

                    lastVersionId = versionId;
                    hasVersionHash = true;
                }
                catch (KeyNotFoundException versionNotFound)
                {
                    // We don't expect a exception here as it's just version guessing
                    hasVersionHash = false;
                    lastVersionId = 0;
                    continue;
                }
            }
            else
            {
                if (_logging != null) _logging.LogDebug("Failed to find version ID via hash, trying brute force guessing...");

                var testBasePackage = new Package(this, expectedBaseFilePath, 0, false);
                using (var reader = testBasePackage.GetContentReader())
                {
                    var guesser = new VersionGuesser(reader, expectedBaseFilePath, lastVersionId, false);
                    lastVersionId = guesser._versionId;
                    if (_logging != null) _logging.LogDebug($"Guessed ID: {lastVersionId}, checking validity...");
                }

                testPackage = new Package(this, expectedBaseFilePath, lastVersionId.Value, false);
                hasVersionHash = false;
            }

            try
            {
                // Reload with the proper version
                testPackage = new Package(this, expectedBaseFilePath, lastVersionId.Value, hasVersionHash);

                var smap = testPackage._mainDirectory.Children.Single(child => child._name == "smap.img");
                var zmap = testPackage._mainDirectory.Children.Single(child => child._name == "zmap.img");
                var smapChildrenCount = smap.Children.Count();
                var zmapChildrenCount = zmap.Children.Count();
                var difference = zmapChildrenCount - smapChildrenCount;

                // We expect ZMap to have more children than SMap as zmap describes all layers while smap describes most
                if (smapChildrenCount > 100 && zmapChildrenCount > 100 && difference > 0)
                {
                    if (_logging != null) _logging.LogDebug("Brute force version appears valid {0}", lastVersionId);
                    valid = true;
                    versionId = lastVersionId.Value;
                }
                else
                {
                    if (_logging != null) _logging.LogDebug($"Error: Invalid count found / wrong difference, trying another ID (id: {lastVersionId}, smap: {smapChildrenCount}, zmap: {zmapChildrenCount}, diff: {difference})");
                    throw new InvalidDataException("Wrong version ID");
                }
            }
            catch (Exception ex)
            {
                // Move forward
                lastVersionId++;
            }

            try
            {
                var directories = Directory.GetDirectories(path);
                foreach (var directory in directories)
                {
                    var subDirectoryName = Path.GetFileName(directory);
                    var builtPackage = WalkDirectory(path, null, subDirectoryName, versionId, hasVersionHash);
                    if (builtPackage != null)
                        Packages.Add(subDirectoryName, builtPackage);
                }
            }
            catch (Exception ex)
            {
                // Something happened this late???
                _logging.LogError("Something went wrong with fully loading the data, is this a hash collision that also collided the pointers? Seems extremely unlikely.{1}{0}{1}{2} @ {1}{3}", ex.GetType().FullName, Environment.NewLine, ex.Message, ex.StackTrace);
                lastVersionId++;
                throw;
            }

            BasePackage = Packages.Where(p => p.Key.EndsWith("Base")).Select(kvp => kvp.Value).FirstOrDefault();
        }

        if (!valid)
            throw new InvalidDataException("Couldn't determine version, no result found :(");
    }

    private Package WalkDirectory(string basePath, string parentPath, string directoryName = null, ushort versionId = 0, bool hasVersionHash = true)
    {
        var currentPath = Path.Combine(basePath, directoryName);
        if (parentPath != null)
            currentPath = Path.Combine(basePath, parentPath, directoryName);

        //string iniFile = Path.Combine(currentPath, $"{directoryName}.ini");

        var directories = Directory.GetDirectories(currentPath).Select(directory =>
        {
            var subDirectoryName = Path.GetFileName(directory);
            var childsParentPath = directoryName;
            if (parentPath != null) childsParentPath = Path.Combine(parentPath, directoryName);
            return WalkDirectory(basePath, childsParentPath, subDirectoryName, versionId, hasVersionHash);
        }).ToArray();

        //if (!File.Exists(iniFile))
        //    return null;

        //string[] iniFileContents = File.ReadAllLines(iniFile);
        //int lastWz = 0;

        //foreach (string iniFileLine in iniFileContents)
        //{
        //    if (iniFileLine.StartsWith("LastWzIndex", StringComparison.OrdinalIgnoreCase))
        //    {
        //        string count = iniFileLine.Substring(12);
        //        int.TryParse(count, out lastWz);
        //    }
        //}

        var packageName = directoryName;
        if (parentPath != null)
            packageName = Path.Combine(parentPath, directoryName).Replace("\\", "/");

        var wzPath = Path.Combine(currentPath, $"{directoryName}.wz");
        if (!File.Exists(wzPath)) return null;

        var mainDirectory = new Package(this, wzPath, versionId, hasVersionHash, packageName);
        var properties = wzPath.Contains("_0") ? mainDirectory._mainDirectory.Children.ToList() : [];

        foreach (var subPackages in directories)
        {
            properties.Add(subPackages._mainDirectory);
            subPackages._mainDirectory._parent = mainDirectory._mainDirectory;
        }

        var packages = Directory.GetFiles(currentPath, $"{directoryName}_*.wz");
        foreach (var package in packages)
        {
            var childrenCollection = new Package(this, package, versionId, hasVersionHash, packageName);
            properties.AddRange(childrenCollection._mainDirectory.Children);
        }

        foreach (var child in properties)
            child._parent = mainDirectory._mainDirectory;

        mainDirectory._mainDirectory.Children = properties;
        return mainDirectory;
    }
}