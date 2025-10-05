// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ModuleBindings/SpacetimeDBClient.g.h"
#include "SpacetimeDB.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpacetimeDB, Log, All);

class UDbConnection;

/**
 * GameInstanceSubsystem that manages the connection to SpacetimeDB
 * Persists across level changes
 */
UCLASS()
class CLIENT_UNREAL_API USpacetimeDB : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Connection settings
	UPROPERTY(EditAnywhere, Category="SpacetimeDB|Connection")
	FString ServerUri = TEXT("127.0.0.1:3000");
	
	UPROPERTY(EditAnywhere, Category="SpacetimeDB|Connection")
	FString ModuleName = TEXT("salamandra");
	
	UPROPERTY(EditAnywhere, Category="SpacetimeDB|Connection")
	FString TokenFilePath = TEXT(".spacetime_salamandra");

	// Connection state
	UPROPERTY(BlueprintReadOnly, Category="SpacetimeDB|Connection")
	FSpacetimeDBIdentity LocalIdentity;
	
	UPROPERTY(BlueprintReadOnly, Category="SpacetimeDB|Connection")
	UDbConnection* Conn = nullptr;

	// Check if connected
	UFUNCTION(BlueprintPure, Category="SpacetimeDB|Connection")
	bool IsConnected() const
	{
		return Conn != nullptr && Conn->IsActive();
	}
	
	// Disconnect from server
	UFUNCTION(BlueprintCallable, Category="SpacetimeDB|Connection")
	void Disconnect()
	{
		if (Conn != nullptr)
		{
			Conn->Disconnect();
			Conn = nullptr;
		}
	}

protected:
	// GameInstanceSubsystem overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	// Connection event handlers
	UFUNCTION()
	void HandleConnect(UDbConnection* InConn, FSpacetimeDBIdentity Identity, const FString& Token);
	
	UFUNCTION()
	void HandleConnectError(const FString& Error);
	
	UFUNCTION()
	void HandleDisconnect(UDbConnection* InConn, const FString& Error);
	
	UFUNCTION()
	void HandleSubscriptionApplied(FSubscriptionEventContext& Context);

	// Ticker for FrameTick
	FTSTicker::FDelegateHandle TickerHandle;
	bool TickConnection(float DeltaTime);
};
