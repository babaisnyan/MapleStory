using Newtonsoft.Json;

namespace MobExporter;

public class FrameInfo(int mobId, string action, int frame)
{
    public int Name { get; set; }
    public string Sprite { get; } = $"\"/Paper2D.PaperSprite'/Game/Mob/{mobId}/{action}/S_{frame}.S_{frame}'\"";
    public int Delay { get; set; } = 100;
    public int OffsetX { get; set; }
    public int OffsetY { get; set; }
    public int ZOrder { get; set; }
    public bool ZigZag { get; set; }
    public bool HasAlpha { get; set; } = false;
    public int AlphaStart { get; set; } = 255;
    public int AlphaEnd { get; set; }
}