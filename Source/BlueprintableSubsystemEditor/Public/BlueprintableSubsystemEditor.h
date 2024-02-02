#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBlueprintableSubsystemEditorModule : public IModuleInterface
{
	void HandleNewBlueprintableSubsystemBlueprint(UBlueprint* Blueprint);

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
