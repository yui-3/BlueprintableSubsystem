// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintableSubsystem.h"

#include "BlueprintableGameInstanceSubsystem.h"
#include "BlueprintSubsystemSettings.h"
#include "UnrealEdMisc.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Developer/Settings/Public/ISettingsContainer.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FBlueprintableSubsystemModule"

bool FBlueprintableSubsystemModule::HandleBlueprintSubsystemSettingsSaved()
{
	UBlueprintSubsystemSettings* Settings = GetMutableDefault<UBlueprintSubsystemSettings>();
	bool Resave = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (Resave)
	{
		Settings->SaveConfig();
	}
	if (!NotificationPtr.IsValid())
	{
		FNotificationInfo NotifyInfo(FText::FromString("Restart Editor to apply changes"));
		//NotifyInfo.bUseLargeFont = true;
		NotifyInfo.FadeOutDuration = 200.f;
		NotifyInfo.ButtonDetails.Add(FNotificationButtonInfo(
			FText::FromString("Restart"), FText::FromString("Restart Editor to apply changes"),
			FSimpleDelegate::CreateLambda([&]()
			{
				FUnrealEdMisc::Get().RestartEditor(false);

				NotificationPtr = nullptr;
			})));
		NotificationPtr = FSlateNotificationManager::Get().AddNotification(NotifyInfo);
		if (const auto NotificationPin = NotificationPtr.Pin(); NotificationPin.IsValid())
		{
			NotificationPin->SetCompletionState(SNotificationItem::CS_Pending);
			NotificationPin->ExpireAndFadeout();
		}
	}
	return true;
}

void FBlueprintableSubsystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (const auto SettingsSection = SettingsModule->RegisterSettings("Project", "Game", "Subsystems",
		                                                                  NSLOCTEXT("YourModule",
			                                                                  "BlueprintSubsystemSettingsName",
			                                                                  "Blueprint GameInstance Subsystems"),
		                                                                  NSLOCTEXT("YourModule",
			                                                                  "BlueprintSubsystemSettingsDesc",
			                                                                  "Add Blueprint Subsystems"),
		                                                                  GetMutableDefault<
			                                                                  UBlueprintSubsystemSettings>());
			SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(
				this, &FBlueprintableSubsystemModule::HandleBlueprintSubsystemSettingsSaved);


			UBlueprintSubsystemSettings* Settings = GetMutableDefault<UBlueprintSubsystemSettings>();
			for (const auto& BlueprintSubsystem : Settings->BlueprintSubsystems)
			{
				if (!IsValid(BlueprintSubsystem)) continue;
				const auto LoadObject = StaticLoadObject(BlueprintSubsystem->StaticClass(),
				                                         nullptr, *BlueprintSubsystem->GetPathName());
				UE_LOG(LogTemp, Warning, TEXT("Load Blueprint Derived GameInstance Subsystem: %s"),
				       *LoadObject->GetName());
			}
			TArray<int32> Indexes;
			// find invalid entries
			for (int i = 0; i < Settings->BlueprintSubsystems.Num(); ++i)
			{
				if (!IsValid(Settings->BlueprintSubsystems[i]))
				{
					Indexes.Add(i);
				}
			}
			//remove invalid entries from settings
			for (const auto Index : Indexes)
			{
				Settings->BlueprintSubsystems.RemoveAt(Index);
			}
		}
	}
}

void FBlueprintableSubsystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Subsystems");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBlueprintableSubsystemModule, BlueprintableSubsystem)
