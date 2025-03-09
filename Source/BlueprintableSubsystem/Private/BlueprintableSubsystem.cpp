// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintableSubsystem.h"

#include "BlueprintSubsystemSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FBlueprintableSubsystemModule"


bool FBlueprintableSubsystemModule::HandleBlueprintSubsystemSettingsSaved()
{
	UBlueprintSubsystemSettings* Settings = GetMutableDefault<UBlueprintSubsystemSettings>();

	//add your validation here. by default, it will resave
	bool Resave = true;

	// value has been entered
	if (Resave)
	{
		Settings->SaveConfig();
		Settings->TryUpdateDefaultConfigFile();
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
				OnRestartNotificationClickedDelegate.ExecuteIfBound();
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
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (const auto SettingsSection = SettingsModule->RegisterSettings("Project", "Game", "Subsystems",
		                                                                  NSLOCTEXT("BlueprintableSubsystem",
			                                                                  "BlueprintSubsystemSettingsName",
			                                                                  "Blueprintable Subsystem"),
		                                                                  NSLOCTEXT("BlueprintableSubsystem",
			                                                                  "BlueprintSubsystemSettingsDesc",
			                                                                  "Configure Blueprintable Subsystems"),
		                                                                  GetMutableDefault<
			                                                                  UBlueprintSubsystemSettings>());
			SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(
				this, &FBlueprintableSubsystemModule::HandleBlueprintSubsystemSettingsSaved);


			UBlueprintSubsystemSettings* Settings = GetMutableDefault<UBlueprintSubsystemSettings>();
			for (const auto& BlueprintSubsystem : Settings->BlueprintSubsystems)
			{
				if (!IsValid(BlueprintSubsystem.LoadSynchronous())) continue;
				const auto LoadObject = StaticLoadObject(BlueprintSubsystem->StaticClass(),
				                                         nullptr, *BlueprintSubsystem->GetPathName());
				UE_LOG(LogTemp, Warning, TEXT("Load Blueprint Derived GameInstance Subsystem: %s"),
				       *LoadObject->GetName());
			}
			TArray<int32> Indexes;

			// find invalid entries
			for (int i = Settings->BlueprintSubsystems.Num() - 1; i >= 0; --i)
			{
				if (!IsValid(Settings->BlueprintSubsystems[i].LoadSynchronous()))
				{
					//remove invalid entries from settings
					Settings->BlueprintSubsystems.RemoveAt(i);
				}
			}
		}
	}
}

void FBlueprintableSubsystemModule::ShutdownModule()
{
	// we call this function before unloading the module.
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Subsystems");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBlueprintableSubsystemModule, BlueprintableSubsystem)
