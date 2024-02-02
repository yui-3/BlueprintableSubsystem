// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintableGameInstanceSubsystem.h"

void UBlueprintableGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	for (auto& DependencyClass : DependencyClasses)
	{
		Collection.InitializeDependency(DependencyClass);
	}
	BP_Initialize();
}

UGameInstance* UBlueprintableGameInstanceSubsystem::BP_GetGameInstance() const
{
	return GetGameInstance();
}
