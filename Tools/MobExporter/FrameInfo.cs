namespace MobExporter;

public class FrameInfo(int mobId, string action, int frame)
{
    public int Name { get; set; }
    public string Sprite { get; } = $"\"/Paper2D.PaperSprite'/Game/Mob/{mobId}/{action}/S_{frame}.S_{frame}'\"";
    public int Delay { get; set; } = 100;
    public int OffsetX { get; set; } = 0;
    public int OffsetY { get; set; } = 0;
    public int ZOrder { get; set; } = 0;
    public bool ZigZag { get; set; } = false;
}