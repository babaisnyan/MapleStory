using CsvHelper.Configuration.Attributes;
using Newtonsoft.Json;
using SixLabors.ImageSharp;

namespace SoundExporter;

public class MobInfo
{
    [JsonIgnore]
    public int Name { get; set; }

    public int MobId { get; init; }
    public string MobName { get; init; }
    public int Level { get; init; }
    public int MaxHp { get; init; }
    public bool BodyAttack { get; init; }
    public bool FirstAttack { get; init; }
    public int Speed { get; init; }
    public int PaDamage { get; init; }
    public int MaDamage { get; init; }
    public int PdDamage { get; init; }
    public int MdDamage { get; init; }
    public int PdRate { get; init; }
    public int MdRate { get; init; }
    public int Exp { get; init; }
    public bool HasStand { get; init; }
    public bool HasMove { get; init; }
    public bool HasHit { get; init; }
    public bool HasDie { get; init; }
    public bool HasRegen { get; init; }
    public bool HasAttack { get; init; }
    public int AttackWidth { get; init; }
    public int AttackHeight { get; init; }

    [JsonIgnore]
    public string CharHitSound { get; set; } = "\"\"";

    [JsonIgnore]
    public string DieSound { get; set; } = "\"\"";

    [JsonIgnore]
    public string HitSound { get; set; } = "\"\"";

    [JsonIgnore]
    public string RegenSound { get; set; } = "\"\"";

    [JsonIgnore]
    public string AttackSound { get; set; } = "\"\"";

    [Ignore]
    public int HitLength { get; init; }

    [Ignore]
    public int DieLength { get; init; }

    [Ignore]
    public int AttackLength { get; init; }

    [Ignore]
    public int RegenLength { get; init; }

    [Ignore]
    public Point StandSize { get; init; }

    [Ignore]
    public Point MoveSize { get; init; }

    [Ignore]
    public Point HitSize { get; init; }

    [Ignore]
    public Point AttackSize { get; init; }

    public int AttackDelay { get; init; }
}