using SpacetimeDB;

public partial class Module
{
    [SpacetimeDB.Reducer]
    public static void CreatePlayer(ReducerContext ctx, string name)
    {
        var p = ctx.Db.Player.Insert(new Player { Name = name });
        Log.Info($"[Player Created] {p.Id} = {p.Name}");
    }
}