#include "Commands/EpicUnrealMCPEditorCommands.h"
#include "Commands/EpicUnrealMCPCommonUtils.h"
#include "Editor.h"
#include "EditorViewportClient.h"
#include "LevelEditorViewport.h"
#include "ImageUtils.h"
#include "HighResScreenshot.h"
#include "Engine/GameViewportClient.h"
#include "Misc/FileHelper.h"
#include "GameFramework/Actor.h"
#include "Engine/Selection.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Camera/CameraActor.h"
#include "Components/StaticMeshComponent.h"
#include "EditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "EditorAssetLibrary.h"
#include "Commands/EpicUnrealMCPBlueprintCommands.h"

#include "AssetToolsModule.h"
#include "AssetImportTask.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"
#include "Factories/FbxMeshImportData.h"
#include "Factories/FbxFactory.h"
#include "FileHelpers.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"

FEpicUnrealMCPEditorCommands::FEpicUnrealMCPEditorCommands()
{
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleCommand(const FString& CommandType, const TSharedPtr<FJsonObject>& Params)
{
    // Actor manipulation commands
    if (CommandType == TEXT("get_actors_in_level"))
    {
        return HandleGetActorsInLevel(Params);
    }
    else if (CommandType == TEXT("find_actors_by_name"))
    {
        return HandleFindActorsByName(Params);
    }
    else if (CommandType == TEXT("spawn_actor"))
    {
        return HandleSpawnActor(Params);
    }
    else if (CommandType == TEXT("delete_actor"))
    {
        return HandleDeleteActor(Params);
    }
    else if (CommandType == TEXT("set_actor_transform"))
    {
        return HandleSetActorTransform(Params);
    }
    // Blueprint actor spawning
    else if (CommandType == TEXT("spawn_blueprint_actor"))
    {
        return HandleSpawnBlueprintActor(Params);
    }
    else if (CommandType == TEXT("import_fbx"))
    {
        return HandleImportFbx(Params);
    }
    
    return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("Unknown editor command: %s"), *CommandType));
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleGetActorsInLevel(const TSharedPtr<FJsonObject>& Params)
{
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GWorld, AActor::StaticClass(), AllActors);
    
    TArray<TSharedPtr<FJsonValue>> ActorArray;
    for (AActor* Actor : AllActors)
    {
        if (Actor)
        {
            ActorArray.Add(FEpicUnrealMCPCommonUtils::ActorToJson(Actor));
        }
    }
    
    TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
    ResultObj->SetArrayField(TEXT("actors"), ActorArray);
    
    return ResultObj;
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleFindActorsByName(const TSharedPtr<FJsonObject>& Params)
{
    FString Pattern;
    if (!Params->TryGetStringField(TEXT("pattern"), Pattern))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'pattern' parameter"));
    }
    
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GWorld, AActor::StaticClass(), AllActors);
    
    TArray<TSharedPtr<FJsonValue>> MatchingActors;
    for (AActor* Actor : AllActors)
    {
        if (Actor && Actor->GetName().Contains(Pattern))
        {
            MatchingActors.Add(FEpicUnrealMCPCommonUtils::ActorToJson(Actor));
        }
    }
    
    TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
    ResultObj->SetArrayField(TEXT("actors"), MatchingActors);
    
    return ResultObj;
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleSpawnActor(const TSharedPtr<FJsonObject>& Params)
{
    // Get required parameters
    FString ActorType;
    if (!Params->TryGetStringField(TEXT("type"), ActorType))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'type' parameter"));
    }

    // Get actor name (required parameter)
    FString ActorName;
    if (!Params->TryGetStringField(TEXT("name"), ActorName))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'name' parameter"));
    }

    // Get optional transform parameters
    FVector Location(0.0f, 0.0f, 0.0f);
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FVector Scale(1.0f, 1.0f, 1.0f);

    if (Params->HasField(TEXT("location")))
    {
        Location = FEpicUnrealMCPCommonUtils::GetVectorFromJson(Params, TEXT("location"));
    }
    if (Params->HasField(TEXT("rotation")))
    {
        Rotation = FEpicUnrealMCPCommonUtils::GetRotatorFromJson(Params, TEXT("rotation"));
    }
    if (Params->HasField(TEXT("scale")))
    {
        Scale = FEpicUnrealMCPCommonUtils::GetVectorFromJson(Params, TEXT("scale"));
    }

    // Create the actor based on type
    AActor* NewActor = nullptr;
    UWorld* World = GEditor->GetEditorWorldContext().World();

    if (!World)
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Failed to get editor world"));
    }

    // Check if an actor with this name already exists
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);
    for (AActor* Actor : AllActors)
    {
        if (Actor && Actor->GetName() == ActorName)
        {
            return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("Actor with name '%s' already exists"), *ActorName));
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = *ActorName;

    if (ActorType == TEXT("StaticMeshActor"))
    {
        AStaticMeshActor* NewMeshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Location, Rotation, SpawnParams);
        if (NewMeshActor)
        {
            // Check for an optional static_mesh parameter to assign a mesh
            FString MeshPath;
            if (Params->TryGetStringField(TEXT("static_mesh"), MeshPath))
            {
                UStaticMesh* Mesh = Cast<UStaticMesh>(UEditorAssetLibrary::LoadAsset(MeshPath));
                if (Mesh)
                {
                    NewMeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Could not find static mesh at path: %s"), *MeshPath);
                }
            }
        }
        NewActor = NewMeshActor;
    }
    else if (ActorType == TEXT("PointLight"))
    {
        NewActor = World->SpawnActor<APointLight>(APointLight::StaticClass(), Location, Rotation, SpawnParams);
    }
    else if (ActorType == TEXT("SpotLight"))
    {
        NewActor = World->SpawnActor<ASpotLight>(ASpotLight::StaticClass(), Location, Rotation, SpawnParams);
    }
    else if (ActorType == TEXT("DirectionalLight"))
    {
        NewActor = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), Location, Rotation, SpawnParams);
    }
    else if (ActorType == TEXT("CameraActor"))
    {
        NewActor = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Location, Rotation, SpawnParams);
    }
    else
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("Unknown actor type: %s"), *ActorType));
    }

    if (NewActor)
    {
        // Set scale (since SpawnActor only takes location and rotation)
        FTransform Transform = NewActor->GetTransform();
        Transform.SetScale3D(Scale);
        NewActor->SetActorTransform(Transform);

        // Return the created actor's details
        return FEpicUnrealMCPCommonUtils::ActorToJsonObject(NewActor, true);
    }

    return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Failed to create actor"));
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleDeleteActor(const TSharedPtr<FJsonObject>& Params)
{
    FString ActorName;
    if (!Params->TryGetStringField(TEXT("name"), ActorName))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'name' parameter"));
    }

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GWorld, AActor::StaticClass(), AllActors);
    
    for (AActor* Actor : AllActors)
    {
        if (Actor && Actor->GetName() == ActorName)
        {
            // Store actor info before deletion for the response
            TSharedPtr<FJsonObject> ActorInfo = FEpicUnrealMCPCommonUtils::ActorToJsonObject(Actor);
            
            // Delete the actor
            Actor->Destroy();
            
            TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
            ResultObj->SetObjectField(TEXT("deleted_actor"), ActorInfo);
            return ResultObj;
        }
    }
    
    return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("Actor not found: %s"), *ActorName));
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleSetActorTransform(const TSharedPtr<FJsonObject>& Params)
{
    // Get actor name
    FString ActorName;
    if (!Params->TryGetStringField(TEXT("name"), ActorName))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'name' parameter"));
    }

    // Find the actor
    AActor* TargetActor = nullptr;
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GWorld, AActor::StaticClass(), AllActors);
    
    for (AActor* Actor : AllActors)
    {
        if (Actor && Actor->GetName() == ActorName)
        {
            TargetActor = Actor;
            break;
        }
    }

    if (!TargetActor)
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("Actor not found: %s"), *ActorName));
    }

    // Get transform parameters
    FTransform NewTransform = TargetActor->GetTransform();

    if (Params->HasField(TEXT("location")))
    {
        NewTransform.SetLocation(FEpicUnrealMCPCommonUtils::GetVectorFromJson(Params, TEXT("location")));
    }
    if (Params->HasField(TEXT("rotation")))
    {
        NewTransform.SetRotation(FQuat(FEpicUnrealMCPCommonUtils::GetRotatorFromJson(Params, TEXT("rotation"))));
    }
    if (Params->HasField(TEXT("scale")))
    {
        NewTransform.SetScale3D(FEpicUnrealMCPCommonUtils::GetVectorFromJson(Params, TEXT("scale")));
    }

    // Set the new transform
    TargetActor->SetActorTransform(NewTransform);

    // Return updated actor info
    return FEpicUnrealMCPCommonUtils::ActorToJsonObject(TargetActor, true);
}


TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleImportFbx(const TSharedPtr<FJsonObject>& Params)
{
    FString SourceFbx;
    if (!Params->TryGetStringField(TEXT("source_fbx"), SourceFbx))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Missing 'source_fbx' parameter"));
    }

    if (!FPaths::FileExists(SourceFbx))
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(FString::Printf(TEXT("FBX file not found: %s"), *SourceFbx));
    }

    FString DestinationPath = TEXT("/Game/Billiards/Meshes");
    Params->TryGetStringField(TEXT("destination_path"), DestinationPath);

    bool bReplaceExisting = true;
    bool bImportMaterials = true;
    bool bImportTextures = true;
    bool bCombineMeshes = false;
    bool bAutoGenerateCollision = false;
    bool bTransformVertexToAbsolute = true;
    bool bConvertScene = true;
    bool bConvertSceneUnit = true;
    float ImportUniformScale = 1.0f;

    Params->TryGetBoolField(TEXT("replace_existing"), bReplaceExisting);
    Params->TryGetBoolField(TEXT("import_materials"), bImportMaterials);
    Params->TryGetBoolField(TEXT("import_textures"), bImportTextures);
    Params->TryGetBoolField(TEXT("combine_meshes"), bCombineMeshes);
    Params->TryGetBoolField(TEXT("auto_generate_collision"), bAutoGenerateCollision);
    Params->TryGetBoolField(TEXT("transform_vertex_to_absolute"), bTransformVertexToAbsolute);
    Params->TryGetBoolField(TEXT("convert_scene"), bConvertScene);
    Params->TryGetBoolField(TEXT("convert_scene_unit"), bConvertSceneUnit);

    double UniformScaleAsDouble = 1.0;
    if (Params->TryGetNumberField(TEXT("import_uniform_scale"), UniformScaleAsDouble))
    {
        ImportUniformScale = static_cast<float>(UniformScaleAsDouble);
    }

    if (!UEditorAssetLibrary::DoesDirectoryExist(DestinationPath))
    {
        UEditorAssetLibrary::MakeDirectory(DestinationPath);
    }

    TArray<FString> PreDeletedAssets;
    if (bReplaceExisting)
    {
        const FString SourceBaseName = FPaths::GetBaseFilename(SourceFbx);
        const FString SourcePrefix = SourceBaseName + TEXT("_");
        const TArray<FString> ExistingAssets = UEditorAssetLibrary::ListAssets(DestinationPath, true, false);

        for (const FString& AssetPath : ExistingAssets)
        {
            const FString AssetName = FPackageName::ObjectPathToObjectName(AssetPath);
            if (AssetName.Equals(SourceBaseName) || AssetName.StartsWith(SourcePrefix))
            {
                if (UEditorAssetLibrary::DeleteAsset(AssetPath))
                {
                    PreDeletedAssets.Add(AssetPath);
                }
            }
        }
    }

    UFbxImportUI* ImportUI = NewObject<UFbxImportUI>();
    ImportUI->bAutomatedImportShouldDetectType = false;
    ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    ImportUI->bImportMesh = true;
    ImportUI->bImportAsSkeletal = false;
    ImportUI->bImportAnimations = false;
    ImportUI->bImportMaterials = bImportMaterials;
    ImportUI->bImportTextures = bImportTextures;

    if (!ImportUI->StaticMeshImportData)
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Failed to initialize static mesh import data"));
    }

    UFbxStaticMeshImportData* SMData = ImportUI->StaticMeshImportData;
    SMData->bBuildNanite = false;
    SMData->bCombineMeshes = bCombineMeshes;
    SMData->bAutoGenerateCollision = bAutoGenerateCollision;
    SMData->NormalImportMethod = EFBXNormalImportMethod::FBXNIM_ImportNormalsAndTangents;
    SMData->bRemoveDegenerates = true;
    SMData->bTransformVertexToAbsolute = bTransformVertexToAbsolute;
    SMData->bBakePivotInVertex = false;
    SMData->ImportUniformScale = ImportUniformScale;
    SMData->bConvertScene = bConvertScene;
    SMData->bConvertSceneUnit = bConvertSceneUnit;

    UAssetImportTask* Task = NewObject<UAssetImportTask>();
    Task->Filename = SourceFbx;
    Task->DestinationPath = DestinationPath;
    Task->bAutomated = true;
    Task->bSave = true;
    // Avoid UE reimport code path (currently unstable for MCP automation on Linux);
    // when replace_existing=true we pre-delete matching assets above and do a clean import.
    Task->bReplaceExisting = false;
    Task->bReplaceExistingSettings = false;
    Task->Options = ImportUI;

    UFbxFactory* FbxFactory = NewObject<UFbxFactory>();
    Task->Factory = FbxFactory;

    TArray<UAssetImportTask*> Tasks;
    Tasks.Add(Task);

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
    AssetToolsModule.Get().ImportAssetTasks(Tasks);

    FEditorFileUtils::SaveDirtyPackages(false, true, true, false, false, false);

    TSet<FString> ImportedPaths;
    const TArray<UObject*> ImportedObjects = Task->GetObjects();
    for (UObject* ImportedObject : ImportedObjects)
    {
        if (ImportedObject)
        {
            ImportedPaths.Add(ImportedObject->GetPathName());
        }
    }

    // Some automated imports can return null object handles even when assets were created.
    if (ImportedPaths.Num() == 0)
    {
        const FString SourceBaseName = FPaths::GetBaseFilename(SourceFbx);
        const FString SourcePrefix = SourceBaseName + TEXT("_");
        const TArray<FString> PostImportAssets = UEditorAssetLibrary::ListAssets(DestinationPath, true, false);

        for (const FString& AssetPath : PostImportAssets)
        {
            const FString AssetName = FPackageName::ObjectPathToObjectName(AssetPath);
            if (AssetName.Equals(SourceBaseName) || AssetName.StartsWith(SourcePrefix))
            {
                ImportedPaths.Add(AssetPath);
            }
        }
    }

    if (ImportedPaths.Num() == 0)
    {
        return FEpicUnrealMCPCommonUtils::CreateErrorResponse(TEXT("Import completed with zero imported objects"));
    }

    TArray<FString> ImportedPathsSorted = ImportedPaths.Array();
    ImportedPathsSorted.Sort();

    TArray<TSharedPtr<FJsonValue>> ImportedArray;
    for (const FString& ImportedPath : ImportedPathsSorted)
    {
        ImportedArray.Add(MakeShared<FJsonValueString>(ImportedPath));
    }

    TArray<TSharedPtr<FJsonValue>> PreDeletedArray;
    for (const FString& AssetPath : PreDeletedAssets)
    {
        PreDeletedArray.Add(MakeShared<FJsonValueString>(AssetPath));
    }

    TSharedPtr<FJsonObject> SettingsObj = MakeShared<FJsonObject>();
    SettingsObj->SetBoolField(TEXT("replace_existing"), bReplaceExisting);
    SettingsObj->SetBoolField(TEXT("import_materials"), bImportMaterials);
    SettingsObj->SetBoolField(TEXT("import_textures"), bImportTextures);
    SettingsObj->SetBoolField(TEXT("combine_meshes"), bCombineMeshes);
    SettingsObj->SetBoolField(TEXT("auto_generate_collision"), bAutoGenerateCollision);
    SettingsObj->SetBoolField(TEXT("transform_vertex_to_absolute"), bTransformVertexToAbsolute);
    SettingsObj->SetBoolField(TEXT("convert_scene"), bConvertScene);
    SettingsObj->SetBoolField(TEXT("convert_scene_unit"), bConvertSceneUnit);
    SettingsObj->SetNumberField(TEXT("import_uniform_scale"), ImportUniformScale);

    TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
    ResultObj->SetStringField(TEXT("source_fbx"), SourceFbx);
    ResultObj->SetStringField(TEXT("destination_path"), DestinationPath);
    ResultObj->SetNumberField(TEXT("imported_count"), ImportedArray.Num());
    ResultObj->SetArrayField(TEXT("imported_object_paths"), ImportedArray);
    ResultObj->SetArrayField(TEXT("pre_deleted_asset_paths"), PreDeletedArray);
    ResultObj->SetObjectField(TEXT("settings"), SettingsObj);

    return ResultObj;
}

TSharedPtr<FJsonObject> FEpicUnrealMCPEditorCommands::HandleSpawnBlueprintActor(const TSharedPtr<FJsonObject>& Params)
{
    // This function will now correctly call the implementation in BlueprintCommands
    FEpicUnrealMCPBlueprintCommands BlueprintCommands;
    return BlueprintCommands.HandleCommand(TEXT("spawn_blueprint_actor"), Params);
}
