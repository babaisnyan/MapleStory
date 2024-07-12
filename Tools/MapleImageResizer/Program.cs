using PKG1;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace MapleImageResizer
{
    internal class Program
    {
        private const string MAPLE_STORY_PATH = @"C:\Nexon\Maple\Data";

        private static async Task Main(string[] args)
        {
            if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
            Directory.CreateDirectory("./Exported");

            var path = Console.ReadLine() ?? "";
            var split = path.Split('/');
            if (split.Length < 2) return;

            Console.WriteLine("Loading Package Collection");
            var collection = new PackageCollection(MAPLE_STORY_PATH);
            Console.WriteLine("Package Collection Loaded");

            var package = await collection.Resolve(split[0]).ConfigureAwait(false);
            var canvas = package.Children.Where(x => x._name == "_Canvas")
                                .First(x => x.Children.Any());

            Console.WriteLine("Loading Children");
            package.LoadChildren();
            Console.WriteLine("Children Loaded");

            var targetPath = string.Join("/", split.Skip(1));
            var resolved = await package.Resolve(targetPath).ConfigureAwait(false);

            if (resolved == null)
            {
                Console.WriteLine("Could not resolve path");
                return;
            }

            if (resolved.Type == PropertyType.Canvas)
            {
                Console.WriteLine("Can't export single canvas. Exporting parent instead.");
                resolved = resolved._parent;

                if (resolved == null)
                {
                    Console.WriteLine("Could not resolve parent");
                    return;
                }
            }

            Console.Write("Export All? Y/N: ");
            var response = Console.ReadKey().Key;
            if (response != ConsoleKey.Y) return;

            Console.WriteLine($"\nExporting {targetPath}");
            await ExportAsync(resolved, canvas, new DirectoryInfo("./Exported"), split[0]).ConfigureAwait(false);
            Console.WriteLine("Exported");
        }

        private static async Task ExportAsync(WzProperty property, WzProperty canvas, DirectoryInfo currentDirectory, string wz)
        {
            if (property.Children == null) return;
            if (property.Type == PropertyType.Canvas) return;
            if (property._name == "_Canvas") return;

            var currentPath = Path.Combine(currentDirectory.FullName, property._name);
            property.LoadChildren();
            Directory.CreateDirectory(currentPath);

            var canvasList = property.Children.Where(x => x.Type == PropertyType.Canvas);
            var propertyList = property.Children.Where(x => x.Type == PropertyType.Directory || x.Type == PropertyType.Image);

            var maxFrame = canvasList.Select(x => int.TryParse(x._name, out var frameNumber) ? frameNumber : -1).Max();
            var frames = await Task.WhenAll(Enumerable.Range(0, maxFrame + 1).Select(async frameNumber =>
            {
                var node = await property.Resolve(frameNumber.ToString()).ConfigureAwait(false);
                if (node == null) return null;

                var originPoint = await node.ResolveFor<Point>("origin").ConfigureAwait(false);
                if (!originPoint.HasValue) return null;

                if (node.Children.Any(x => x._name == "_outlink"))
                {
                    var outlink = await node.ResolveForOrNull<string>("_outlink").ConfigureAwait(false);

                    if (outlink != null)
                    {
                        outlink = outlink.Replace($"{wz}/_Canvas/", "");

                        var realCanvas = await canvas.Resolve(outlink).ConfigureAwait(false);
                        node = realCanvas;
                    }
                }

                if (node == null) return null;

                var result = new Tuple<Image<Rgba32>, Point>(await node.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false), originPoint.Value);
                return result;
            }).ToArray()).ConfigureAwait(false);

            if (frames.Length == 0) return;
            if (frames.Any(x => x == null)) return;

            var maxWidth = frames.Max(x => x.Item1.Width);
            var maxHeight = frames.Max(x => x.Item1.Height);
            var maxOrigin = new Point(frames.Max(x => x.Item2.X), frames.Max(x => x.Item2.Y));
            var maxDiff = new Point(maxOrigin.X - frames.Min(x => x.Item2.X), maxOrigin.Y - frames.Min(x => x.Item2.Y));
            var disposeList = new List<Image<Rgba32>>();
            var saveTasks = new List<Task>();
            var result = new Image<Rgba32>(maxWidth + maxDiff.X, maxHeight + maxDiff.Y);
            var frameIndex = 0;

            foreach (var (image, (x, y)) in frames)
            {
                using var clone = image.Clone();
                var frameImage = clone;
                var offset = new Point(maxOrigin.X - x, maxOrigin.Y - y);

                if (offset.X != 0 || offset.Y != 0 || (frameImage.Width < result.Width || frameImage.Height < result.Height))
                {
                    var offsetImage = new Image<Rgba32>(result.Width, result.Height);
                    offsetImage.Mutate(x => x.DrawImage(frameImage, new Point(offset.X, offset.Y), 1));
                    frameImage.Dispose();
                    frameImage = offsetImage;
                }

                if (frameImage.Width > result.Width || frameImage.Height > result.Height)
                {
                    frameImage.Mutate(x => x.Crop(result.Width, result.Height));
                }

                var task = frameImage.SaveAsPngAsync(Path.Combine(currentPath, $"{frameIndex++}.png"));
                saveTasks.Add(task);
                disposeList.Add(frameImage);
            }

            Task.WaitAll(saveTasks.ToArray());
            saveTasks.Clear();
            disposeList.ForEach(x => x.Dispose());

            foreach (var wzProperty in propertyList)
            {
                await ExportAsync(wzProperty, canvas, new DirectoryInfo(Path.Combine(currentDirectory.FullName, property._name)), wz).ConfigureAwait(false);
            }
        }
    }
}