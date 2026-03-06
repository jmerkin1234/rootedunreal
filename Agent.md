# rootedunreal Agent Log

## Resume Token
- CodexResumeID: `ROOTEDUNREAL-20260305-ADD17C6`
- Branch: `phase1-step4-7`
- Commit: `add17c6`
- Resume from: `Section 3 — FBX Import Procedure` (strict in-order plan execution)

## Project Identity
- Project name: `rootedunreal`
- Project root: `/home/justin/UnrealEngine/rootedunreal`
- Unreal project file: `/home/justin/UnrealEngine/rootedunreal/rootedunreal.uproject`
- Engine path used: `/unrealengine/Engine/Binaries/Linux/UnrealEditor`
- Platform target: Desktop/Linux only

## Hard Constraints
- Follow `/home/justin/Downloads/Billiards_Game_Plan_v4.docx` step-by-step.
- Project is now C++-capable (runtime module added) while still following the same gameplay plan order.
- Keep Interchange disabled before any import.
- Run import/setup actions one step at a time with verification before the next step.

## Source Inputs
- Active guide: `/home/justin/Downloads/Billiards_Game_Plan_v4.docx`
- Superseded guide: `/home/justin/Downloads/Billiards_Game_Plan_v3.docx` (do not use for new steps)
- FBX export folder (5 files): `/home/justin/Desktop/BilliardsExport`
- Ball textures folder: `/home/justin/Desktop/billiard assets/wip`

## Git
- Branch: `phase1-step4-7`
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
  - `.uproject` includes runtime module `rootedunreal` (C++-capable project).
  - `Source/rootedunreal` module and `rootedunrealEditor` target exist and build.
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
  - MCP `import_fbx` now uses safe delete-and-import flow for `replace_existing=true` (avoids UE reimport crash path).
- [ ] **3.2 Mesh imports in strict order**
  - Verified present right now: `Pool_Felt.fbx`, `Pool_Table.fbx` assets.
  - Pending re-run in exact v4 order: `Pool_Cushions.fbx` → `Pool_Felt.fbx` → `Pool_Table.fbx` → `Pool_Balls.fbx` → `Pool_Cue.fbx`.
- [ ] **3.3 Cushion UCX collision verification**
  - Blocked until cushion meshes are imported in current pass.
- [ ] **3.4 Post-import collision policy**
  - To run immediately after all 5 files are imported and verified.

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
- Live connection confirmed during this session (`ping` success).
- MCP command `create_enum` is now implemented in plugin source, compiled, and verified after editor restart.
- MCP command `import_fbx` implemented and verified for `Pool_Felt.fbx` without editor crash.
- Operating rule: keep MCP calls paced (single call, wait for completion).

## Current Phase Gate
- Section 3 is active and must be completed in strict import order before proceeding.
- Next strict-order step: Section 3.2 step 1 (`Pool_Cushions.fbx` import), then Section 3.3 UCX verification.

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
- 2026-03-06: Plan source switched to v4 (`/home/justin/Downloads/Billiards_Game_Plan_v4.docx`).
  - v4 introduces additional/updated logic in later sections (spin vector, rack Z verification note, foul handling extensions, cue-ball post-contact direction, and AI shot scoring penalty).
  - Execution remains strict in-order from current checkpoint.
- 2026-03-06: Project converted back to C++-capable by request.
  - Added `Source/rootedunreal` runtime module (`rootedunreal.Build.cs`, `rootedunreal.cpp`, `rootedunreal.h`).
  - Added C++ targets: `Source/rootedunreal.Target.cs`, `Source/rootedunrealEditor.Target.cs`.
  - Updated `rootedunreal.uproject` with `Modules` block for `rootedunreal`.
  - Verified compile success: `rootedunrealEditor` build completed with `Result: Succeeded`.
- 2026-03-06: UnrealMCP extended with new blueprint command `create_enum`.
  - Files updated:
    - `Plugins/UnrealMCP/Source/UnrealMCP/Public/Commands/EpicUnrealMCPBlueprintCommands.h`
    - `Plugins/UnrealMCP/Source/UnrealMCP/Private/Commands/EpicUnrealMCPBlueprintCommands.cpp`
    - `Plugins/UnrealMCP/Source/UnrealMCP/Private/EpicUnrealMCPBridge.cpp`
  - Live verification after restart:
    - MCP `ping` returned `pong`.
    - MCP `create_enum` succeeded for `/Game/Billiards/Enums/E_MCP_Verify_20260306`.
  - Temporary probe enum asset was removed after verification to keep content clean.
- 2026-03-06: Asset integrity/fix pass rerun on imported meshes.
  - Ran `verify_cushion_ucx.py`: all six cushion meshes report `convex=1` and UCX verification passed.
  - Ran `post_import_collision_setup.py`: reapplied Section 3.4 collision rules across 26 meshes.
  - Cleaned regenerated off-plan files:
    - `Content/Untitled.umap`
    - `Content/Untitled_HLOD0_Instancing.uasset`
    - `Content/__ExternalActors__/Untitled`
    - `Content/__ExternalObjects__/Untitled`
    - `Content/Blueprints/` (root-level off-plan folder)
- 2026-03-06: Placed imported meshes into active `Untitled` level via MCP `spawn_actor`.
  - Actor prefix used: `PL_`
  - Placed set:
    - Table assembly meshes (base/frame/felt/cushions/pockets)
    - Cue mesh
    - Ball meshes `Pool_Balls_ball0` through `Pool_Balls_ball15`
  - Ball placement layout: 8x2 grid for visibility/selection.
- 2026-03-06: Created persistent preview level to avoid temporary `Untitled` visibility issues.
  - New level asset: `/Game/Billiards/Levels/L_AssetPreview`
  - Level save verified (`saved=True`) with 32 placed actors.
  - Actor label prefix in this level: `LV_`
- 2026-03-06: Created Untitled-style lit preview level per visibility request.
  - New level asset: `/Game/Billiards/Levels/L_AssetPreview_Lit`
  - Created from template: `/Engine/Maps/Templates/OpenWorld`
  - Landscape actors removed after template creation; billiards assets placed with `LV_` labels.
  - Level save verified (`saved=True`), actor_count=105 (template lighting/environment + placed assets).
- 2026-03-06: Verification-first correction pass (v4 Section 3 focus).
  - Removed Android runtime block from `Config/DefaultEngine.ini` to keep project desktop/Linux only.
  - Patched MCP `import_fbx` to pre-delete matching destination assets when `replace_existing=true`, then clean-import with legacy `FbxFactory` settings.
  - Live socket verification:
    - `ping` returned `pong`.
    - `import_fbx` on `Pool_Felt.fbx` completed without crashing the editor and recreated `Pool_Felt.uasset`.
  - Section 3 status reset to in-progress to match current content state and strict order requirement.

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
- 2026-03-06 (C++ + MCP verification):
  - Build command: `"/unrealengine/Engine/Build/BatchFiles/Linux/Build.sh" rootedunrealEditor Linux Development "/home/justin/UnrealEngine/rootedunreal/rootedunreal.uproject" -waitmutex`
  - MCP live checks via socket on `127.0.0.1:55557`:
    - `{"type":"ping","params":{}}`
    - `{"type":"create_enum","params":{"name":"E_MCP_Verify_20260306","path":"/Game/Billiards/Enums","values":["Pending","Active","Done"]}}`
- 2026-03-06 (asset fix verification):
  - UCX verify log: `/tmp/rootedunreal_verify_ucx_now.log`
  - Collision pass log: `/tmp/rootedunreal_post_collision_now.log`
- 2026-03-06 (pre-reexport dual audit: Blender + UE):
  - Blender audit artifacts:
    - Script: `/tmp/blender_billiards_audit.py`
    - JSON: `/tmp/blender_billiards_audit.json`
    - Log: `/tmp/blender_billiards_audit.log`
  - Blender checks:
    - PASS: table/felt/cushions/cue/balls present
    - PASS: UCX cushion collision objects present (`UCX_cushion_*`)
    - PASS: transforms applied (scale/rotation)
    - PASS: source dimensions at target scale (felt ~224x112 cm, ball diameter ~5.715 cm)
    - FAIL (expected for collision-only meshes): UV check flags only `UCX_cushion_*` objects (6)
  - UE audit artifacts:
    - Script: `/tmp/rootedunreal_asset_audit.py`
    - JSON: `/tmp/rootedunreal_asset_audit.json`
    - Text: `/tmp/rootedunreal_asset_audit.txt`
    - Log: `/tmp/rootedunreal_asset_audit_run.log`
  - UE checks:
    - PASS: `cushion_ucx_convex_present`
    - PASS: `balls_simple_sphere_collision_only`
    - PASS: `cue_no_collision`
    - PASS: `felt_complex_as_simple`
    - FAIL: felt size target check (measured ~2.24x1.12 cm instead of 224x112 cm)
    - FAIL: ball diameter target check (measured ~0.057 cm instead of 5.715 cm)
  - Root cause verification (UE import-data):
    - Probe logs: `/tmp/rootedunreal_import_scale_probe.log`, `/tmp/rootedunreal_import_scale_probe2.log`
    - Bulk import settings: `/tmp/rootedunreal_import_settings_audit.json`
    - All 32 static meshes show `FbxStaticMeshImportData`: `import_uniform_scale=1.0`, `convert_scene=True`, `convert_scene_unit=False`
    - Conclusion: meshes are imported at ~1/100 scale in UE from current FBX/unit pipeline.
- 2026-03-06 (Blender re-export overwrite, same filenames):
  - Source blend: `/home/justin/Desktop/billiard assets/models/pool-table-cycles/Pool-Table-Jay-Hardy1.blend`
  - Target folder: `/home/justin/Desktop/BilliardsExport`
  - Overwritten files:
    - `Pool_Table.fbx`
    - `Pool_Felt.fbx`
    - `Pool_Cushions.fbx`
    - `Pool_Balls.fbx`
    - `Pool_Cue.fbx`
  - Automation script: `/tmp/blender_reexport_overwrite.py`
  - Run log: `/tmp/blender_reexport_overwrite.log`
  - Export summary: `/tmp/blender_reexport_overwrite_summary.json`
  - Export used UE-targeted FBX settings:
    - `global_scale=1.0`
    - `apply_unit_scale=True`
    - `apply_scale_options=FBX_SCALE_UNITS`
    - `axis_forward=-Z`, `axis_up=Y`
    - `use_selection=True`
    - `bake_space_transform=True`
- 2026-03-06 (pre-import cleanup per user direction):
  - Verified new FBX were exported to `/home/justin/Desktop/BilliardsExport` and had newer mtimes than UE mesh assets.
  - Removed old imported mesh assets from `/Game/Billiards/Meshes` before any reimport.
  - Deletion runner: `/tmp/rootedunreal_delete_old_mesh_assets.py`
  - Deletion log: `/tmp/rootedunreal_delete_old_mesh_assets.log`
  - Result: `target_count=32`, `deleted=32`, `failed=0`.
  - Scope removed: all `Pool_Table_*`, `Pool_Felt`, `Pool_Cushions_*`, `Pool_Balls_ball0..15`, and `Pool_Cue` static mesh assets.
- 2026-03-06 (live MCP level cleanup in `L_AssetPreview_Lit`):
  - User-reported stale preview actors were still present in level despite mesh asset deletion.
  - Found stale actor set by pattern: `StaticMeshActor_UAID_107B441A87B625C502_*` (32 actors).
  - Deleted all 32 actors via live MCP (`delete_actor`), then verified pattern search returned zero.
  - Verified no `LV_` actor names found via MCP search and no `LV_`-prefixed content asset paths in `Content/`.
- 2026-03-06 (save-state checkpoint: full Blender audit + clean FBX redo):
  - Full Blender audit run against source blend:
    - `/home/justin/Desktop/billiard assets/models/pool-table-cycles/Pool-Table-Jay-Hardy1.blend`
    - Reports copied to `/home/justin/Desktop/BilliardsExport`:
      - `blender_full_audit_report.md`
      - `blender_full_audit_report.json`
  - Findings:
    - UCX pairing present for all 6 cushions.
    - Ball diameters within tolerance (~5.715 cm target).
    - Root issue confirmed: object-level placement/pivot data caused baked export offsets in UE workflow.
  - Clean re-export pipeline executed in Blender (temporary clean-copy objects with corrected pivots, then FBX export).
  - Fresh 5 FBX generated and verified by round-trip re-import audit:
    - `Pool_Table.fbx`
    - `Pool_Felt.fbx`
    - `Pool_Cushions.fbx`
    - `Pool_Balls.fbx`
    - `Pool_Cue.fbx`
    - Verify report: `/home/justin/Desktop/BilliardsExport/verify_report.json`
    - Export metadata: `/home/justin/Desktop/BilliardsExport/reexport_report.json`
  - Broken FBX files in `/home/justin/Desktop/BilliardsExport` were overwritten with corrected versions at ~06:03 local.
  - Next resume step: re-import these 5 corrected FBX in strict Section 3 order and re-run Section 3.3 UCX verification immediately after cushions.
