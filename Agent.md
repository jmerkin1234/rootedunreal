# rootedunreal Agent Log

## Project Identity
- Project name: `rootedunreal`
- Project root: `/home/justin/UnrealEngine/rootedunreal`
- Unreal project file: `/home/justin/UnrealEngine/rootedunreal/rootedunreal.uproject`
- Engine path used: `/unrealengine/Engine/Binaries/Linux/UnrealEditor`
- Platform target: Desktop/Linux only

## Hard Constraints
- Follow `/home/justin/Downloads/Billiards_Game_Plan_v3.docx` step-by-step.
- Keep project Blueprint-only per guide section 2.1 (`C++=NO`).
- Keep Interchange disabled before any import.
- Run import/setup actions one step at a time with verification before the next step.

## Source Inputs
- Guide: `/home/justin/Downloads/Billiards_Game_Plan_v3.docx`
- FBX export folder (5 files): `/home/justin/Desktop/BilliardsExport`
- Ball textures folder: `/home/justin/Desktop/billiard assets/wip`

## Git
- Branch: `main`
- Remote `origin`: `git@github.com:jmerkin1234/rootedunreal.git`

## Guide Verification Status

### Phase 1 (Section 1)
- [x] **1.1 DefaultEngine.ini renderer baseline applied**
  - `r.Nanite=0`
  - `r.RayTracing=0`
  - `r.HardwareRayTracing=0`
  - `r.Lumen.HardwareRayTracing=0`
  - `r.Lumen.Reflections.HardwareRayTracing=0`
  - `r.DynamicGlobalIlluminationMethod=1`
  - `r.ReflectionMethod=1`
  - `r.Shadow.Virtual.Enable=0`
  - `r.AntiAliasingMethod=2` (TAA)
  - Linux RHI: `+TargetedRHIs=SF_VULKAN_SM5`, `-TargetedRHIs=SF_VULKAN_SM6`
- [x] **1.2 Project rendering expectations aligned via config**
  - Lumen software path + shadow maps + no Nanite + no ray tracing in config.

### Section 2 (Creation & Setup)
- [x] **2.1 Project type and baseline**
  - `.uproject` has **no `Modules` block** (Blueprint-only).
  - `EnhancedInput` enabled.
  - Fixed framerate + physics baseline present in config.
- [x] **2.2 Interchange disabled (critical)**
  - `Interchange` = disabled
  - `InterchangeEditor` = disabled
- [x] **2.3 Content Browser folder tree created**
  - `Content/Billiards/Meshes`
  - `Content/Billiards/Materials`
  - `Content/Billiards/Instances`
  - `Content/Billiards/Physics`
  - `Content/Billiards/Blueprints/Actors`
  - `Content/Billiards/Blueprints/GameMode`
  - `Content/Billiards/Blueprints/AI`
  - `Content/Billiards/Blueprints/UI`
  - `Content/Billiards/Enums`
  - `Content/Billiards/Levels`
  - `Content/Billiards/DataAssets`
  - `Content/Billiards/Sounds`
  - `Content/Billiards/VFX`

### Section 3 (FBX Import & Collision Setup)
- [x] **3.1 Legacy FBX pipeline enforced**
  - Interchange stayed disabled in project plugins.
  - Import scripts use legacy `FbxFactory` path.
- [x] **3.2 Mesh imports completed from `/home/justin/Desktop/BilliardsExport`**
  - `Pool_Cushions.fbx`
  - `Pool_Felt.fbx`
  - `Pool_Table.fbx`
  - `Pool_Balls.fbx` (imported with `remove_degenerates=False` workaround due degenerate-poly rejection)
  - `Pool_Cue.fbx`
- [x] **3.3 Cushion UCX collision verified**
  - Verified convex collision present on all six cushion meshes.
- [x] **3.4 Post-import collision policy applied**
  - Balls `0-15`: sphere simple collision.
  - Cue: no collision.
  - Felt: complex-as-simple.
  - Pockets: no collision.
  - Table base/frame: no collision.

### Section 4 (Physical Materials)
- [x] **4.2 PM_Felt created**
  - `Friction=0.20`, `StaticFriction=0.22`, `Restitution=0.0`
- [x] **4.3 PM_Cushion created**
  - `Friction=0.05`, `StaticFriction=0.05`, `Restitution=0.65`
- [x] **4.4 PM_Ball created**
  - `Friction=0.04`, `StaticFriction=0.04`, `Restitution=0.65`, `Density=1700`

### Section 5 (Custom Collision Channel)
- [x] **5.1 BallCollision trace channel added in project config**
  - `DefaultResponse=Ignore`
- [ ] **5.2 Component response wiring pending**
  - BP_Ball response to `BallCollision=Block` will be applied when BP_Ball is created in later guide steps.

## MCP Status
- UnrealMCP plugin installed at:
  - `/home/justin/UnrealEngine/rootedunreal/Plugins/UnrealMCP`
- Enabled in `.uproject` as `"UnrealMCP": true`.
- Live connection confirmed during this session (`get_actors_in_level` success).
- Operating rule: keep MCP calls paced (single call, wait for completion).

## Current Phase Gate
- Sections 3, 4, and 5.1 are complete.
- Next strict-order step: Section 6 (create all required enums).

## Change Log
### 2026-03-05
- Corrected project from prior C++ state to Blueprint-only state (removed project module declaration and Source tree).
- Installed and enabled UnrealMCP plugin in project.
- Enforced desktop/Linux-only baseline and removed non-required config drift.
- Normalized content tree to exact Section 2.3 structure.
- Removed off-plan generated `Content/Blueprints` assets from earlier attempts.
- Imported all 5 FBX exports with legacy importer path.
- Cleaned and re-imported cushion meshes after detecting an Interchange-path import attempt.
- Verified UCX collisions on cushions with updated convex-aware validator.
- Applied and verified Section 3.4 collision rules across imported meshes.
- Created all Section 4 physical materials with target values.
- Added Section 5.1 `BallCollision` custom trace channel to project config.
- Pushed project state to GitHub `main` at commit `446946a`.
- Removed open-level `Landscape` and all `LandscapeStreamingProxy` actors via MCP cleanup pass.

## Verification Commands Used
- Config check: `sed -n` on `Config/DefaultEngine.ini`
- Project type/plugins: `sed -n` on `rootedunreal.uproject`
- Folder tree: `find Content/Billiards -maxdepth 3 -type d | sort`
- MCP bridge validation: UnrealMCP command `get_actors_in_level`
- 2026-03-05: Disabled uncontrolled changelist discovery to reduce startup scan/stutter.
  - Added to `Config/DefaultEditor.ini`:
    - `[/Script/SourceControl.SourceControlPreferences]`
    - `bEnableUncontrolledChangelists=False`
  - Added same override to local runtime file: `Saved/Config/LinuxEditor/Editor.ini`.
  - Note: Unreal Editor restart is required for this setting to fully take effect.
- 2026-03-05 (post-restart verification):
  - MCP heartbeat succeeded (`get_actors_in_level`).
  - No `Uncontrolled asset discovery` entries found in current `Saved/Logs/rootedunreal.log` after disabling uncontrolled changelists.
- 2026-03-05 (user direction lock):
  - Follow the entire guide in exact written order (all steps), no reordering.
  - Do not skip or front-run any step; complete and verify each step before the next.
  - Do not use Interchange importer for FBX in this project.
  - Split multi-step operations into small units (one import/action at a time) with verification between each step.
  - Keep MCP pace low (single action, wait for completion).
- 2026-03-05 (Section 3 execution and verification):
  - Import runner: `/unrealengine/tools/import_one_fbx.py`
  - UCX verify: `/unrealengine/tools/verify_cushion_ucx.py`
  - Collision post-pass: `/unrealengine/tools/post_import_collision_setup.py`
  - Final collision verification from `/tmp/rootedunreal_post_collision.log`:
    - `no collision: Pool_Table_table_base`
    - `no collision: Pool_Table_table_frame`
    - `post-import collision setup complete; touched=26`
- 2026-03-05 (Section 4 execution and verification):
  - Material creation runner: `/unrealengine/tools/create_phase1_physical_materials.py`
  - Verification log: `/tmp/rootedunreal_phase1_pm.log`
  - Verified created assets:
    - `/Game/Billiards/Physics/PM_Felt`
    - `/Game/Billiards/Physics/PM_Cushion`
    - `/Game/Billiards/Physics/PM_Ball`
- 2026-03-05 (live editor cleanup):
  - MCP `find_actors_by_name("Landscape")` initially returned `Landscape` + 64 `LandscapeStreamingProxy` actors.
  - Deleted all landscape actors.
  - Final verification: `find_actors_by_name("Landscape")` returned `0` actors.
