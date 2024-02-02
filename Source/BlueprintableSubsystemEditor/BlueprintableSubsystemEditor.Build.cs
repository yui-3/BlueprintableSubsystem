using UnrealBuildTool;

public class BlueprintableSubsystemEditor : ModuleRules
{
	public BlueprintableSubsystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", "EditorScriptingUtilities", "Blutility", "BlueprintableSubsystem", "UnrealEd"
			}
		);
	}
}