using UnrealBuildTool;

public class TestProjectEditor : ModuleRules
{
	public TestProjectEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
				"SlateCore",
				"Slate",
				"UnrealEd",
				"LevelEditor"
		});
	}
}