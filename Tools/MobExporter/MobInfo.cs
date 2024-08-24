namespace MobExporter;

public class MobInfo(int mobId, string name)
{
    public int Name { get; set; }
    public int MobId { get; } = mobId;
    public string MobName { get; } = name;
    public int Level { get; set; }
    public int MaxHp { get; set; }
    public int MaxMp { get; set; }
    public bool BodyAttack { get; set; }
    public int Speed { get; set; }
    public int PaDamage { get; set; }
    public int MaDamage { get; set; }
    public int PdDamage { get; set; }
    public int MdDamage { get; set; }
    public int PdRate { get; set; }
    public int MdRate { get; set; }
    public int Accuracy { get; set; }
    public int Evasion { get; set; }
    public int Exp { get; set; }
}