using System.Globalization;
using CsvHelper;
using Newtonsoft.Json;
using PKG1;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace NpcExporter
{
    internal static class Program
    {
        private const string MAPLE_STORY_PATH = @"C:\Nexon\Maple\Data";

        private static IDictionary<int, NpcInfo> _npcInfos = null!;
        private static readonly Dictionary<string, string> ImageHash = [];

        private static async Task Main(string[] args)
        {
            if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
            Directory.CreateDirectory("./Exported");

            Console.WriteLine("Loading Package Collection");
            var collection = new PackageCollection(MAPLE_STORY_PATH);
            Console.WriteLine("Package Collection Loaded");

            var stringCollection = await collection.Resolve("String").ConfigureAwait(false);
            var npcCollection = await collection.Resolve("Npc").ConfigureAwait(false);

            _npcInfos = await GetNpcInfosAsync(stringCollection).ConfigureAwait(false);
            await FillNpcInfoAsync(npcCollection).ConfigureAwait(false);

            var index = 1;

            foreach (var (_, npcInfo) in _npcInfos)
            {
                npcInfo.Name = index++.ToString();
            }

            await using var writer = new StreamWriter("Npc.json");
            await writer.WriteAsync(JsonConvert.SerializeObject(_npcInfos.Values.OrderBy(x => x.Id), Formatting.Indented)).ConfigureAwait(false);
        }

        private static async Task<IDictionary<int, NpcInfo>> GetNpcInfosAsync(WzProperty collection)
        {
            var npcs = new Dictionary<int, NpcInfo>();
            var directory = await collection.Resolve("Npc.img").ConfigureAwait(false);

            foreach (var npc in directory.Children)
            {
                var id = int.Parse(npc.Name);
                var name = await npc.ResolveForOrNull<string>("name").ConfigureAwait(false);

                if (id >= 4000000 || string.IsNullOrWhiteSpace(name) || npcs.ContainsKey(id))
                {
                    continue;
                }

                var info = new NpcInfo(id, name);

                foreach (var node in npc.Children)
                {
                    if (node.Type != PropertyType.String) continue;
                    if (node.Name.Length > 2) continue;
                    if (info.Texts.ContainsKey(node.Name)) continue;

                    var text = await node.ResolveForOrNull<string>().ConfigureAwait(false);
                    if (string.IsNullOrWhiteSpace(text)) continue;

                    info.Texts.Add(node.Name, text);
                }

                npcs.Add(id, info);
            }

            return npcs;
        }

        private static async Task FillNpcInfoAsync(WzProperty collection)
        {
            var removeList = new List<int>();
            var excludes = new[] {"link", "default", "reg", "trunkPut", "illustration2", "forceMove", "questIconPreStart", "quarterViewYOffset", "zeroComponentNPC", "float", "speed", "illustration", "name", "0", "publicReward", "explosiveReward", "rareItemDropLevel", "defaultHP", "defaultMP", "attack", "skill", "firstAttack", "hpTagColor", "hpTagBgcolor", "charismaEXP", "level", "bodyAttack", "maxHP", "maxMP", "PADamage", "PDDamage", "PDRate", "MADamage", "MDDamage", "MDRate", "acc", "eva", "pushed", "fs", "boss", "exp", "summonType", "category", "hpRecovery", "mpRecovery", "mobType",};

            foreach (var (id, npcInfo) in _npcInfos)
            {
                var npc = await collection.Resolve($"{id:D7}.img").ConfigureAwait(false);

                if (npc == null)
                {
                    removeList.Add(id);
                    continue;
                }

                var info = await npc.Resolve("info").ConfigureAwait(false);

                if (info == null)
                {
                    await ExtractImagesAsync(npc, npcInfo).ConfigureAwait(false);
                    continue;
                }

                var stop = false;

                foreach (var exclude in excludes)
                {
                    if (await info.HasChildAsync(exclude).ConfigureAwait(false))
                    {
                        stop = true;
                        break;
                    }
                }

                if (stop)
                {
                    removeList.Add(id);
                    continue;
                }

                var hide = await info.GetAsync("hideName", 0).ConfigureAwait(false) > 0;
                var hide2 = await info.GetAsync("hide", 0).ConfigureAwait(false) > 0;
                var component = await info.GetAsync("componentNPC", 0).ConfigureAwait(false) > 0;
                var mouseOnly = await info.GetAsync("talkMouseOnly", 0).ConfigureAwait(false) > 0;

                if (hide || hide2 || mouseOnly || component)
                {
                    removeList.Add(id);
                    continue;
                }

                npcInfo.IsShop = await info.GetAsync("shop", 0).ConfigureAwait(false) > 0;

                var scripts = await GetScriptsAsync(info).ConfigureAwait(false);

                if (scripts.Count > 0)
                {
                    npcInfo.Actions.Add("stand", new("stand", scripts));
                }

                await ExtractImagesAsync(npc, npcInfo).ConfigureAwait(false);
            }

            foreach (var id in removeList)
            {
                _npcInfos.Remove(id);
            }

            Console.WriteLine($"Exported {_npcInfos.Count} Items.");
        }

        private static async Task<List<string>> GetScriptsAsync(WzProperty node)
        {
            var list = new List<string>();

            if (await node.HasChildAsync("speak").ConfigureAwait(false))
            {
                var speak = await node.Resolve("speak").ConfigureAwait(false);
                var scripts = speak.Children.Where(x => x.Type == PropertyType.String)
                                   .OrderBy(x => x.Name)
                                   .ToList();

                foreach (var script in scripts)
                {
                    var text = await script.ResolveForOrNull<string>().ConfigureAwait(false);
                    if (string.IsNullOrWhiteSpace(text)) continue;
                    list.Add(text);
                }
            }

            return list;
        }

        private static async Task ExtractImagesAsync(WzProperty npc, NpcInfo info)
        {
            foreach (var action in npc.Children)
            {
                var actionName = action.Name.Trim();

                if (action.Type != PropertyType.SubProperty) continue;
                if (await action.HasChildAsync("special").ConfigureAwait(false)) continue;
                if (actionName == "info" || actionName == "image" || actionName.Contains("condition") || actionName.Contains("illust") || actionName.Contains("special")) continue;

                var scripts = await GetScriptsAsync(action).ConfigureAwait(false);

                if (scripts.Count > 0 && actionName != "stand")
                {
                    info.Actions.Add(actionName, new(actionName, scripts));
                }

                var frameInfo = new List<FrameInfo>();
                var time = 0;

                foreach (var frameNode in action.Children)
                {
                    if (!int.TryParse(frameNode.Name, out var frame)) continue;

                    var image = await frameNode.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false);
                    if (image == null) break;

                    var origin = await frameNode.GetAsync("origin", new Point()).ConfigureAwait(false);
                    var delay = await frameNode.GetAsync("delay", 100).ConfigureAwait(false);
                    var z = await frameNode.GetAsync("z", 0).ConfigureAwait(false);
                    var a0 = await frameNode.GetAsync("a0", 255).ConfigureAwait(false);
                    var a1 = await frameNode.GetAsync("a1", -1).ConfigureAwait(false);
                    var path = $"\"/Paper2D.PaperSprite'/Game/Npc/{info.Id}/{actionName}/S_{frame}.S_{frame}'\"";
                    var hash = await image.ComputeHashAsync().ConfigureAwait(false);
                    var found = false;

                    if (ImageHash.TryGetValue(hash, out var value))
                    {
                        path = value;
                        found = true;
                    }
                    else
                    {
                        ImageHash.Add(hash, path);
                    }

                    frameInfo.Add(new FrameInfo(path)
                    {
                        Delay = delay,
                        OffsetX = origin.X,
                        OffsetY = origin.Y,
                        ZOrder = z,
                        ZigZag = false,
                        HasAlpha = a1 != -1,
                        AlphaStart = a0,
                        AlphaEnd = a1 == -1 ? 0 : a1
                    });

                    time += delay;

                    Directory.CreateDirectory($"./Exported/{info.Id}/{actionName}");

                    if (!found)
                    {
                        await image.SaveAsPngAsync($"./Exported/{info.Id}/{actionName}/T_{frame}.png").ConfigureAwait(false);
                    }
                }

                if (info.Actions.TryGetValue(actionName, out var infoAction))
                {
                    infoAction.Length = time;
                }

                var index = 1;

                foreach (var frame in frameInfo)
                {
                    frame.Name = index++;
                }

                await using (var write = new StreamWriter($"./Exported/{info.Id}/{actionName}/DT_Frames.csv"))
                {
                    await using var csv = new CsvWriter(write, CultureInfo.InvariantCulture);
                    await csv.WriteRecordsAsync(frameInfo.OrderBy(x => x.Name)).ConfigureAwait(false);
                }

                var content = await File.ReadAllTextAsync($"./Exported/{info.Id}/{actionName}/DT_Frames.csv").ConfigureAwait(false);
                var changed = content.Replace("\"\"\"", "\"");
                await File.WriteAllTextAsync($"./Exported/{info.Id}/{actionName}/DT_Frames.csv", changed).ConfigureAwait(false);
            }
        }
    }
}