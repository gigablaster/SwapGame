using UnrealBuildTool;

public class SwapEditor : ModuleRules
{
	public SwapEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Swap",
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"ComponentVisualizers",
			"UnrealEd",
			"Slate",
			"SlateCore",
			"EditorStyle"
		});   
	}
}