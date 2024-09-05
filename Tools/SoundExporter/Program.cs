using System.Diagnostics;
using CsvHelper;
using System.Globalization;
using NAudio.Wave;
using Newtonsoft.Json;
using PKG1;

namespace SoundExporter
{
    internal class Program
    {
        private const string MAPLE_STORY_PATH = @"D:\Maple\Data";
        private static IDictionary<int, MobInfo>? _mobInfos = null!;
        private static readonly Dictionary<string, string> Hash = [];

        private static async Task Main(string[] args)
        {
            if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
            Directory.CreateDirectory("./Exported");

            Console.WriteLine("Loading Package Collection");
            var collection = new PackageCollection(MAPLE_STORY_PATH);
            Console.WriteLine("Package Collection Loaded");

            var json = await File.ReadAllTextAsync("Mobs.json").ConfigureAwait(false);
            var list = JsonConvert.DeserializeObject<IList<MobInfo>>(json);
            _mobInfos = list.ToDictionary(x => x.MobId);

            var soundCollection = await collection.Resolve("Sound").ConfigureAwait(false);

            await SaveMobSoundsAsync(soundCollection).ConfigureAwait(false);

            var index = 1;

            foreach (var kvp in _mobInfos)
            {
                kvp.Value.Name = index++;
            }


            await using (var write = new StreamWriter("Mobs.csv"))
            {
                await using var csv = new CsvWriter(write, CultureInfo.InvariantCulture);
                await csv.WriteRecordsAsync(_mobInfos.Values.OrderBy(x => x.Name)).ConfigureAwait(false);
            }

            var content = await File.ReadAllTextAsync("Mobs.csv").ConfigureAwait(false);
            var changed = content.Replace("\"\"\"", "\"");
            await File.WriteAllTextAsync("Mobs.csv", changed).ConfigureAwait(false);
        }

        private static async Task SaveMobSoundsAsync(WzProperty collection)
        {
            var img = await collection.Resolve("Mob.img").ConfigureAwait(false);

            foreach (var (id, mobInfo) in _mobInfos!)
            {
                var mobSounds = await img.Resolve($"{id:D7}.img").ConfigureAwait(false);
                var sounds = new Dictionary<string, WzProperty>();

                if (mobSounds == null) continue;

                if (mobInfo.HasHit)
                {
                    var hitSound = await mobSounds.Resolve("Hit").ConfigureAwait(false);
                    var damageSound = await mobSounds.Resolve("Damage").ConfigureAwait(false);

                    if (hitSound != null)
                    {
                        sounds.Add("Hit", hitSound);
                    }
                    else if (damageSound != null)
                    {
                        sounds.Add("Damage", damageSound);
                    }
                }

                if (mobInfo.HasDie)
                {
                    var dieSound = await mobSounds.Resolve("Die").ConfigureAwait(false);

                    if (dieSound != null)
                    {
                        sounds.Add("Die", dieSound);
                    }
                }

                if (mobInfo.HasRegen)
                {
                    var regenSound = await mobSounds.Resolve("Regen").ConfigureAwait(false);

                    if (regenSound != null)
                    {
                        sounds.Add("Regen", regenSound);
                    }
                }

                if (mobInfo.HasAttack)
                {
                    var attackSound = await mobSounds.Resolve("Attack1").ConfigureAwait(false);
                    var charDamSound = await mobSounds.Resolve("CharDam").ConfigureAwait(false);
                    var charDam1Sound = await mobSounds.Resolve("CharDam1").ConfigureAwait(false);

                    if (attackSound != null)
                    {
                        sounds.Add("Attack", attackSound);
                    }

                    if (charDamSound != null)
                    {
                        sounds.Add("CharHit", charDamSound);
                    }
                    else if (charDam1Sound != null)
                    {
                        sounds.Add("CharHit", charDam1Sound);
                    }
                }

                if (sounds.Count > 0)
                {
                    Directory.CreateDirectory($"./Exported/{id}");
                }

                foreach (var (name, node) in sounds)
                {
                    if (node is IWzPropertyVal<byte[]> nodeData)
                    {
                        var data = nodeData.Value;
                        var path = $"\"/Engine.SoundWave'/Game/Sound/Mob/{id}/{name}.{name}'\"";
                        var hash = await data.ComputeHashAsync().ConfigureAwait(false);
                        var found = false;

                        if (Hash.TryGetValue(hash, out var value))
                        {
                            path = value;
                            found = true;
                        }
                        else
                        {
                            Hash.Add(hash, path);
                        }

                        switch (name)
                        {
                            case "Hit":
                            case "Damage":
                                mobInfo.HitSound = path;
                                break;
                            case "Die":
                                mobInfo.DieSound = path;
                                break;
                            case "Regen":
                                mobInfo.RegenSound = path;
                                break;
                            case "Attack":
                                mobInfo.AttackSound = path;
                                break;
                            case "CharHit":
                                mobInfo.CharHitSound = path;
                                break;
                        }

                        if (!found)
                        {
                            await File.WriteAllBytesAsync($"./Exported/{id}/{name}.mp3", data).ConfigureAwait(false);
                        }
                    }
                }
            }
        }
    }
}