#include "g3d/g3d_System.h"
#include "g3d/dFuncNode.h"
#include <new>
#include "Bs/dBsLightMgr.h"
#include "Bs/dBsSeadParticleMgr.h"
#include "Camera/dCameraMgr.h"
#include "Other/dHeapUtil.h"
#include "Other/dUlcdFramework.h"
#include "Sv/dSvProc.h"
#include "g3d/dResourceLoader.h"
#include "g3d/dSortMeshGroup.h"
#include "graphic/dModuleRenderer.h"
#include "graphic/dScreenCamera.h"
#include "graphic/dShadowRenderer.h"
#include "nn/gx/gx_Api.h"
#include "nonameg3d/dCharTreeNode.h"
#include "nw/gfx/gfx_BillboardUpdater.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_ISceneUpdater.h"
#include "nw/gfx/internal/gfx_LightingRegisters.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/gfx_LightSet.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_MeshRenderer.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_PriorDepthRenderKeyFactory.h"
#include "nw/gfx/gfx_PriorMaterialRenderKeyFactory.h"
#include "nw/gfx/gfx_RenderContext_Builder.h"
#include "nw/gfx/gfx_SceneContext.h"
#include "nw/gfx/gfx_SceneContext_Builder.h"
#include "nw/gfx/gfx_SceneTraverser.h"
#include "nw/gfx/gfx_SceneTraverser_Builder.h"
#include "nw/gfx/gfx_SceneUpdater.h"
#include "nw/gfx/gfx_SceneUpdater_Builder.h"
#include "nw/gfx/gfx_SkeletonUpdater_Builder.h"
#include "nw/gfx/gfx_WorldMatrixUpdater_Builder.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResLookupTableSet.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"
#include "nonameg3d/dChar.h"
#include "sead/seadTreeMapNode.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_RenderContext.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_SkeletonUpdater.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_WorldMatrixUpdater.h"
#include "nn/math/math_ARMv6.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the key factories of the render queue
typedef nw::gfx::PriorMaterialRenderKeyFactory<u64, nw::gfx::RenderElement::Depth24bitQuantizer> MaterialKeyFactory;
typedef nw::gfx::PriorDepthRenderKeyFactory<u64, nw::gfx::RenderElement::ReverseDepth24bitQuantizer> DepthKeyFactory;

// the matrices of the camera of the top screen while it has the matrices of an image (SelectCamera, Render)
struct SavedCameraMatrices
{
    nn::math::MTX34 view;         // 0x00
    nn::math::MTX34 inverseView;  // 0x30
    nn::math::MTX44 projection;   // 0x60
};

// the heap of the engine
const u32 ENGINE_HEAP_SIZE = 0x39800;
// the command lists of the saved scene (one is filled while the other one is used)
const s32 SAVED_CMDLIST_COUNT = 2;
const s32 CMDLIST_BUFFER_SIZE = 0x96000;
const s32 CMDLIST_REQUEST_COUNT = 10;
// the parameters of nngxGetCmdlistParameteri / nngxSetCmdlistParameteri (as in nn::gx)
const GLenum CMDLIST_RUN_MODE = 0x200;
const GLenum CMDLIST_NAME = 0x207;
// (the value of the original)
const GLint CMDLIST_RUN_MODE_VALUE = 0x300;
// all states for nngxUpdateState
const GLbitfield UPDATE_STATE_ALL = 0x3FFF;
// the elements of the render queue
const s32 RENDER_QUEUE_CAPACITY = 512;
// the draw modes of the screens (ssys::ma::lyt::Layout)
const u32 DRAW_MODE_TOP_LEFT = 0x400;
const u32 DRAW_MODE_BOTTOM = 0x401;
const u32 DRAW_MODE_TOP_RIGHT = 0x410;
// the scenes Draw and RenderShadow render
const s32 SCENE_DRAW = 1;
const s32 SCENE_SHADOW = 2;
// the state ResetRenderState resets
const s32 RESET_STATE_FLAGS = 0x107;

// (inline; name is ours) the camera at the index becomes the active one of the environment; a new active camera sets
// the w buffer scale of its depth (0: z buffer)
inline void ApplyActiveCamera(nw::gfx::SceneEnvironment& environment, int cameraIndex)
{
    nw::gfx::Camera* camera = environment.m_Cameras.begin()[cameraIndex];
    environment.m_ActiveCameraIndex = cameraIndex;
    if (environment.m_pCamera == camera) {
        return;
    }
    environment.m_pCamera = camera;
    const f32 wScale = camera->m_WScale;
    if (wScale == 0.0f) {
        nw::gfx::internal::g_WScale = 0;
    } else {
        nw::gfx::internal::g_WScale = nw::gfx::internal::Float32ToFloat24Clamped(-wScale);
    }
}
} // namespace

// 0x00953A30
bool s_IsRendering = false;
// the flag of the UlcdFramework before BeginRender
// 0x00953A31
bool s_WasRendering3D = false;
// 0x00953A34
ssys::ma::Allocator* s_pAllocator = NULL;
// 0x00953A38
nw::gfx::WorldMatrixUpdater* s_pWorldMatrixUpdater = NULL;
// 0x00953A3C
nw::gfx::SkeletonUpdater* s_pSkeletonUpdater = NULL;
// 0x00953A40
nw::gfx::BillboardUpdater* s_pBillboardUpdater = NULL;
// 0x00953A44
nw::gfx::SceneUpdater* s_pSceneUpdater = NULL;
// 0x00953A48
nw::gfx::RenderContext* s_pRenderContext = NULL;
// 0x00953A4C
nw::gfx::MeshRenderer* s_pMeshRenderer = NULL;
// 0x00953A50
ResourceLoader* s_pShaderResource = NULL;
// 0x00953A54
ResourceLoader* s_pLutResource = NULL;
// the saved command list that the next frame fills
// 0x00953A58
s32 s_SavedCmdlistIndex = 0;
// 0x00953A5C
nw::gfx::Camera* s_pCurrentCamera = NULL;
// the shader file of the engine
// 0x00953A60
const char* s_pShaderPath = "Shader/nwgfx_DefaultShader.bcsdr";
// 0x00953A68
nw::gfx::RenderQueue* s_pRenderQueue = NULL;
// 0x00953A6C
MaterialKeyFactory* s_pMaterialKeyFactory = NULL;
// 0x00953A70
DepthKeyFactory* s_pDepthKeyFactory = NULL;
// 0x00953A78
GLuint s_Cmdlists[SAVED_CMDLIST_COUNT];

// 0x009CA0A8
nw::gfx::LightSet* s_pLightSets[LIGHT_SET_COUNT];
// 0x009CA0B8
nw::gfx::SceneContext* s_pSceneContexts[SCENE_COUNT];
// 0x009CA0C4
nw::gfx::SceneTraverser* s_pSceneTraversers[SCENE_COUNT];
// 0x009CA0D0
SavedCmdlist s_SavedCmdlists[SAVED_CMDLIST_COUNT];
// 0x009CA0F8
SavedCameraMatrices s_SavedCameraMatrices;

// 0x00953A64
const nw::gfx::TypeInfo FuncNode::s_TypeInfo(&nw::gfx::TransformNode::s_TypeInfo);
// 0x00953A74
sead::TreeMapImpl<nonameg3d::Char> s_LookupTables;

// constructs s_LookupTables, s_SavedCmdlists and the type info of FuncNode
// 0x007A0218 (static initializer)

namespace {
// the smallest world scale: its square is the smallest squared length of the scale vector, and (so in the original)
// also the value of the components of a scale that is too short
const f32 MIN_SCALE = 0.001f;
const f32 MIN_SCALE_SQ = MIN_SCALE * MIN_SCALE;

// (inline) the world scale kept away from zero and its flags (name is ours)
inline void SetWorldScale(nw::gfx::CalculatedTransform& worldTransform, nn::math::VEC3 scale)
{
    if (scale.x * scale.x + scale.y * scale.y + scale.z * scale.z < MIN_SCALE_SQ) {
        scale.x = (scale.x >= 0.0f) ? MIN_SCALE_SQ : -MIN_SCALE_SQ;
        scale.y = (scale.y >= 0.0f) ? MIN_SCALE_SQ : -MIN_SCALE_SQ;
        scale.z = (scale.z >= 0.0f) ? MIN_SCALE_SQ : -MIN_SCALE_SQ;
    }
    worldTransform.SetScale(scale);
}

// (inline) the composite flags of a world transform after its matrix and scale changed (name is ours)
inline void ResetWorldFlags(nw::gfx::CalculatedTransform& worldTransform)
{
    worldTransform.DisableFlags(nw::gfx::CalculatedTransform::FLAG_IS_IDENTITY |
                                nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO |
                                nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO |
                                nw::gfx::CalculatedTransform::FLAG_IS_TRANSLATE_ZERO |
                                nw::gfx::CalculatedTransform::FLAG_IS_SCALE_ONE |
                                nw::gfx::CalculatedTransform::FLAG_IS_UNIFORM_SCALE);
    worldTransform.UpdateScaleFlags();
}
} // namespace

// 0x004F0694 (name is ours)
ssys::ma::Allocator* GetAllocator()
{
    return s_pAllocator;
}

// 0x004F09E0 (name is ours)
void UpdateSkeleton(nw::gfx::SceneNode* node)
{
    nw::gfx::SkeletalModel* model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(node);
    if (model == NULL || model->m_IsWorldMatrixAnimation == true) {
        return;
    }
    model->m_Skeleton.Get()->m_IsWorldCalculated = false;
    s_pSkeletonUpdater->UpdateWorld(model->m_Skeleton.Get(), *s_pWorldMatrixUpdater);
}

// 0x004F0A58 (name is ours)
void UpdateWorldMatrix(nw::gfx::TransformNode* node)
{
    const nw::gfx::CalculatedTransform* parentWorldTransform;
    const nw::gfx::CalculatedTransform* parentTransform;
    nw::gfx::SceneNode* parent = node->m_pParent;
    if (parent != NULL) {
        parentWorldTransform = &parent->TrackbackWorldTransform();
        parentTransform = &parent->TrackbackLocalTransform();
    } else {
        parentWorldTransform = &nw::gfx::CalculatedTransform::Identity();
        parentTransform = &nw::gfx::CalculatedTransform::Identity();
    }
    nn::math::VEC3 scale;
    s_pWorldMatrixUpdater->CalculateWorldBasic(&node->m_WorldTransform.m_TransformMatrix, &scale, node->m_Transform,
                                               *parentWorldTransform, *parentTransform);
    SetWorldScale(node->m_WorldTransform, scale);
    nn::math::ARMv6::MTX34MultScaleAsm(&node->m_WorldMatrix, &node->m_WorldTransform.m_TransformMatrix,
                                       &node->m_Transform.m_Scale);
    ResetWorldFlags(node->m_WorldTransform);
    node->ClearUnknown0x12C();
}

// 0x004EB258 (name is ours)
void SetFogDirty()
{
    s_pRenderContext->m_SceneEnvironment.m_IsFogDirty = true;
}

// 0x004EB270 (name is ours)
void UpdateSkeletonView(nw::gfx::SkeletalModel* model)
{
    if (s_pCurrentCamera == NULL) {
        return;
    }
    s_pSkeletonUpdater->UpdateView(model->GetSkeleton(), *s_pBillboardUpdater, *s_pCurrentCamera);
}

// 0x004EF2A8 (name is ours)
void SetEnvironmentDirty()
{
    nw::gfx::SceneEnvironment& environment = s_pRenderContext->m_SceneEnvironment;
    environment.m_IsFogDirty = true;
    environment.m_IsAmbientLightDirty = true;
    environment.m_IsFragmentLightDirty = true;
    environment.m_IsHemiSphereLightDirty = true;
}

// 0x004EF2CC (name is ours)
nw::gfx::RenderContext* GetRenderContext()
{
    return s_pRenderContext;
}

// 0x004ED614 (name is ours)
g3d::SavedCmdlist::SavedCmdlist() : cmdlist(0)
{
}

// 0x004EAB28 (name is ours)
void RenderShadow()
{
    graphic::ShadowRenderer* shadowRenderer = graphic::ShadowRenderer::s_pInstance;
    if (shadowRenderer == NULL) {
        return;
    }
    shadowRenderer->BeginDraw(s_pRenderContext);
    s_pCurrentCamera = s_pRenderContext->m_SceneEnvironment.m_pCamera;
    RenderScene(s_pSceneContexts[SCENE_SHADOW], s_pCurrentCamera);
    shadowRenderer->EndDraw(s_pRenderContext);
    s_pCurrentCamera = NULL;
}

// 0x004EAB88 (name is ours)
bool Initialize(sead::Heap* heap, s32 maxNodes, s32 maxCameras)
{
    sead::ExpHeap* engineHeap = HeapUtil::CreateExpHeapWithCName(ENGINE_HEAP_SIZE, heap, "dG3d", 32);

    GLint boundCmdlist;
    nngxGetCmdlistParameteri(CMDLIST_NAME, &boundCmdlist);
    nngxGenCmdlists(SAVED_CMDLIST_COUNT, s_Cmdlists);
    for (s32 i = 0; i < SAVED_CMDLIST_COUNT; i++) {
        nngxBindCmdlist(s_Cmdlists[i]);
        nngxCmdlistStorage(CMDLIST_BUFFER_SIZE, CMDLIST_REQUEST_COUNT);
        nngxSetCmdlistParameteri(CMDLIST_RUN_MODE, CMDLIST_RUN_MODE_VALUE);
    }
    nngxBindCmdlist(boundCmdlist);

    s_pAllocator = new (engineHeap, 4) ssys::ma::Allocator;
    s_pAllocator->SetHeap(engineHeap);
    s_pSkeletonUpdater = nw::gfx::SkeletonUpdater::Builder().Create(s_pAllocator);
    s_pWorldMatrixUpdater = nw::gfx::WorldMatrixUpdater::Builder().Create(s_pAllocator);
    s_pBillboardUpdater = nw::gfx::BillboardUpdater::Create(s_pAllocator);
    {
        nw::gfx::SceneUpdater::Builder builder;
        {
            nw::gfx::GfxPtr<nw::gfx::WorldMatrixUpdater> worldMatrixUpdater(s_pWorldMatrixUpdater);
            builder.m_WorldMatrixUpdater = worldMatrixUpdater;
        }
        {
            nw::gfx::GfxPtr<nw::gfx::SkeletonUpdater> skeletonUpdater(s_pSkeletonUpdater);
            builder.m_SkeletonUpdater = skeletonUpdater;
        }
        {
            nw::gfx::GfxPtr<nw::gfx::BillboardUpdater> billboardUpdater(s_pBillboardUpdater);
            builder.m_BillboardUpdater = billboardUpdater;
        }
        s_pSceneUpdater = builder.Create(s_pAllocator);
    }

    for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
        nw::gfx::LightSet::Description description;
        description.isFixedSizeMemory = true;
        description.maxVertexLights = 0;
        s_pLightSets[i] = nw::gfx::LightSet::Create(description, s_pAllocator);
    }
    for (s32 i = 0; i < SCENE_COUNT; i++) {
        nw::gfx::SceneContext::Builder builder;
        builder.m_IsFixedSizeMemory = true;
        builder.m_MaxSceneNodes = maxNodes;
        builder.m_MaxUnknown0x18Nodes = 0;
        builder.m_MaxModels = maxNodes;
        builder.m_MaxSkeletalModels = maxNodes;
        builder.m_MaxCameras = maxCameras;
        builder.m_MaxLights = 8;
        builder.m_MaxFragmentLights = 8;
        builder.m_MaxVertexLights = 8;
        builder.m_MaxHemiSphereLights = 4;
        builder.m_MaxAmbientLights = 4;
        builder.m_MaxFogs = 4;
        builder.m_MaxParticleSets = maxNodes;
        builder.m_MaxParticleEmitters = maxNodes;
        builder.m_MaxParticleModels = maxNodes;
        builder.m_MaxAnimatableNodes = maxNodes;
        s_pSceneContexts[i] = builder.Create(s_pAllocator);
        s_pSceneTraversers[i] = nw::gfx::SceneTraverser::Builder().Create(s_pAllocator);
    }
    BeginScenes();

    {
        nw::gfx::RenderContext::Builder builder;
        builder.m_MaxCameras = 32;
        builder.m_MaxLightSets = LIGHT_SET_COUNT;
        builder.m_MaxFogs = 1;
        builder.m_MaxVertexLights = 4;
        builder.m_pParticleMaterialActivator = NULL;
        s_pRenderContext = builder.Create(s_pAllocator);
    }
    s_pMeshRenderer = nw::gfx::MeshRenderer::Create(s_pAllocator);
    s_pMeshRenderer->m_pRenderContext = s_pRenderContext;

    void* memory = s_pAllocator->Alloc(sizeof(nw::gfx::RenderQueue), 4);
    s_pRenderQueue = (memory != NULL) ? new (memory) nw::gfx::RenderQueue(s_pAllocator, RENDER_QUEUE_CAPACITY, true) : NULL;
    memory = s_pAllocator->Alloc(sizeof(MaterialKeyFactory), 4);
    s_pMaterialKeyFactory = (memory != NULL) ? new (memory) MaterialKeyFactory(s_pAllocator) : NULL;
    memory = s_pAllocator->Alloc(sizeof(DepthKeyFactory), 4);
    s_pDepthKeyFactory = (memory != NULL) ? new (memory) DepthKeyFactory(s_pAllocator) : NULL;
    s_pRenderQueue->Reset(s_pMaterialKeyFactory, s_pDepthKeyFactory, s_pMaterialKeyFactory, s_pMaterialKeyFactory);

    memory = s_pAllocator->Alloc(sizeof(ResourceLoader), ResourceLoader::CGFX_ALIGNMENT);
    s_pShaderResource = (memory != NULL) ? new (memory) ResourceLoader : NULL;
    s_pLutResource = new (s_pAllocator->m_pHeap, 4) ResourceLoader;
    InitializeSortMeshGroups(GetSortMeshGroups(), heap);
    s_IsRendering = false;
    return true;
}

// 0x004EB298 (name is ours)
nw::gfx::Camera* SelectCamera(u32 drawMode, nw::gfx::SceneContext* sceneContext)
{
    if (sceneContext->m_Cameras.empty()) {
        return NULL;
    }
    nw::gfx::Camera* camera = *sceneContext->m_Cameras.begin();
    if (drawMode == DRAW_MODE_BOTTOM) {
        return camera;
    }
    if (drawMode == DRAW_MODE_TOP_LEFT || drawMode == DRAW_MODE_TOP_RIGHT) {
        s_SavedCameraMatrices.view = camera->m_ViewMatrix;
        s_SavedCameraMatrices.inverseView = camera->m_InverseViewMatrix;
        s_SavedCameraMatrices.projection = camera->m_ProjectionMatrix;
        graphic::ScreenCamera::s_Instance.SetCamera(camera);
    }
    camera->m_ProjectionMatrix = graphic::ScreenCamera::s_Instance.GetProjectionMatrix(drawMode);
    camera->m_ViewMatrix = graphic::ScreenCamera::s_Instance.GetViewMatrix(drawMode);
    camera->m_InverseViewMatrix = graphic::ScreenCamera::s_Instance.GetInverseViewMatrix(drawMode);
    return camera;
}

// 0x004EB3D8 (name is ours)
void Draw(u32 drawMode)
{
    if (s_IsRendering != true || SvProc::IsUnknownFlag0xFADSet()) {
        return;
    }
    Render(drawMode, SCENE_DRAW);
    *s_pRenderContext->m_SceneEnvironment.m_Fogs.begin() = NULL;
}

// 0x004EB7D0 (name is ours)
void BeginScenes()
{
    for (s32 i = 0; i < SCENE_COUNT; i++) {
        s_pSceneTraversers[i]->Begin(s_pSceneContexts[i]);
        s_pSceneContexts[i]->Clear();
    }
    for (SortMeshGroup* group = GetSortMeshGroups(); group < GetSortMeshGroups() + SORT_MESH_GROUP_COUNT; group++) {
        group->Clear();
    }
}

// 0x004ECBFC (name is ours)
void RenderScene(nw::gfx::SceneContext* sceneContext, nw::gfx::Camera* camera)
{
    s_pSceneUpdater->SubmitView(NULL, sceneContext, *camera, 0, nw::gfx::ISceneUpdater::RENDER_SORT_MODE_MESH);
    for (u32 pass = 0; pass <= nw::gfx::res::ResMaterialData::TRANSLUCENCY_KIND_ADDITIVE; pass++) {
        for (nw::gfx::SceneNode** it = sceneContext->m_SceneNodes.begin(); it != sceneContext->m_SceneNodes.end(); ++it) {
            nw::gfx::Model* model = nw::gfx::DynamicCast<nw::gfx::Model>(*it);
            if (model != NULL) {
                nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits> meshes = model->GetResMeshes();
                for (const nw::ut::Offset* offset = meshes.m_pBegin; offset != meshes.m_pEnd; ++offset) {
                    nw::gfx::res::ResMesh mesh(offset->ToPtr());
                    if (!model->IsMeshVisible(mesh)) {
                        continue;
                    }
                    if (static_cast<u8>(model->GetMaterial(mesh.ptr()->materialIndex)
                                            ->GetResource(nw::gfx::Material::RESOURCE_SHADING)
                                            .ptr()
                                            ->translucencyKind) != static_cast<u8>(pass)) {
                        continue;
                    }
                    s_pMeshRenderer->RenderMesh(mesh, model);
                }
                continue;
            }
            FuncNode* funcNode = nw::gfx::DynamicCast<FuncNode>(*it);
            if (funcNode == NULL) {
                continue;
            }
            if (pass == nw::gfx::res::ResMaterialData::TRANSLUCENCY_KIND_OPAQUE) {
                if (funcNode->m_pOpaqueFunc != NULL) {
                    funcNode->m_pOpaqueFunc(s_pMeshRenderer, funcNode->m_pUserData);
                }
            } else if (pass == nw::gfx::res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT) {
                if (funcNode->m_pTranslucentFunc != NULL) {
                    funcNode->m_pTranslucentFunc(s_pMeshRenderer, funcNode->m_pUserData);
                }
            }
        }
    }
}

// 0x004EEBAC (name is ours)
bool IsRendering()
{
    return s_IsRendering;
}

// 0x004EFBD0 (name is ours)
bool LoadSystemResources()
{
    sead::Heap* heap = s_pAllocator->m_pHeap;
    s_pShaderResource->Load(sead::SafeStringBase<char>(s_pShaderPath), heap, ResourceLoader::CGFX_ALIGNMENT);
    s_pShaderResource->GetGraphicsFile().Setup(s_pAllocator, s_pShaderResource->GetGraphicsFile());
    s_pLutResource->Load(sead::SafeStringBase<char>("LUT/Lut_GardenPlus.bcres"), heap, ResourceLoader::CGFX_ALIGNMENT);
    nw::gfx::res::ResGraphicsFile lutFile = s_pLutResource->GetGraphicsFile();
    lutFile.Setup(s_pAllocator, s_pLutResource->GetGraphicsFile());
    const u32 setCount = lutFile.ptr()->luts.count;
    for (u32 i = 0; i < setCount; i++) {
        const nw::gfx::res::ResLookupTableSetData* set =
            static_cast<const nw::gfx::res::ResLookupTableSetData*>(lutFile.ptr()->luts.GetData(i));
        const u32 tableCount = set->tables.count;
        for (u32 j = 0; j < tableCount; j++) {
            nonameg3d::CharTreeNode* node = new (s_pAllocator->m_pHeap, 4) nonameg3d::CharTreeNode;
            nw::gfx::res::ResImageLookupTableData* table = static_cast<nw::gfx::res::ResImageLookupTableData*>(set->tables.GetData(j));
            node->mKey.m_pName = table->name.To<char>();
            node->m_pValue = table;
            s_LookupTables.mRoot = s_LookupTables.insert(s_LookupTables.mRoot, node);
            s_LookupTables.mRoot->mColor = sead::TreeMapImpl<nonameg3d::Char>::BLACK;
        }
    }
    return true;
}

// 0x004F0194 (name is ours)
void Render(u32 drawMode, s32 sceneIdx)
{
    nw::gfx::SceneContext* sceneContext = s_pSceneContexts[sceneIdx];
    s_pCurrentCamera = SelectCamera(drawMode, sceneContext);
    if (s_pCurrentCamera == NULL) {
        return;
    }
    SetupRenderContext();
    s_pRenderContext->SetCameraMatrix(s_pRenderContext->m_SceneEnvironment.m_pCamera, true);
    if (graphic::ModuleRenderer::s_pInstance != NULL) {
        graphic::ModuleRenderer::s_pInstance->Draw(drawMode);
    }
    if (BsLightMgr::GetInstance() != NULL) {
        BsLightMgr::GetInstance()->SetupLightSets(sceneContext, &s_pRenderContext->m_SceneEnvironment);
    } else {
        for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
            nw::gfx::LightSet* lightSet = s_pLightSets[i];
            lightSet->m_pAmbientLight = NULL;
            lightSet->m_pHemiSphereLight = NULL;
            lightSet->m_VertexLights.clear();
            lightSet->m_FragmentLights.clear();
        }
        nw::gfx::SceneEnvironment& environment = s_pRenderContext->m_SceneEnvironment;
        for (nw::gfx::FragmentLight** it = sceneContext->m_FragmentLights.begin(); it != sceneContext->m_FragmentLights.end(); ++it) {
            for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
                s_pLightSets[i]->m_FragmentLights.push_back(*it);
            }
        }
        if (!sceneContext->m_AmbientLights.empty()) {
            for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
                s_pLightSets[i]->m_pAmbientLight = *sceneContext->m_AmbientLights.begin();
            }
        }
        if (!sceneContext->m_HemiSphereLights.empty()) {
            for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
                s_pLightSets[i]->m_pHemiSphereLight = *sceneContext->m_HemiSphereLights.begin();
            }
        }
        for (s32 i = 0; i < LIGHT_SET_COUNT; i++) {
            environment.m_LightSets.begin()[i] = s_pLightSets[i];
        }
    }
    if (drawMode == DRAW_MODE_TOP_LEFT) {
        GLint boundCmdlist;
        nngxGetCmdlistParameteri(CMDLIST_NAME, &boundCmdlist);
        const GLuint cmdlist = s_Cmdlists[s_SavedCmdlistIndex];
        s_SavedCmdlists[s_SavedCmdlistIndex].cmdlist = cmdlist;
        nngxBindCmdlist(cmdlist);
        nngxClearCmdlist();
        nngxStartCmdlistSave();
        nw::gfx::SceneEnvironment& environment = s_pRenderContext->m_SceneEnvironment;
        nw::gfx::Camera* camera = s_pCurrentCamera;
        s32 fogIdx = 0;
        for (nw::gfx::Fog** it = sceneContext->m_Fogs.begin(); it != sceneContext->m_Fogs.end(); ++it) {
            (*it)->Update(camera);
            environment.m_Fogs.begin()[fogIdx] = *it;
            fogIdx++;
        }
        RenderScene(sceneContext, s_pCurrentCamera);
        SavedCmdlist& saved = s_SavedCmdlists[s_SavedCmdlistIndex];
        nngxSplitDrawCmdlist();
        nngxStopCmdlistSave(&saved.bufferOffset, &saved.bufferSize, &saved.requestOffset, &saved.requestCount);
        nngxBindCmdlist(boundCmdlist);
    }
    s_pRenderContext->SetCameraMatrix(s_pRenderContext->m_SceneEnvironment.m_pCamera, true);
    const SavedCmdlist& saved = s_SavedCmdlists[s_SavedCmdlistIndex];
    nngxUseSavedCmdlist(saved.cmdlist, saved.bufferOffset, saved.bufferSize, saved.requestOffset, saved.requestCount, 0, false);
    if (BsSeadParticleMgr::GetInstance() != NULL) {
        BsSeadParticleMgr::GetInstance()->DrawScene(drawMode);
    }
    nw::gfx::Camera* camera = s_pCurrentCamera;
    camera->m_ProjectionMatrix = s_SavedCameraMatrices.projection;
    camera->m_ViewMatrix = s_SavedCameraMatrices.view;
    camera->m_InverseViewMatrix = s_SavedCameraMatrices.inverseView;
    s_pCurrentCamera = NULL;
}

// 0x004F0638 (name is ours)
void EndRender()
{
    if (camera::Mgr::s_pInstance != NULL) {
        camera::Mgr::s_pInstance->m_pRenderData = NULL;
    }
    if (!s_IsRendering) {
        return;
    }
    s_IsRendering = false;
    if (UlcdFramework::GetInstance() != NULL && !s_WasRendering3D) {
        UlcdFramework::GetInstance()->m_IsRendering3D = false;
    }
}

// 0x004F0998 (name is ours)
void BeginRender()
{
    if (camera::Mgr::s_pInstance != NULL) {
        camera::Mgr::s_pInstance->m_pRenderData = &camera::Mgr::s_pInstance->m_RenderData;
    }
    s_IsRendering = true;
    s_WasRendering3D = UlcdFramework::GetInstance()->m_IsRendering3D;
    UlcdFramework::GetInstance()->m_IsRendering3D = true;
}

// 0x004F0BF0 (name is ours)
const nw::gfx::res::ResBinaryShaderData* GetShader(s32 shaderIdx)
{
    const nw::gfx::res::ResGraphicsFileData* file = s_pShaderResource->GetGraphicsFile().ptr();
    return nw::gfx::res::DynamicCastData<nw::gfx::res::ResBinaryShaderData>(file->shaders.GetData(shaderIdx),
                                                                            nw::gfx::res::ResShader::TYPE_BINARY);
}

// 0x004F0C54 (name is ours)
void ResetRenderState(s32 hashFlags)
{
    s_pRenderContext->ResetState(RESET_STATE_FLAGS, hashFlags);
}

// 0x004F0C70 (name is ours)
void UpdateWorldTransform(nw::gfx::CalculatedTransform* worldTransform, const nw::gfx::CalculatedTransform& transform)
{
    nn::math::VEC3 scale;
    s_pWorldMatrixUpdater->CalculateWorldBasic(&worldTransform->m_TransformMatrix, &scale, transform,
                                               nw::gfx::CalculatedTransform::Identity(),
                                               nw::gfx::CalculatedTransform::Identity());
    SetWorldScale(*worldTransform, scale);
    // (the world matrix is not kept: so in the original)
    nn::math::MTX34 worldMatrix;
    nn::math::ARMv6::MTX34MultScaleAsm(&worldMatrix, &worldTransform->m_TransformMatrix, &transform.m_Scale);
    ResetWorldFlags(*worldTransform);
}

// 0x004F0DBC (name is ours)
void SetupRenderContext()
{
    s_pRenderContext->ResetState();
    nw::gfx::SceneEnvironment& environment = s_pRenderContext->m_SceneEnvironment;
    graphic::ShadowRenderer* shadowRenderer = graphic::ShadowRenderer::s_pInstance;
    nw::gfx::Camera* camera =
        (shadowRenderer != NULL && shadowRenderer->m_IsDrawing) ? shadowRenderer->m_pCamera : s_pCurrentCamera;
    *environment.m_Cameras.begin() = camera;
    ApplyActiveCamera(s_pRenderContext->m_SceneEnvironment, 0);
}

// 0x004F30A0 (name is ours)
void EndFrame()
{
    for (s32 i = 0; i < SCENE_COUNT; i++) {
        s_pSceneTraversers[i]->End();
    }
    BeginScenes();
    s_pRenderContext->ResetState();
    nngxUpdateState(UPDATE_STATE_ALL);
    *s_pRenderContext->m_SceneEnvironment.m_Fogs.begin() = NULL;
    s_SavedCmdlistIndex = 1 - s_SavedCmdlistIndex;
}

} // namespace g3d
