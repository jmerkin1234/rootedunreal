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

## Plan Progress
- [x] Phase 1: apply SM5 renderer + physics baseline in `Config/DefaultEngine.ini`.
- [x] Phase 1: create `Content/Billiards` folder scaffold.
- [x] Phase 1: disable Interchange plugins in `rootedunreal.uproject`.
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
- Disable Interchange import plugins before importing FBX assets to preserve `UCX_` collision meshes.

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
- Added `Config/DefaultEngine.ini` with SM5/GTX1080-safe renderer baseline and physics baseline.
- Enabled fixed framerate baseline (`120.0`) for billiards determinism.
- Created `Content/Billiards` folder structure for Meshes/Materials/Instances/Physics/Blueprints/UI/Enums/Levels/DataAssets/Sounds/VFX.
- Added `.gitkeep` placeholders so scaffold folders are tracked.
- Disabled `Interchange` and `InterchangeEditor` plugins in `rootedunreal.uproject` for UCX-safe FBX import workflow.

## Validation Performed
- Confirmed config values written in `Config/DefaultEngine.ini`.
- Confirmed `Content/Billiards/...` directories exist.
- Confirmed plugin files exist in this engine build: `Interchange.uplugin`, `InterchangeEditor.uplugin`.
- Pushed commits to `origin/main`.
