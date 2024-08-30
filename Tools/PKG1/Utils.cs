using System.Threading.Tasks;

namespace PKG1;

public static class Utils
{
    public static async Task<T> GetAsync<T>(this WzProperty property, string name, T defaultValue) where T : struct
    {
        var resolved = await property.ResolveFor<T>(name).ConfigureAwait(false);
        return resolved ?? defaultValue;
    }

    public static async Task<bool> HasChildAsync(this WzProperty property, string name)
    {
        var node = await property.Resolve(name).ConfigureAwait(false);
        return node != null;
    }
}