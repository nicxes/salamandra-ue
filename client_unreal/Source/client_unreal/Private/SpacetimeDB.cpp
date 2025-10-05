// Fill out your copyright notice in the Description page of Project Settings.

#include "SpacetimeDB.h"
#include "Connection/Credentials.h"
#include "Containers/Ticker.h"

DEFINE_LOG_CATEGORY(LogSpacetimeDB);

void USpacetimeDB::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogSpacetimeDB, Log, TEXT("SpacetimeDB Subsystem initializing..."));
	
	// Setup connection delegates
	FOnConnectDelegate ConnectDelegate;
	BIND_DELEGATE_SAFE(ConnectDelegate, this, USpacetimeDB, HandleConnect);
	
	FOnDisconnectDelegate DisconnectDelegate;
	BIND_DELEGATE_SAFE(DisconnectDelegate, this, USpacetimeDB, HandleDisconnect);
	
	FOnConnectErrorDelegate ConnectErrorDelegate;
	BIND_DELEGATE_SAFE(ConnectErrorDelegate, this, USpacetimeDB, HandleConnectError);
	
	// Initialize credentials and load token
	UCredentials::Init(*TokenFilePath);
	FString Token = UCredentials::LoadToken();
	
	// Build connection
	UDbConnectionBuilder* Builder = UDbConnection::Builder()
		->WithUri(ServerUri)
		->WithModuleName(ModuleName)
		->OnConnect(ConnectDelegate)
		->OnDisconnect(DisconnectDelegate)
		->OnConnectError(ConnectErrorDelegate);
	
	if (!Token.IsEmpty())
	{
		Builder->WithToken(Token);
	}
	
	Conn = Builder->Build();
	
	// Setup ticker for FrameTick
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &USpacetimeDB::TickConnection)
	);
	
	UE_LOG(LogSpacetimeDB, Log, TEXT("SpacetimeDB Subsystem initialized."));
}

void USpacetimeDB::Deinitialize()
{
	UE_LOG(LogSpacetimeDB, Log, TEXT("SpacetimeDB Subsystem deinitializing..."));
	
	// Remove ticker
	if (TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
		TickerHandle.Reset();
	}
	
	// Disconnect
	Disconnect();
	
	Super::Deinitialize();
}

bool USpacetimeDB::TickConnection(float DeltaTime)
{
	if (IsConnected())
	{
		Conn->FrameTick();
	}
	return true; // Return true to keep ticking
}

void USpacetimeDB::HandleConnect(UDbConnection* InConn, FSpacetimeDBIdentity Identity, const FString& Token)
{
	UE_LOG(LogSpacetimeDB, Log, TEXT("Connected to SpacetimeDB!"));
	
	// Save token for future connections
	UCredentials::SaveToken(Token);
	LocalIdentity = Identity;
	
	// Subscribe to all tables
	FOnSubscriptionApplied AppliedDelegate;
	BIND_DELEGATE_SAFE(AppliedDelegate, this, USpacetimeDB, HandleSubscriptionApplied);
	
	Conn->SubscriptionBuilder()
		->OnApplied(AppliedDelegate)
		->SubscribeToAllTables();
}

void USpacetimeDB::HandleConnectError(const FString& Error)
{
	UE_LOG(LogSpacetimeDB, Error, TEXT("Connection error: %s"), *Error);
}

void USpacetimeDB::HandleDisconnect(UDbConnection* InConn, const FString& Error)
{
	UE_LOG(LogSpacetimeDB, Log, TEXT("Disconnected from SpacetimeDB."));
	
	if (!Error.IsEmpty())
	{
		UE_LOG(LogSpacetimeDB, Warning, TEXT("Disconnect error: %s"), *Error);
	}
}

void USpacetimeDB::HandleSubscriptionApplied(FSubscriptionEventContext& Context)
{
	UE_LOG(LogSpacetimeDB, Log, TEXT("Subscription applied! Client cache synchronized."));
}
