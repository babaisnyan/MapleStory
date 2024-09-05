using System;
using System.Buffers;
using System.IO;
using System.Threading.Tasks;
using Blake2Fast;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

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

    public static async Task<string> ComputeHashAsync(this Image<Rgba32> image)
    {
        var hash = Blake2b.CreateIncrementalHasher();
        var buffer = ArrayPool<byte>.Shared.Rent(1024 * 4);
        using var ms = new MemoryStream();
        await image.SaveAsPngAsync(ms).ConfigureAwait(false);
        ms.Seek(0, SeekOrigin.Begin);

        int bytesRead;
        while ((bytesRead = await ms.ReadAsync(buffer, 0, buffer.Length).ConfigureAwait(false)) > 0)
        {
            hash.Update(buffer.AsSpan(0, bytesRead));
        }

        ArrayPool<byte>.Shared.Return(buffer);
        return Convert.ToBase64String(hash.Finish());
    }

    public static async Task<string> ComputeHashAsync(this byte[] image)
    {
        var hash = Blake2b.CreateIncrementalHasher();
        var buffer = ArrayPool<byte>.Shared.Rent(1024 * 4);
        using var ms = new MemoryStream(image);
        ms.Seek(0, SeekOrigin.Begin);

        int bytesRead;
        while ((bytesRead = await ms.ReadAsync(buffer, 0, buffer.Length).ConfigureAwait(false)) > 0)
        {
            hash.Update(buffer.AsSpan(0, bytesRead));
        }

        ArrayPool<byte>.Shared.Return(buffer);
        return Convert.ToBase64String(hash.Finish());
    }

    public static Point Max(Point a, Point b)
    {
        return new Point(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y));
    }
}