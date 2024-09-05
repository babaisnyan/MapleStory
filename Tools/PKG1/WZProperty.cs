using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;

namespace PKG1;

public class WzProperty : IDisposable
{
    private static readonly Dictionary<string, Func<WzProperty, string, WzProperty>> _specialUol = new();
    private static readonly Func<WzProperty, string, Task<WzProperty>> _resolveOutlinkHook = (p, v) => p._fileContainer._collection.Resolve(v);

    [JsonIgnore]
    public Package _fileContainer;

    [JsonIgnore]
    public WzProperty _container;

    [JsonIgnore]
    public WzProperty _parent;

    internal List<IDisposable> _alsoDispose;

    internal uint _size;
    internal int _checksum;
    internal uint _offset;
    internal EncryptionType _encrypted;
    internal uint _containerStartLocation;
    internal long? _pseudoContentStart;
    internal Lazy<IEnumerable<WzProperty>> _weakChildren;

    private ConcurrentDictionary<string, string> _referenceTraces;

    private IDictionary<string, object> _meta;

    private IEnumerable<WzProperty> _children;
    private int _disposed;

    public WzProperty()
    {
        _children = Array.Empty<WzProperty>();
    }

    public WzProperty(string name, string path, Package container, PropertyType type, WzProperty parent, uint size, int checksum, uint offset)
    {
        Name = name;
        NameWithoutExtension = System.IO.Path.GetFileNameWithoutExtension(name);
        Path = path;
        _fileContainer = container;
        _parent = parent;
        if (_parent != null)
        {
            _containerStartLocation = _parent._containerStartLocation;
            if (_parent.Type == PropertyType.Image)
                _container = _parent;
            else
                _container = _parent._container;
        }
        else
        {
            _containerStartLocation = _fileContainer._contentsStartLocation;
            // Assume we are the MainDirectory
            _container = this;
        }

        Type = type;
        _size = size;
        _checksum = checksum;
        _offset = offset;
    }

    public string Name { get; }

    public string NameWithoutExtension { get; }

    public string Path { get; }

    public PropertyType Type { get; set; }

    [JsonIgnore]
    public byte[] RawData
    {
        get => FetchRawData?.Invoke();
        set => FetchRawData = () => value;
    }

    public Func<byte[]> FetchRawData { get; set; }

    [JsonIgnore]
    public WzProperty ResolvesTo { get; set; }

    public IEnumerable<WzProperty> Children
    {
        get
        {
            var res = _children;
            if (res == null)
            {
                if (_weakChildren == null) _weakChildren = new Lazy<IEnumerable<WzProperty>>(LoadChildren, true);
                res = _weakChildren.Value;
            }

            if (_fileContainer?._collection?._childrenMutate != null)
            {
                return _fileContainer._collection._childrenMutate(this, res);
            }

            return res;
        }
        set
        {
            _children = value;

            if (_weakChildren != null)
                _weakChildren = null;
        }
    }

    public IDictionary<string, object> Meta
    {
        get
        {
            if (_meta == null) return _meta = new ConcurrentDictionary<string, object>();
            return _meta;
        }
        set => _meta = value;
    }

    public bool HasDefinedMeta => _meta?.Count > 0;

    public bool Disposing { get; private set; }

    internal static TK? GetValueAs<TK>(WzProperty prop)
        where TK : struct
    {
        if (prop == null) return null;

        var propType = prop.GetType();
        if (propType.Equals(typeof(WzProperty))) return null;

        // If we're asking for the same type as it already is, return as is
        if (typeof(WzPropertyVal<TK>).Equals(propType)) return (WzPropertyVal<TK>) prop;

        var convertTo = typeof(TK);
        var convertFrom = propType.GenericTypeArguments[0];

        var propVal = (IWzPropertyVal) prop;
        var currentValue = propVal.GetValue();

        if (convertFrom == convertTo) return (TK) currentValue;

        if (currentValue is string && convertTo != typeof(string) && convertFrom != convertTo)
        {
            if (decimal.TryParse(currentValue.ToString(), out var newValue))
                currentValue = newValue;
        }

        if (currentValue is IConvertible)
            try
            {
                return (TK) Convert.ChangeType(
                    currentValue,
                    convertTo
                );
            }
            catch (Exception) { }

        return null;
    }

    internal static TK GetValueAsOrNull<TK>(WzProperty prop)
        where TK : class
    {
        if (prop == null) return null;

        var propType = prop.GetType();
        if (propType.Equals(typeof(WzProperty))) return null;

        // If we're asking for the same type as it already is, return as is
        if (typeof(WzPropertyVal<TK>).Equals(propType)) return (WzPropertyVal<TK>) prop;

        var convertTo = typeof(TK);
        var convertFrom = propType.GenericTypeArguments[0];

        var propVal = (IWzPropertyVal) prop;
        var currentValue = propVal.GetValue();

        if (convertFrom == convertTo) return (TK) currentValue;

        if (currentValue is string && convertTo != typeof(string) && convertFrom != convertTo)
        {
            if (decimal.TryParse(currentValue.ToString(), out var newValue))
                currentValue = newValue;
        }

        if (currentValue is IConvertible)
            try
            {
                return (TK) Convert.ChangeType(
                    currentValue,
                    convertTo
                );
            }
            catch (Exception) { }

        return null;
    }

    internal static IWzPropertyVal<TK> AsType<TK>(WzProperty prop)
    {
        if (prop == null) return null;

        var propType = prop.GetType();
        if (propType.Equals(typeof(WzProperty))) return null;

        // If we're asking for the same type as it already is, return as is
        if (typeof(WzPropertyVal<TK>).Equals(propType)) return (WzPropertyVal<TK>) prop;
        if (typeof(TK).IsClass)
        {
            var weakType = typeof(WzPropertyWeak<>).MakeGenericType(typeof(TK));
            if (weakType.Equals(propType)) return (IWzPropertyVal<TK>) prop;
        }

        var convertTo = typeof(TK);
        var convertFrom = propType.GenericTypeArguments[0];

        var propVal = (IWzPropertyVal) prop;
        var currentValue = propVal.GetValue();

        if (convertFrom == convertTo) return new WzPropertyVal<TK>((TK) currentValue, prop);

        if (currentValue is string && convertTo != typeof(string) && convertFrom != convertTo)
        {
            if (decimal.TryParse(currentValue.ToString(), out var newValue))
                currentValue = newValue;
        }

        if (currentValue is IConvertible)
            try
            {
                return new WzPropertyVal<TK>((TK) Convert.ChangeType(
                    currentValue,
                    convertTo
                ), prop);
            }
            catch (Exception) { }

        return null;
    }

    public virtual IEnumerable<WzProperty> GetCachedChildren()
    {
        return _children;
    }

    public virtual IEnumerable<WzProperty> GetChildren()
    {
        return _children ?? LoadChildren();
    }

    public override string ToString()
    {
        return $"{Path} @ {_offset}x{_size}";
    }

    public IEnumerable<WzProperty> LoadChildren()
    {
        IEnumerable<WzProperty> childrenEnumerable = null;
        switch (Type)
        {
            case PropertyType.Image:
                childrenEnumerable = PropertyResolvers.ImageChildren(this);
                break;
            case PropertyType.Directory:
                childrenEnumerable = PropertyResolvers.DirectoryChildren(this);
                break;
            case PropertyType.SubProperty:
                childrenEnumerable = PropertyResolvers.SubPropChildren(this);
                break;
            case PropertyType.Convex:
                childrenEnumerable = PropertyResolvers.ConvexChildren(this);
                break;
            default:
                childrenEnumerable = new WzProperty[0];
                break;
        }

        if (childrenEnumerable != null)
        {
            if (NameWithoutExtension != "zmap" && NameWithoutExtension == "smap")
                childrenEnumerable = childrenEnumerable.OrderBy(c => c.Name);
            childrenEnumerable = childrenEnumerable.ToArray();
        }

        if (_fileContainer?._collection?._childrenMutate != null) return _fileContainer._collection._childrenMutate(this, childrenEnumerable);
        return childrenEnumerable;
    }

    public async Task<WzProperty> Resolve(string v = null)
    {
        if (string.IsNullOrEmpty(v) && Type == PropertyType.Uol)
        {
            var self = (IWzPropertyVal<string>) this;
            var current = this;
            var paths = new List<string>();
            while ((current = await _parent.Resolve(self.Value))?.Type == PropertyType.Uol)
            {
                if (paths.Contains(current.Path)) return current;
                paths.Add(current.Path);
            }

            return current;
        }

        if (Type == PropertyType.Lua)
        {
            if (ResolvesTo != null)
                try
                {
                    return await ResolvesTo.Resolve(v);
                }
                catch (NullReferenceException) { }

            var value = ((IWzPropertyVal<string>) this).Value;
            if (value != null && value.StartsWith("!"))
            {
                var specialType = value.Substring(1, value.IndexOf(':') - 1);
                if (_specialUol.ContainsKey(specialType))
                {
                    if (string.IsNullOrEmpty(v)) return _specialUol[specialType](this, value[(specialType.Length + 2)..]);
                    return await _specialUol[specialType](this, value[(specialType.Length + 2)..]).Resolve(v);
                }

                throw new InvalidOperationException($"Unable to follow Special UOL, as there is no defined route, Type: {specialType} UOL:{value}");
            }
        }

        if (string.IsNullOrEmpty(v)) return this;

        var forwardSlashPosition = v.IndexOf('/');
        var backSlashPosition = v.IndexOf('\\', 0, forwardSlashPosition == -1 ? v.Length : forwardSlashPosition);
        var firstSlash = -1;

        if (forwardSlashPosition == -1) firstSlash = backSlashPosition;
        else if (backSlashPosition == -1) firstSlash = forwardSlashPosition;
        else firstSlash = Math.Min(forwardSlashPosition, backSlashPosition);

        if (firstSlash == -1) firstSlash = v.Length;

        var childName = v[..firstSlash].Replace(".img", "");

        // Dots = children of parent
        if (childName == ".." || childName == ".")
            return _parent == null ? null : await _parent.Resolve(v[Math.Min(firstSlash + 1, v.Length)..]);

        if (Children == null)
            if (Package._logging != null)
                Package._logging.LogDebug($"Children null for {Path} ({Type}) ({ResolvesTo}) ({RawData?.Length})");

        var childMatch = Children.FirstOrDefault(c => c.NameWithoutExtension.Equals(childName, StringComparison.CurrentCultureIgnoreCase));
        if (childMatch != null)
        {
            if (childMatch.Type == PropertyType.Lua) childMatch = await childMatch.Resolve();
            return await childMatch.Resolve(v[Math.Min(firstSlash + 1, v.Length)..]);
        }

        childMatch = Children.FirstOrDefault(c => c.Name.Equals(childName, StringComparison.CurrentCultureIgnoreCase));
        if (childMatch != null)
        {
            if (childMatch.Type == PropertyType.Lua) childMatch = await childMatch.Resolve();
            return await childMatch.Resolve(v[Math.Min(firstSlash + 1, v.Length)..]);
        }

        return childMatch;
    }

    public Task<WzProperty> ResolveInlink(string v = null)
    {
        return _container.Resolve(v);
    }

    public Task<WzProperty> ResolveOutlink(string v = null)
    {
        return _resolveOutlinkHook(this, v);
    }

    public async Task<TK?> ResolveFor<TK>(string v = null)
        where TK : struct
    {
        return GetValueAs<TK>(await Resolve(v));
    }

    public async Task<TK> ResolveForOrNull<TK>(string v = null)
        where TK : class
    {
        var ret = AsType<TK>(await Resolve(v));

        if (ret == null) return null;
        // Only follow inlinks and outlinks for canvas elements. Ignore others as we don't know what the side effects would be.
        if (ret?.Type == PropertyType.Canvas)
        {
            var link = ret.Children.FirstOrDefault(c => c.NameWithoutExtension.Equals("_inlink", StringComparison.CurrentCultureIgnoreCase)) ?? ret.Children.FirstOrDefault(c => c.NameWithoutExtension.Equals("_outlink", StringComparison.CurrentCultureIgnoreCase)) ?? ret.Children.FirstOrDefault(c => c.NameWithoutExtension.Equals("source", StringComparison.CurrentCultureIgnoreCase));
            if (link != null)
            {
                var linksTo = GetValueAsOrNull<string>(link);
                if (linksTo.StartsWith("Map"))
                {
                    if (link.Name.Equals("_inlink"))
                        return await ResolveInlinkForOrNull<TK>(linksTo) ?? await ResolveInlinkForOrNull<TK>("Map2" + linksTo[3..]) ?? await ResolveInlinkForOrNull<TK>("Map001" + linksTo[3..]);
                    if (link.Name.Equals("_outlink"))
                        return await ResolveOutlinkForOrNull<TK>(linksTo) ?? await ResolveOutlinkForOrNull<TK>("Map2" + linksTo[3..]) ?? await ResolveOutlinkForOrNull<TK>("Map001" + linksTo[3..]);
                    if (link.Name.Equals("source"))
                        return await ResolveOutlinkForOrNull<TK>(linksTo) ?? await ResolveOutlinkForOrNull<TK>("Map2" + linksTo[3..]) ?? await ResolveOutlinkForOrNull<TK>("Map001" + linksTo[3..]);
                }
                else if (linksTo.StartsWith("Mob"))
                {
                    if (link.Name.Equals("_inlink"))
                        return await ResolveInlinkForOrNull<TK>(linksTo) ?? await ResolveInlinkForOrNull<TK>("Mob2" + linksTo[3..]);
                    if (link.Name.Equals("_outlink"))
                        return await ResolveOutlinkForOrNull<TK>(linksTo) ?? await ResolveOutlinkForOrNull<TK>("Mob2" + linksTo[3..]);
                    if (link.Name.Equals("source"))
                        return await ResolveOutlinkForOrNull<TK>(linksTo) ?? await ResolveOutlinkForOrNull<TK>("Mob2" + linksTo[3..]);
                }
                else
                {
                    if (link.Name.Equals("_inlink"))
                        return await ResolveInlinkForOrNull<TK>(await link.ResolveForOrNull<string>());
                    if (link.Name.Equals("_outlink"))
                        return await ResolveOutlinkForOrNull<TK>(await link.ResolveForOrNull<string>());
                    if (link.Name.Equals("source"))
                        return await ResolveOutlinkForOrNull<TK>(await link.ResolveForOrNull<string>());
                }
            }
        }

        return ret.Value;
    }

    public async Task<TK> ResolveInlinkForOrNull<TK>(string v = null)
        where TK : class
    {
        return GetValueAsOrNull<TK>(await ResolveInlink(v));
    }

    public async Task<TK> ResolveOutlinkForOrNull<TK>(string v = null)
        where TK : class
    {
        return GetValueAsOrNull<TK>(await ResolveOutlink(v));
    }

    public bool RegisterReference(Action<Action> callbackRegister, string trace)
    {
        if (Disposing) return false;

        _referenceTraces.TryAdd(trace, trace);
        callbackRegister(() => UnregisterReference(trace));

        return !Disposing;
    }

    public void Dispose()
    {
        Disposing = true;
        if (_alsoDispose != null && _referenceTraces.Count == 0)
        {
            if (Interlocked.CompareExchange(ref _disposed, 1, 0) == 0)
            {
                if (PackageCollection._logging != null) PackageCollection._logging.LogDebug($"Disposing of property node ({Path})");
                var toDispose = _alsoDispose.ToArray();
                _alsoDispose = null;
                foreach (var alsoDispose in toDispose)
                    alsoDispose.Dispose();
            }
        }
        else if (_alsoDispose != null)
            if (PackageCollection._logging != null)
                PackageCollection._logging.LogDebug($"Property is locked, will dispose later ({Path})");
    }

    private void UnregisterReference(string trace)
    {
        _referenceTraces.Remove(trace, out trace);

        if (_referenceTraces.Count == 0 && Disposing)
        {
            if (PackageCollection._logging != null) PackageCollection._logging.LogDebug($"Disposing of previously disposed property node ({Path})");

            Dispose();
        }
    }
}