using SpacetimeDB;

public static partial class Module
{
    [SpacetimeDB.Table]
    public partial struct Person
    {
        [SpacetimeDB.AutoInc]
        [SpacetimeDB.PrimaryKey]
        public int Id;
        public string Name;
        public int Age;
    }

    [SpacetimeDB.Reducer]
    public static void Add(ReducerContext ctx, string name, int age)
    {
        var person = ctx.Db.Person.Insert(new Person { Name = name, Age = age });
        Log.Info($"Inserted {person.Name} under #{person.Id}");
    }

    [SpacetimeDB.Reducer]
    public static void SayHello(ReducerContext ctx)
    {
        foreach (var person in ctx.Db.Person.Iter())
        {
            Log.Info($"Hello, {person.Name}!");
        }
        Log.Info("Hello, World!");
    }

    [Reducer]
    public static void Debug(ReducerContext ctx)
    {
        Log.Info($"This reducer was called by {ctx.Sender}");
    }

    [Reducer(ReducerKind.ClientConnected)]
    public static void Connect(ReducerContext ctx)
    {
        Log.Info($"{ctx.Sender} just connected.");
    }

    [Reducer(ReducerKind.ClientDisconnected)]
    public static void Disconnect(ReducerContext ctx)
    {
        Log.Info($"{ctx.Sender} just disconnected.");
    }
}
