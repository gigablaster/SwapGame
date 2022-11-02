using UnrealBuildTool;
using System.Collections.Generic;

public class SwapTarget : TargetRules
{
	public SwapTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Swap" } );
	}
}
