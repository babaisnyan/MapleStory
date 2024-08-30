namespace NpcExporter;

public class NpcInfo(int id, string name)
{
    public int Name { get; set; }
    public int Id { get; } = id;
    public string NpcName { get; } = name;
    public Dictionary<string, string> Texts { get; } = [];
    public bool IsShop { get; set; }
}