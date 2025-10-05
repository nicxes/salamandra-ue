# Salamandra-UE

SpacetimeDB multiplayer game built with Unreal Engine 5.

## Prerequisites

Before you start, make sure you have the following installed:

### Required Software

1. **Unreal Engine 5.6+**
   - Download from [Epic Games Launcher](https://www.unrealengine.com/)

2. **.NET SDK 8.0**
   - **Required for SpacetimeDB C# server module**
   - Download: [.NET 8.0 SDK](https://dotnet.microsoft.com/download/dotnet/8.0)
   - Install the **SDK** (not just the Runtime)
   - For Windows: Download the **x64** installer

3. **SpacetimeDB CLI**
   - Follow installation instructions at [SpacetimeDB Getting Started](https://spacetimedb.com/docs/getting-started)

## Project Structure

```
salamandra-ue/
├── client_unreal/          # Unreal Engine client project
│   ├── Plugins/
│   │   └── SpacetimeDbSdk/ # SpacetimeDB Unreal SDK
│   └── Source/
└── server_spacetimedb/     # SpacetimeDB server module (C#)
    ├── Lib.cs
    ├── StdbModule.csproj
    └── global.json
```

## Setup Instructions

### 1. Verify .NET SDK Installation

After installing .NET SDK 8.0, open a terminal and verify:

```bash
dotnet --version
```

You should see version `8.0.x` or higher.

### 2. Start SpacetimeDB Local Server

Navigate to the project root directory and start the local SpacetimeDB server:

```bash
cd salamandra-ue
spacetime start
```
