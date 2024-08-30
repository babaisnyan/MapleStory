using System.Collections.Concurrent;
using System.Globalization;
using CsvHelper;
using FastEnumUtility;
using Newtonsoft.Json;
using PKG1;

namespace ItemExporter;

internal static class Program
{
    private const string MAPLE_STORY_PATH = @"C:\Nexon\Maple\Data";

    private static readonly string[] SkipWords =
    [
        "어빌리티", "서큘레이터", "헤어", "성형",
        "교환", "마스터리", "스킨", "심볼",
        "이모티", "라이딩", "쿠폰", "소울",
        "레시피", "상자", "펫", "카드", 
        "변신", "스킬", "보따리", "환생",
        "테스트", "이펙트"
    ];

    private static IDictionary<int, ItemInfo> _itemInfos = null!;

    private static async Task Main(string[] args)
    {
        if (Directory.Exists("./Exported")) Directory.Delete("./Exported", true);
        Directory.CreateDirectory("./Exported");

        Console.WriteLine("Loading Package Collection");
        var collection = new PackageCollection(MAPLE_STORY_PATH);
        Console.WriteLine("Package Collection Loaded");

        var stringCollection = await collection.Resolve("String").ConfigureAwait(false);
        var characterCollection = await collection.Resolve("Character").ConfigureAwait(false);
        var itemCollection = await collection.Resolve("Item").ConfigureAwait(false);
        _itemInfos = GetItemInfos(stringCollection);

        await FillItemInfoAsync(itemCollection).ConfigureAwait(false);
        await FillEquipInfoAsync(characterCollection).ConfigureAwait(false);

        var index = 1;

        foreach (var kvp in _itemInfos)
        {
            kvp.Value.Name = index++;
        }

        await SaveToCsvAsync().ConfigureAwait(false);
        await SaveToJsonAsync().ConfigureAwait(false);
    }

    private static async Task SaveToCsvAsync()
    {
        await using var writer = new StreamWriter("Items.csv");
        await using var csv = new CsvWriter(writer, CultureInfo.InvariantCulture);

        await csv.WriteRecordsAsync(_itemInfos.Values.OrderBy(x => x.Name)).ConfigureAwait(false);
    }

    private static async Task SaveToJsonAsync()
    {
        await using var writer = new StreamWriter("Items.json");
        await writer.WriteAsync(JsonConvert.SerializeObject(_itemInfos.Values.OrderBy(x => x.ItemId), Formatting.Indented)).ConfigureAwait(false);
    }

    private static IDictionary<int, ItemInfo> GetItemInfos(WzProperty collection)
    {
        var itemTypes = new Dictionary<string, ItemType>
        {
            {"Eqp.img", ItemType.Eqp},
            {"Consume.img", ItemType.Use},
            {"Etc.img", ItemType.Etc}
        };

        var items = new ConcurrentDictionary<int, ItemInfo>();
        var itemNames = new ConcurrentDictionary<string, bool>();

        Parallel.ForEach(itemTypes, async kvp =>
        {
            switch (kvp.Value)
            {
                case ItemType.Eqp:
                {
                    var subTypes = FastEnum.GetNames<SubItemType>();

                    foreach (var subType in subTypes)
                    {
                        var directory = await collection.Resolve($"{kvp.Key}/Eqp/{subType}").ConfigureAwait(false);

                        foreach (var node in directory.Children)
                        {
                            var info = await GetSimpleItemInfoAsync(node).ConfigureAwait(false);
                            if (info == null) continue;
                            if (itemNames.ContainsKey(info.ItemName)) continue;

                            info.SubType = (byte) FastEnum.Parse<SubItemType>(subType);
                            items.TryAdd(info.ItemId, info);
                            itemNames.TryAdd(info.ItemName, true);
                        }
                    }

                    break;
                }
                case ItemType.Use:
                {
                    var directory = await collection.Resolve($"{kvp.Key}").ConfigureAwait(false);

                    foreach (var node in directory.Children)
                    {
                        var info = await GetSimpleItemInfoAsync(node).ConfigureAwait(false);
                        if (info == null) continue;
                        if (itemNames.ContainsKey(info.ItemName)) continue;
                        items.TryAdd(info.ItemId, info);
                        itemNames.TryAdd(info.ItemName, true);
                    }

                    break;
                }
                case ItemType.Etc:
                {
                    var directory = await collection.Resolve($"{kvp.Key}/Etc").ConfigureAwait(false);

                    foreach (var node in directory.Children)
                    {
                        var info = await GetSimpleItemInfoAsync(node).ConfigureAwait(false);
                        if (info == null) continue;
                        if (itemNames.ContainsKey(info.ItemName)) continue;
                        items.TryAdd(info.ItemId, info);
                        itemNames.TryAdd(info.ItemName, true);
                    }

                    break;
                }
            }
        });

        return items;
    }

    private static async Task FillItemInfoAsync(WzProperty collection)
    {
        var nodes = collection.Children.Where(x => x._name == "Etc" || x._name == "Consume")
                              .SelectMany(x => x.Children)
                              .Where(x => !x._path.Contains("_Canvas"))
                              .SelectMany(x => x.Children)
                              .Where(x => int.TryParse(x._name, out _));

        foreach (var node in nodes)
        {
            var id = node._name.Contains(".img") ? int.Parse(node._name.Replace(".img", "")) : int.Parse(node._name);
            var info = await node.Resolve("info").ConfigureAwait(false);
            var spec = await node.Resolve("spec").ConfigureAwait(false);

            if (!_itemInfos.TryGetValue(id, out var itemInfo)) continue;
            if (info == null) continue;

            foreach (var wzProperty in info.Children)
            {
                switch (wzProperty._name)
                {
                    case "icon":
                    case "iconRaw":
                    {
                        // var image = await wzProperty.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false);
                        //
                        // if (image != null)
                        // {
                        //     await image.SaveAsPngAsync($"./Exported/T_{id}_{wzProperty._name}.png").ConfigureAwait(false);
                        // }

                        break;
                    }
                    case "price":
                    {
                        var resolved = await wzProperty.ResolveFor<int>().ConfigureAwait(false);
                        if (resolved != null) itemInfo.Price = resolved.Value;
                        break;
                    }
                    case "slotMax":
                    {
                        var resolved = await wzProperty.ResolveFor<int>().ConfigureAwait(false);
                        if (resolved != null) itemInfo.SlotMax = resolved.Value;
                        break;
                    }
                }
            }

            if (spec == null) continue;

            itemInfo.Hp = await spec.GetAsync("hp", 0).ConfigureAwait(false);
            itemInfo.Mp = await spec.GetAsync("mp", 0).ConfigureAwait(false);
            itemInfo.Time = await spec.GetAsync("time", 0).ConfigureAwait(false);
        }
    }

    private static async Task FillEquipInfoAsync(WzProperty collection)
    {
        var nodes = FastEnum.GetNames<SubItemType>()
                            .SelectMany(x => collection.Children.Where(y => y._name == x))
                            .Where(x => !x._path.Contains("_Canvas"))
                            .SelectMany(x => x.Children)
                            .Where(x => int.TryParse(x._nameWithoutExtension, out _));

        foreach (var node in nodes)
        {
            var id = int.Parse(node._name.Replace(".img", ""));
            var info = await node.Resolve("info").ConfigureAwait(false);

            if (!_itemInfos.TryGetValue(id, out var itemInfo)) continue;
            if (info == null) continue;

            var cash = await info.GetAsync("cash", 0).ConfigureAwait(false);

            if (cash != 0)
            {
                _itemInfos.Remove(id);
                continue;
            }

            foreach (var wzProperty in info.Children)
            {
                switch (wzProperty._name)
                {
                    case "icon":
                    case "iconRaw":
                    {
                        // var image = await wzProperty.ResolveForOrNull<Image<Rgba32>>().ConfigureAwait(false);
                        //
                        // if (image != null)
                        // {
                        //     await image.SaveAsPngAsync($"./Exported/T_{id}_{wzProperty._name}.png").ConfigureAwait(false);
                        // }

                        break;
                    }
                }
            }

            itemInfo.IncAcc = await info.GetAsync("IncACC", 0).ConfigureAwait(false);
            itemInfo.IncDex = await info.GetAsync("incDEX", 0).ConfigureAwait(false);
            itemInfo.IncEva = await info.GetAsync("incEVA", 0).ConfigureAwait(false);
            itemInfo.IncInt = await info.GetAsync("incINT", 0).ConfigureAwait(false);
            itemInfo.IncJump = await info.GetAsync("incJump", 0).ConfigureAwait(false);
            itemInfo.IncLuk = await info.GetAsync("incLUK", 0).ConfigureAwait(false);
            itemInfo.IncMad = await info.GetAsync("incMAD", 0).ConfigureAwait(false);
            itemInfo.IncMdd = await info.GetAsync("incMDD", 0).ConfigureAwait(false);
            itemInfo.IncMhp = await info.GetAsync("incMHP", 0).ConfigureAwait(false);
            itemInfo.IncPad = await info.GetAsync("incPAD", 0).ConfigureAwait(false);
            itemInfo.IncPdd = await info.GetAsync("incPDD", 0).ConfigureAwait(false);
            itemInfo.IncSpeed = await info.GetAsync("incSpeed", 0).ConfigureAwait(false);
            itemInfo.Price = await info.GetAsync("price", 0).ConfigureAwait(false);
            itemInfo.SlotMax = await info.GetAsync("slotMax", 0).ConfigureAwait(false);
            itemInfo.ReqLevel = await info.GetAsync("reqLevel", 0).ConfigureAwait(false);
            itemInfo.ReqStr = await info.GetAsync("reqSTR", 0).ConfigureAwait(false);
            itemInfo.ReqDex = await info.GetAsync("reqDEX", 0).ConfigureAwait(false);
            itemInfo.ReqInt = await info.GetAsync("reqINT", 0).ConfigureAwait(false);
            itemInfo.ReqLuk = await info.GetAsync("reqLUK", 0).ConfigureAwait(false);
            itemInfo.IncStr = await info.GetAsync("incSTR", 0).ConfigureAwait(false);
            itemInfo.IncMmd = await info.GetAsync("incMDD", 0).ConfigureAwait(false);
        }
    }

    private static async Task<ItemInfo?> GetSimpleItemInfoAsync(WzProperty itemImg)
    {
        var id = int.Parse(itemImg._name);
        var name = await itemImg.ResolveForOrNull<string>("name").ConfigureAwait(false) ?? "";

        if (name == string.Empty) return null;
        if (SkipWords.Any(x => name.Contains(x))) return null;

        var desc = await itemImg.ResolveForOrNull<string>("desc").ConfigureAwait(false) ?? "";
        var info = new ItemInfo(id, name, ItemType.Eqp) {Desc = desc};

        return info;
    }
}