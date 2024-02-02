// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBlueprintableSubsystemModule : public IModuleInterface
{
	bool HandleBlueprintSubsystemSettingsSaved();
	TWeakPtr<SNotificationItem> NotificationPtr;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
