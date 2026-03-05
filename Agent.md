# rootedunreal Agent Log

## Project Identity
- Project name: `rootedunreal`
- Project root: `/home/justin/UnrealEngine/rootedunreal`
- Unreal project file: `/home/justin/UnrealEngine/rootedunreal/rootedunreal.uproject`

## Hard Constraints
- Build as a fresh project under `/home/justin/UnrealEngine/rootedunreal`.
- Do not pull gameplay logic or assets from old billiards projects.
- Use only the provided guide and provided export/texture sources.

## Source Inputs
- Guide: `/home/justin/Downloads/Billiards_Game_Plan_v3.docx`
- Export FBX folder (5 files): `/home/justin/Desktop/BilliardsExport`
- Ball textures folder: `/home/justin/Desktop/billiard assets/wip`

## Git
- Branch: `main`
- Remote `origin`: `git@github.com:jmerkin1234/rootedunreal.git`

## Guide Section Status
- [x] **2.1 Create the Project**: converted to C++ project layout (module + target files under `Source/`).
- [x] **2.2 Project Settings**: physics/framerate values applied in `Config/DefaultEngine.ini`.
- [x] **2.3 Content Browser Folder Structure**: created under `Content/Billiards/`:
  - `Meshes`, `Materials`, `Instances`, `Physics`
  - `Blueprints/Actors`, `Blueprints/GameMode`, `Blueprints/AI`
  - `UI`, `Enums`, `Levels`, `DataAssets`, `Sounds`, `VFX`

## Setup Checklist (Guide-Aligned)
- [x] Create project as C++ (module + targets under `Source/`)
- [x] Disable starter content baseline (project scaffold only)
- [x] Target desktop/linux profile in config
- [x] Enable `EnhancedInput` plugin
- [x] Keep Interchange plugins disabled before FBX import (`Interchange`, `InterchangeEditor`)
- [x] Set autosave to 5 minutes (`Config/DefaultEditorPerProjectUserSettings.ini`)
- [x] Apply physics settings from plan
- [x] Apply fixed framerate settings from plan
- [x] Compile-check `rootedunrealEditor` successfully

## Plan Progress
- [x] Phase 1: SM5 renderer + physics/framerate baseline in `Config/DefaultEngine.ini`.
- [x] Phase 1: C++ project module/target scaffolding.
- [x] Phase 1: `Content/Billiards` folder scaffold.
- [x] Phase 1: plugin configuration (`EnhancedInput` on, Interchange off).
- [ ] Phase 2: import 5 FBX assets and validate cushion `UCX_` collisions.
- [ ] Phase 2: create `PM_Felt`, `PM_Cushion`, `PM_Ball` and assign defaults.
- [ ] Phase 3: create core enums and Blueprint class skeletons.

## Import Baseline
- FBX import order:
  1. `Pool_Cushions.fbx`
  2. `Pool_Felt.fbx`
  3. `Pool_Table.fbx`
  4. `Pool_Balls.fbx`
  5. `Pool_Cue.fbx`
- Do not import until explicit go-ahead.

## Update Rule
Update this file after each meaningful change with:
- Date (`YYYY-MM-DD`)
- What changed (files/settings)
- Validation actually run

## Change Log
### 2026-03-05
- Created fresh `rootedunreal` project scaffold and git baseline.
- Added source references and workflow constraints in `Agent.md`.
- Moved active repo to `/home/justin/UnrealEngine/rootedunreal`.
- Archived previous folder to `/home/justin/UnrealEngine/rootedunreal_backup_20260305_034955` before move.
- Added `README.md` and pushed branch `main` to `origin`.
- Connected git remote: `git@github.com:jmerkin1234/rootedunreal.git`.
- Verified Unreal MCP connectivity with successful actor queries.
- Added `Config/DefaultEngine.ini` with SM5 renderer baseline and plan-aligned physics/framerate values.
- Enabled fixed framerate (`120.0`) for determinism.
- Created `Content/Billiards` folder structure for Meshes/Materials/Instances/Physics/Blueprints/UI/Enums/Levels/DataAssets/Sounds/VFX.
- Added `.gitkeep` placeholders so scaffold folders are tracked.
- Added C++ project files in `Source/`:
  - `rootedunreal.Target.cs`
  - `rootedunrealEditor.Target.cs`
  - `rootedunreal/rootedunreal.Build.cs`
  - `rootedunreal/rootedunreal.h`
  - `rootedunreal/rootedunreal.cpp`
- Updated `.uproject` to include runtime module and `EnhancedInput` plugin.
- Added `Config/DefaultEditorPerProjectUserSettings.ini` with autosave interval at 5 minutes.
- Updated status tracking to explicitly show guide section numbers 2.1, 2.2, and 2.3.

## Validation Performed
- Confirmed guide path exists: `/home/justin/Downloads/Billiards_Game_Plan_v3.docx`.
- Confirmed C++ module/target files exist in `Source/`.
- Built editor target successfully:
  - Command: `Build.sh rootedunrealEditor Linux Development ...`
  - Result: `Succeeded`
- Confirmed `Config/DefaultEngine.ini` includes requested settings:
  - `DefaultGravityZ=-980.0`
  - `BounceThresholdVelocity=200.0`
  - `FrictionCombineMode=Average`
  - `RestitutionCombineMode=Average`
  - `MaxDepenetrationVelocity=500.0`
  - `SolverOptions.PositionIterations=8`
  - `SolverOptions.VelocityIterations=4`
  - `bUseFixedFrameRate=True`
  - `FixedFrameRate=120.0`
