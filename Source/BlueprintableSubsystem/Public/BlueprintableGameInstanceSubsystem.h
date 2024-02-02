// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BlueprintableGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class BLUEPRINTABLESUBSYSTEM_API UBlueprintableGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameInstanceSubsystem>> DependencyClasses;

	UFUNCTION(BlueprintCallable, DisplayName="Get Game Instance")
	UGameInstance* BP_GetGameInstance() const;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="Initialize")
	void BP_Initialize();
};
