using UnrealBuildTool;
using System.Collections.Generic;

public class SwapEditorTarget : TargetRules
{
	public SwapEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Swap", "SwapEditor" } );
	}
}
