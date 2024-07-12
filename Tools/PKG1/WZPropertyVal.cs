namespace PKG1;

public class WzPropertyVal<TK> : WzProperty, IWzPropertyVal<TK>
{
    public WzPropertyVal(TK val, WzProperty original)
        : this(val, original._name, original._path, original._fileContainer, original.Type, original._parent, original._size, original._checksum, original._offset)
    {
        _encrypted = original._encrypted;
    }

    public WzPropertyVal(TK val, string name, string path, Package container, PropertyType type, WzProperty parent, uint size, int checksum, uint offset)
        : base(name, path, container, type, parent, size, checksum, offset)
    {
        Value = val;
    }

    public TK Value { get; set; }

    public static implicit operator TK(WzPropertyVal<TK> prop)
    {
        return prop.Value;
    }

    public object GetValue()
    {
        return Value;
    }
}