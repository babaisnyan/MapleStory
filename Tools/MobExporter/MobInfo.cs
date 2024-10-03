using CsvHelper.Configuration.Attributes;
using Newtonsoft.Json;
using SixLabors.ImageSharp;

namespace MobExporter;

public class MobInfo(int mobId, string name)
{
    [JsonIgnore]
    public int Name { get; set; }

    public int MobId { get; } = mobId;
    public string MobName { get; } = name;
    public int Level { get; set; }
    public int MaxHp { get; set; }
    public bool BodyAttack { get; set; }
    public bool FirstAttack { get; set; }
    public int Speed { get; set; }
    public int PaDamage { get; set; }
    public int MaDamage { get; set; }
    public int PdDamage { get; set; }
    public int MdDamage { get; set; }
    public int PdRate { get; set; }
    public int MdRate { get; set; }
    public int Exp { get; set; }
    public bool HasStand { get; set; }
    public bool HasMove { get; set; }
    public bool HasHit { get; set; }
    public bool HasDie { get; set; }
    public bool HasRegen { get; set; }
    public bool HasAttack { get; set; }
    public int AttackWidth { get; set; }
    public int AttackHeight { get; set; }

    [Ignore]
    public int HitLength { get; set; }

    [Ignore]
    public int DieLength { get; set; }

    [Ignore]
    public int AttackLength { get; set; }

    [Ignore]
    public int RegenLength { get; set; }

    [Ignore]
    public Point StandSize { get; set; }

    [Ignore]
    public Point MoveSize { get; set; }

    [Ignore]
    public Point HitSize { get; set; }

    [Ignore]
    public Point AttackSize { get; set; }

    public int AttackDelay { get; set; }
}