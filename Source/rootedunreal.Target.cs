using UnrealBuildTool;
using System.Collections.Generic;

public class rootedunrealTarget : TargetRules
{
    public rootedunrealTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.Add("rootedunreal");
    }
}
