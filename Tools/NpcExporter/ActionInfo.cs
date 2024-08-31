namespace NpcExporter;

public class ActionInfo(string name, List<string> scripts)
{
    public string Name { get; } = name;
    public int Length { get; set; }
    public List<string> Scripts { get; } = scripts;
}