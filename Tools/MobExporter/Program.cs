using System.Globalization;
using CsvHelper;
using Newtonsoft.Json;
using PKG1;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace MobExporter
{
    internal static class Program
    {
        private const string MAPLE_STORY_PATH = @"D:\Maple\Data";

        private static IDictionary<int, MobInfo> _mobInfos = null!;

        private static async Task Main(string[] args)
        {
            if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
            Directory.CreateDirectory("./Exported");

            Console.WriteLine("Loading Package Collection");
            var collection = new PackageCollection(MAPLE_STORY_PATH);
            Console.WriteLine("Package Collection Loaded");

            var stringCollection = await collection.Resolve("String").ConfigureAwait(false);
            var mobCollection = await collection.Resolve("Mob").ConfigureAwait(false);

            _mobInfos = await GetMobInfosAsync(stringCollection).ConfigureAwait(false);

            await FillMobInfoAsync(mobCollection).ConfigureAwait(false);

            var index = 1;

            foreach (var kvp in _mobInfos)
            {
                kvp.Value.Name = index++;
            }

            await using var write = new StreamWriter("Mobs.csv");
            await using var csv = new CsvWriter(write, CultureInfo.InvariantCulture);
            await csv.WriteRecordsAsync(_mobInfos.Values.OrderBy(x => x.Name)).ConfigureAwait(false);

            await using var writer = new StreamWriter("Mobs.json");
            await writer.WriteAsync(JsonConvert.SerializeObject(_mobInfos.Values.OrderBy(x => x.MobId), Formatting.Indented)).ConfigureAwait(false);
        }

        private static async Task<IDictionary<int, MobInfo>> GetMobInfosAsync(WzProperty collection)
        {
            var mobs = new Dictionary<int, MobInfo>();
            var directory = await collection.Resolve("Mob.img").ConfigureAwait(false);

            foreach (var mob in directory.Children)
            {
                var id = int.Parse(mob._name);
                var name = await mob.ResolveForOrNull<string>("name").ConfigureAwait(false);

                if (id < 3000000 && !string.IsNullOrWhiteSpace(name) && !mobs.ContainsKey(id))
                {
                    mobs.Add(id, new MobInfo(id, name));
                }
            }

            return mobs;
        }

        private static async Task FillMobInfoAsync(WzProperty collection)
        {
            var removeList = new List<int>();
            var actions = new[] {"move", "stand", "hit1", "die1", "regen", "attack1"};

            foreach (var (id, mobInfo) in _mobInfos)
            {
                var mob = await collection.Resolve($"{id:D7}.img").ConfigureAwait(false);

                if (mob == null)
                {
                    removeList.Add(id);
                    continue;
                }

                var info = await mob.Resolve("info").ConfigureAwait(false);

                if (info == null)
                {
                    removeList.Add(id);
                    continue;
                }

                if (await mob.HasChildAsync("jump").ConfigureAwait(false))
                {
                    removeList.Add(id);
                    continue;
                }

                if (await mob.HasChildAsync("fly").ConfigureAwait(false))
                {
                    removeList.Add(id);
                    continue;
                }

                var boss = await info.GetAsync("boss", 0).ConfigureAwait(false) > 0;
                var link = await info.GetAsync("link", 0).ConfigureAwait(false) > 0;
                var fly = await info.GetAsync("flySpeed", 0).ConfigureAwait(false) > 0;
                var skeleton = await info.GetAsync("skeleton", 0).ConfigureAwait(false) > 0;
                var changeable = await info.GetAsync("changeableMob", 0).ConfigureAwait(false) > 0;

                if (link || boss || fly || skeleton || changeable)
                {
                    removeList.Add(id);
                    continue;
                }

                mobInfo.Level = await info.GetAsync("level", 0).ConfigureAwait(false);
                mobInfo.MaxHp = await info.GetAsync("maxHP", 0).ConfigureAwait(false);
                mobInfo.BodyAttack = await info.GetAsync("bodyAttack", 0).ConfigureAwait(false) > 0;
                mobInfo.FirstAttack = await info.GetAsync("firstAttack", 0).ConfigureAwait(false) > 0;
                mobInfo.Speed = await info.GetAsync("speed", 0).ConfigureAwait(false);
                mobInfo.PaDamage = await info.GetAsync("PADamage", 0).ConfigureAwait(false);
                mobInfo.MaDamage = await info.GetAsync("MADamage", 0).ConfigureAwait(false);
                mobInfo.PdDamage = await info.GetAsync("PDDamage", 0).ConfigureAwait(false);
                mobInfo.MdDamage = await info.GetAsync("MDDamage", 0).ConfigureAwait(false);
                mobInfo.PdRate = await info.GetAsync("PDRate", 0).ConfigureAwait(false);
                mobInfo.MdRate = await info.GetAsync("MDRate", 0).ConfigureAwait(false);
                mobInfo.Exp = await info.GetAsync("exp", 0).ConfigureAwait(false);

                foreach (var action in actions)
                {
                    var node = await mob.Resolve(action).ConfigureAwait(false);
                    if (node == null) continue;

                    var frameInfo = new List<FrameInfo>();
                    var zigzag = await node.GetAsync("zigzag", 0).ConfigureAwait(false) > 0;
                    var time = 0;

                    foreach (var frameNode in node.Children)
                    {
                        if (!int.TryParse(frameNode._name, out var frame)) continue;

                        var image = await frameNode.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false);
                        if (image == null) break;

                        var origin = await frameNode.GetAsync("origin", new Point()).ConfigureAwait(false);
                        var delay = await frameNode.GetAsync("delay", 100).ConfigureAwait(false);
                        var z = await frameNode.GetAsync("z", 0).ConfigureAwait(false);
                        var a0 = await frameNode.GetAsync("a0", 255).ConfigureAwait(false);
                        var a1 = await frameNode.GetAsync("a1", -1).ConfigureAwait(false);

                        frameInfo.Add(new FrameInfo($"\"/Paper2D.PaperSprite'/Game/Mob/{id}/{action}/S_{frame}.S_{frame}'\"")
                        {
                            Delay = delay,
                            OffsetX = origin.X,
                            OffsetY = origin.Y,
                            ZOrder = z,
                            ZigZag = zigzag,
                            HasAlpha = a1 != -1,
                            AlphaStart = a0,
                            AlphaEnd = a1 == -1 ? 0 : a1
                        });

                        time += delay;

                        Directory.CreateDirectory($"./Exported/{id}/{action}");
                        await image.SaveAsPngAsync($"./Exported/{id}/{action}/T_{frame}.png").ConfigureAwait(false);
                    }

                    if (frameInfo.Count > 0)
                    {
                        switch (action)
                        {
                            case "move":
                                mobInfo.HasMove = true;
                                break;
                            case "stand":
                                mobInfo.HasStand = true;
                                break;
                            case "hit1":
                                mobInfo.HasHit = true;
                                mobInfo.HitLength = time;
                                break;
                            case "die1":
                                mobInfo.HasDie = true;
                                mobInfo.DieLength = time;
                                break;
                            case "regen":
                                mobInfo.HasRegen = true;
                                break;
                            case "attack1":
                                mobInfo.HasAttack = true;
                                mobInfo.AttackLength = time;
                                break;
                        }
                    }

                    var index = 1;

                    foreach (var frame in frameInfo)
                    {
                        frame.Name = index++;
                    }

                    await using (var write = new StreamWriter($"./Exported/{id}/{action}/DT_Frames.csv"))
                    {
                        await using var csv = new CsvWriter(write, CultureInfo.InvariantCulture);
                        await csv.WriteRecordsAsync(frameInfo.OrderBy(x => x.Name)).ConfigureAwait(false);
                    }

                    var content = await File.ReadAllTextAsync($"./Exported/{id}/{action}/DT_Frames.csv").ConfigureAwait(false);
                    var changed = content.Replace("\"\"\"", "\"");
                    await File.WriteAllTextAsync($"./Exported/{id}/{action}/DT_Frames.csv", changed).ConfigureAwait(false);

                    if (action == "attack1" && frameInfo.Count > 0)
                    {
                        var attackInfo = await node.Resolve("info").ConfigureAwait(false);

                        if (attackInfo == null)
                        {
                            Directory.Delete($"./Exported/{id}/{action}", true);
                            mobInfo.HasAttack = false;
                            continue;
                        }

                        var attackAfter = await attackInfo.GetAsync("attackAfter", 0).ConfigureAwait(false);
                        var range = await attackInfo.Resolve("range").ConfigureAwait(false);

                        if (range == null || attackAfter == 0 || !await attackInfo.HasChildAsync("range").ConfigureAwait(false))
                        {
                            Directory.Delete($"./Exported/{id}/{action}", true);
                            mobInfo.HasAttack = false;
                            continue;
                        }

                        var lt = await range.GetAsync("lt", new Point(1557, 1557)).ConfigureAwait(false);
                        var rb = await range.GetAsync("rb", new Point(1557, 1557)).ConfigureAwait(false);

                        if (lt.X == 1557 || rb.X == 1557)
                        {
                            Directory.Delete($"./Exported/{id}/{action}", true);
                            mobInfo.HasAttack = false;
                            continue;
                        }

                        var hit = await attackInfo.Resolve("hit").ConfigureAwait(false);

                        if (hit != null)
                        {
                            var hitFrameInfo = new List<FrameInfo>();
                            mobInfo.AttackCool = attackAfter;
                            mobInfo.AttackWidth = Math.Abs(lt.X - rb.X);
                            mobInfo.AttackHeight = Math.Abs(lt.Y - rb.Y);

                            foreach (var hitNode in hit.Children)
                            {
                                if (!int.TryParse(hitNode._name, out var frame)) continue;

                                var image = await hitNode.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false);
                                if (image == null) break;

                                var origin = await hitNode.GetAsync("origin", new Point()).ConfigureAwait(false);
                                var delay = await hitNode.GetAsync("delay", 100).ConfigureAwait(false);
                                var z = await hitNode.GetAsync("z", 0).ConfigureAwait(false);

                                hitFrameInfo.Add(new FrameInfo($"\"/Paper2D.PaperSprite'/Game/Mob/{id}/hit/S_{frame}.S_{frame}'\"")
                                {
                                    Delay = delay,
                                    OffsetX = origin.X,
                                    OffsetY = origin.Y,
                                    ZOrder = z,
                                    ZigZag = false,
                                    HasAlpha = false,
                                    AlphaStart = 0,
                                    AlphaEnd = 0
                                });

                                Directory.CreateDirectory($"./Exported/{id}/hit");
                                await image.SaveAsPngAsync($"./Exported/{id}/hit/T_{frame}.png").ConfigureAwait(false);
                            }

                            index = 1;

                            foreach (var frame in hitFrameInfo)
                            {
                                frame.Name = index++;
                            }

                            await using (var write = new StreamWriter($"./Exported/{id}/hit/DT_Frames.csv"))
                            {
                                await using var csv = new CsvWriter(write, CultureInfo.InvariantCulture);
                                await csv.WriteRecordsAsync(hitFrameInfo.OrderBy(x => x.Name)).ConfigureAwait(false);
                            }

                            var hitContent = await File.ReadAllTextAsync($"./Exported/{id}/hit/DT_Frames.csv").ConfigureAwait(false);
                            var hitChanged = hitContent.Replace("\"\"\"", "\"");
                            await File.WriteAllTextAsync($"./Exported/{id}/hit/DT_Frames.csv", hitChanged).ConfigureAwait(false);
                        }
                    }
                }
            }

            foreach (var id in removeList)
            {
                _mobInfos.Remove(id);
            }
        }

        private static async Task<T> GetAsync<T>(this WzProperty property, string name, T defaultValue) where T : struct
        {
            var resolved = await property.ResolveFor<T>(name).ConfigureAwait(false);
            return resolved ?? defaultValue;
        }

        private static async Task<bool> HasChildAsync(this WzProperty property, string name)
        {
            var node = await property.Resolve(name).ConfigureAwait(false);
            return node != null;
        }
    }
}