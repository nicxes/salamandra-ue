using SpacetimeDB;

public partial class Module
{
    [SpacetimeDB.Table]
    public partial class Player
    {
        [SpacetimeDB.AutoInc]
        [SpacetimeDB.PrimaryKey]
        public int Id;

        public string Name;

        public int Level = 1;
    }
}