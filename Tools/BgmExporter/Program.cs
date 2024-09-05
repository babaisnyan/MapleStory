using System.Xml.Linq;
using PKG1;

namespace BgmExporter
{
    internal class Program
    {
        private const string BASE_PATH = @"C:\Nexon\Maple\Data";

        private static async Task Main(string[] args)
        {
            if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
            Directory.CreateDirectory("./Exported");

            Console.WriteLine("Loading Package Collection");
            var collection = new PackageCollection(BASE_PATH);
            Console.WriteLine("Package Collection Loaded");

            var sound = await collection.Resolve("Sound").ConfigureAwait(false);
            sound.LoadChildren();

            foreach (var img in sound.Children)
            {
                if (!img.Name.Contains("Bgm")) continue;
                if (img.Name.Contains("UI")) continue;

                await SaveSoundAsync(img).ConfigureAwait(false);
            }
        }

        private static async Task SaveSoundAsync(WzProperty soundProperty, string? directoryName = null)
        {
            if (soundProperty.Children.Any())
            {
                if (directoryName == null)
                {
                    directoryName = $"./Exported/{soundProperty.Name.Replace(".img", "")}";
                    Directory.CreateDirectory(directoryName);
                }

                foreach (var child in soundProperty.Children)
                {
                    if (child is not WzPropertyWeak<byte[]>)
                    {
                        Directory.CreateDirectory($"{directoryName}/{child.Name}");
                        await SaveSoundAsync(child, $"{directoryName}/{child.Name}").ConfigureAwait(false);
                    }
                    else
                    {
                        await SaveSoundAsync(child, directoryName).ConfigureAwait(false);
                    }
                }
            }
            else
            {
                if (soundProperty is IWzPropertyVal<byte[]> {Value: not null} nodeData)
                {
                    var name = soundProperty.Name.Replace(":", "_").Replace(".mp3", "").Replace(" ", "_");
                    await File.WriteAllBytesAsync($"{directoryName}/{name}.mp3", nodeData.Value).ConfigureAwait(false);
                }
            }
        }
    }
}