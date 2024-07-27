namespace ItemExporter;

public class ItemInfo(int itemId, string itemName, ItemType type)
{
    public int Name { get; set; }
    public int ItemId { get; } = itemId;
    public string Icon { get; } = $"\"/Paper2D.PaperSprite'/Game/Item/S_{itemId}_icon.S_{itemId}_icon'\"";
    public string IconRaw { get; } = $"\"/Paper2D.PaperSprite'/Game/Item/S_{itemId}_iconRaw.S_{itemId}_iconRaw'\"";
    public string ItemName { get; } = itemName;
    public string Desc { get; set; }
    public byte ItemType { get; } = (byte) type;
    public byte SubType { get; set; }
    public int IncAcc { get; set; }
    public int IncDex { get; set; }
    public int IncEva { get; set; }
    public int IncInt { get; set; }
    public int IncJump { get; set; }
    public int IncLuk { get; set; }
    public int IncMad { get; set; }
    public int IncMdd { get; set; }
    public int IncMhp { get; set; }
    public int IncPad { get; set; }
    public int IncPdd { get; set; }
    public int IncSpeed { get; set; }
    public int Price { get; set; }
    public int SlotMax { get; set; }
    public int ReqLevel { get; set; }
    public int ReqStr { get; set; }
    public int ReqDex { get; set; }
    public int ReqInt { get; set; }
    public int ReqLuk { get; set; }
    public int IncStr { get; set; }
    public int IncMmd { get; set; }
    public int Hp { get; set; }
    public int Mp { get; set; }
    public int Time { get; set; }
}