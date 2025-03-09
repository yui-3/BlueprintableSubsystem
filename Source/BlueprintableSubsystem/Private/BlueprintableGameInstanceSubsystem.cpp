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

bool UBlueprintableGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (GetClass()->IsFunctionImplementedInScript(
		GET_FUNCTION_NAME_CHECKED(UBlueprintableGameInstanceSubsystem, BP_ShouldCreateSubsystem)))
	{
		return BP_ShouldCreateSubsystem();
	}
	return Super::ShouldCreateSubsystem(Outer);
}
