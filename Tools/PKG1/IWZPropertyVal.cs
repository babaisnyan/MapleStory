using System.Collections.Generic;

namespace PKG1;

public interface IWzPropertyVal<TK> : IWzPropertyVal
{
    TK Value { get; set; }
}

public interface IWzPropertyVal
{
    PropertyType Type { get; }
    IEnumerable<WzProperty> Children { get; }
    object GetValue();
}