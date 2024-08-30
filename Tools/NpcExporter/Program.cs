using System.Text;
using Newtonsoft.Json;
using PKG1;

namespace NpcExporter
{
    internal static class Program
    {
        private const string MAPLE_STORY_PATH = @"C:\Nexon\Maple\Data";

        private static IDictionary<int, NpcInfo> _npcInfos = null!;

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

            await using var writer = new StreamWriter("Npc.json");
            await writer.WriteAsync(JsonConvert.SerializeObject(_npcInfos.Values.OrderBy(x => x.Id), Formatting.Indented)).ConfigureAwait(false);
        }

        private static async Task<IDictionary<int, NpcInfo>> GetNpcInfosAsync(WzProperty collection)
        {
            var npcs = new Dictionary<int, NpcInfo>();
            var directory = await collection.Resolve("Npc.img").ConfigureAwait(false);

            foreach (var npc in directory.Children)
            {
                var id = int.Parse(npc._name);
                var name = await npc.ResolveForOrNull<string>("name").ConfigureAwait(false);

                if (id >= 3000000 || string.IsNullOrWhiteSpace(name) || npcs.ContainsKey(id))
                {
                    continue;
                }

                var info = new NpcInfo(id, name);

                foreach (var node in npc.Children)
                {
                    if (node.Type != PropertyType.String) continue;
                    if (node._name.Length > 2) continue;
                    if (info.Texts.ContainsKey(node._name)) continue;

                    var text = await node.ResolveForOrNull<string>().ConfigureAwait(false);
                    if (string.IsNullOrWhiteSpace(text)) continue;

                    info.Texts.Add(node._name, text);
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
                    await ExtractImagesAsync(npc).ConfigureAwait(false);
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

                await ExtractImagesAsync(npc).ConfigureAwait(false);
            }

            foreach (var id in removeList)
            {
                _npcInfos.Remove(id);
            }
        }

        private static async Task ExtractImagesAsync(WzProperty npc)
        {
            foreach (var action in npc.Children)
            {
                if (action.Type != PropertyType.Directory) continue;
                if (await action.HasChildAsync("special").ConfigureAwait(false)) continue;
                if (action._name == "info" || action._name == "image" || action._name.Contains("condition") || action._name.Contains("illust") || action._name.Contains("special")) continue;
            }
        }
    }
}