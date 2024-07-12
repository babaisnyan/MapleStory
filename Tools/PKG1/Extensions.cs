using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.ImageSharp.Formats.Gif;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace PKG1;

public static class Extensions
{
    public static async Task<WzProperty> Resolve(this Task<WzProperty> original, string resolveFor = "")
    {
        var originalAwaited = await original;

        if (originalAwaited == null)
            return null;

        return await originalAwaited.Resolve(resolveFor);
    }

    public static async Task<TK> ResolveForOrNull<TK>(this Task<WzProperty> original, string resolveFor = "")
        where TK : class
    {
        var originalAwaited = await original;

        if (originalAwaited == null)
            return null;

        return await originalAwaited.ResolveForOrNull<TK>(resolveFor);
    }

    public static async Task<TK?> ResolveFor<TK>(this Task<WzProperty> original, string resolveFor = "")
        where TK : struct
    {
        var originalAwaited = await original;

        if (originalAwaited == null)
            return null;

        return await originalAwaited.ResolveFor<TK>(resolveFor);
    }

    public static async Task<Image<Rgba32>> GenerateGif(this WzProperty animationNode, Rgba32? background)
    {
        var maxFrame = animationNode.Children.Select(c => int.TryParse(c._name, out var frameNumber) ? frameNumber : -1).Max();

        var frames = await Task.WhenAll(Enumerable.Range(0, maxFrame + 1).Select(async frameNumber =>
        {
            var frameNode = await animationNode.Resolve(frameNumber.ToString());
            if (frameNode == null) return null;
            return new Tuple<Image<Rgba32>, Point, int>(
                await frameNode.ResolveForOrNull<Image<Rgba32>>(),
                await frameNode.ResolveFor<Point>("origin") ?? Point.Empty,
                await frameNode.ResolveFor<int>("delay") ?? 0
            );
        }).ToArray());

        frames = frames?.Where(c => c != null).ToArray();

        var maxWidth = frames.Max(x => x.Item1.Width);
        var maxHeight = frames.Max(x => x.Item1.Height);
        var maxOrigin = new Point(frames.Max(x => x.Item2.X), frames.Max(x => x.Item2.Y));
        var maxDifference = new Point(maxOrigin.X - frames.Min(x => x.Item2.X), maxOrigin.Y - frames.Min(x => x.Item2.Y));

        var pendingDispose = new List<Image<Rgba32>>();
        var gif = new Image<Rgba32>(maxWidth + maxDifference.X, maxHeight + maxDifference.Y);

        foreach (var (img, origin, delay) in frames)
        {
            using var clonedImg = img.Clone();
            var frameImage = clonedImg;
            var offset = new Point(maxOrigin.X - origin.X, maxOrigin.Y - origin.Y);
            if (offset.X != 0 || offset.Y != 0 || frameImage.Width < gif.Width || frameImage.Height < gif.Height)
            {
                var offsetFrameImage = new Image<Rgba32>(gif.Width, gif.Height);
                offsetFrameImage.Mutate(x => x.DrawImage(frameImage, offset, 1));
                frameImage.Dispose();
                frameImage = offsetFrameImage;
            }

            if (frameImage.Width > gif.Width || frameImage.Height > gif.Height) frameImage.Mutate(x => x.Crop(gif.Width, gif.Height));

            if (background?.A != 0)
            {
                var frameWithBackground = new Image<Rgba32>(frameImage.Width, frameImage.Height);
                var image = frameImage;
                frameWithBackground.Mutate(x =>
                {
                    if (background != null) x.Fill(background.Value);
                    x.DrawImage(image, Point.Empty, 1);
                });

                frameImage.Dispose();
                frameImage = frameWithBackground;
            }

            var resultFrame = gif.Frames.AddFrame(frameImage.Frames.RootFrame);
            var meta = resultFrame.Metadata.GetFormatMetadata(GifFormat.Instance);
            meta.FrameDelay = delay / 10;
            meta.DisposalMethod = GifDisposalMethod.RestoreToBackground;

            pendingDispose.Add(frameImage);
        }

        gif.Frames.RemoveFrame(0);
        var gifMeta = gif.Metadata.GetGifMetadata();
        gifMeta.RepeatCount = 0;

        pendingDispose.ForEach(c => c.Dispose());

        return gif;
    }
}