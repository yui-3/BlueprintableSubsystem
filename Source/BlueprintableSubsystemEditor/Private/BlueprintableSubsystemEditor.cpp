#include "BlueprintableSubsystemEditor.h"

#include "ContentBrowserModule.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "BlueprintableGameInstanceSubsystem.h"
#include "BlueprintSubsystemSettings.h"
#include "ISettingsModule.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "FBlueprintableSubsytemEditorModule"

void FBlueprintableSubsystemEditorModule::HandleNewBlueprintableSubsystemBlueprint(UBlueprint* Blueprint)
{
	auto BlueprintClass = Blueprint->GetClass();

	//check blueprint is a subclass of UBlueprintableGameInstanceSubsystem
	if (Blueprint->GeneratedClass->IsChildOf(UBlueprintableGameInstanceSubsystem::StaticClass()))
	{
		if (UBlueprintSubsystemSettings* Settings = GetMutableDefault<UBlueprintSubsystemSettings>(); IsValid(Settings))
		{
			bool bFound = false;
			for (auto BlueprintSubsystem : Settings->BlueprintSubsystems)
			{
				if (BlueprintSubsystem == Blueprint->GetClass())
				{
					bFound = true;
					break;
				}
			}
			if (bFound) return;
			const TSubclassOf<UBlueprintableGameInstanceSubsystem> NewSubsystemClass(Blueprint->GeneratedClass);
			//how can we add a new entry to the settings?
			Settings->BlueprintSubsystems.Add(NewSubsystemClass);
			Settings->SaveConfig();
		}
	}
}


void FBlueprintableSubsystemEditorModule::ShutdownModule()
{
}

void FBlueprintableSubsystemEditorModule::StartupModule()
{
	FKismetEditorUtilities::RegisterOnBlueprintCreatedCallback(
		this,
		UBlueprintableGameInstanceSubsystem::StaticClass(),
		FKismetEditorUtilities::FOnBlueprintCreated::CreateRaw(
			this, &FBlueprintableSubsystemEditorModule::HandleNewBlueprintableSubsystemBlueprint)
	);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBlueprintableSubsystemEditorModule, BlueprintableSubsytemEditor)
