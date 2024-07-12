using System;

namespace PKG1;

public class WzPropertyWeak<TK> : WzProperty, IWzPropertyVal<TK>
    where TK : class
{
    private readonly Lazy<TK> _weakValue;

    public WzPropertyWeak(Func<TK> val, WzProperty original)
        : this(val, original._name, original._path, original._fileContainer, original.Type, original._parent, original._size, original._checksum, original._offset)
    {
        Children = original.Children;
        _encrypted = original._encrypted;
    }

    public WzPropertyWeak(Func<TK> val, string name, string path, Package container, PropertyType type, WzProperty parent, uint size, int checksum, uint offset) : base(name, path, container, type, parent, size, checksum, offset)
    {
        _weakValue = new Lazy<TK>(val, true);
    }

    public TK Value
    {
        get => _weakValue.Value;
        set => throw new InvalidOperationException("WeakReference WZProperties can not have strong reference values set.");
    }

    public static implicit operator TK(WzPropertyWeak<TK> prop)
    {
        return prop.Value;
    }

    public object GetValue()
    {
        return Value;
    }
}