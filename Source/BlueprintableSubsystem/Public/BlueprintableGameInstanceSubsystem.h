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

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/** Override to control if the Subsystem should be created at all.
 	* For example you could only have your system created on servers.
 	* It's important to note that if using this is becomes very important to null check whenever getting the Subsystem.
 	*
 	* Note: This function is called on the CDO prior to instances being created!
 	*
 	*/
	UFUNCTION(BlueprintImplementableEvent)
	bool BP_ShouldCreateSubsystem() const;
};
