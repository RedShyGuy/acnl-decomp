#pragma once

#include "decomp.h"

namespace nn {
namespace math {
struct VEC2;
struct VEC3;
} // namespace math
} // namespace nn

namespace nw {
namespace gfx {
class BillboardUpdater;
class Camera;
class LightSet;
class MeshRenderer;
class RenderContext;
class SceneContext;
class SceneNode;
class SceneTraverser;
class SceneUpdater;
class SkeletalModel;
class CalculatedTransform;
class SkeletonUpdater;
class TransformNode;
class WorldMatrixUpdater;
} // namespace gfx
namespace gfx {
namespace res {
struct ResBinaryShaderData;
struct ResImageTextureData;
struct ResMaterialData;
struct ResMeshData;
struct ResSkeletalModelData;
class ResMaterial;
class ResTexture;
struct ResLightingLookupTableData;
struct ResModelData;
struct ResSeparateDataShapeData;
struct ResTextureData;
} // namespace res
} // namespace gfx
namespace os {
class IAllocator;
} // namespace os
namespace ut {
struct FloatColor;
} // namespace ut
} // namespace nw

namespace nonameg3d {
class Char;
} // namespace nonameg3d

namespace sead {
class Heap;
template <typename Key>
class TreeMapImpl;
} // namespace sead

namespace ssys {
namespace ma {
class Allocator;
} // namespace ma
} // namespace ssys

// The 3D layer of the game over nw::gfx: models, lights, cameras, animations and their resources, plus the state of
// the engine (updaters, render context, scenes). Only the class names are from the RTTI and libgarden; all function,
// member and variable names are ours unless noted.
namespace g3d {
class Model;
class ResourceLoader;

// (resource helpers for the game; a component of 256 or more / below 0 keeps the component)
// the translation of a bone of the skeleton of the model resource; always true
bool GetBoneTranslate(nn::math::VEC3* translate, const nw::gfx::res::ResSkeletalModelData* model, s32 boneIdx); // 0x004F1B38
// the index of the bone of that name (-1: none)
s32 GetBoneIndex(const nw::gfx::res::ResSkeletalModelData* model, const char* boneName); // 0x004F1B98
// a constant color of the material of that name from components 0..255; false without the material
bool SetConstantColor(const nw::gfx::res::ResModelData* model, const char* materialName, s32 colorIdx, s32 r, s32 g, s32 b, s32 a); // 0x004F1CD0
bool SetConstantColor(nw::gfx::res::ResMaterialData* material, s32 colorIdx, s32 r, s32 g, s32 b, s32 a); // 0x004F33D0
void GetConstantColor(nw::ut::FloatColor* color, const nw::gfx::res::ResMaterialData* material, s32 colorIdx); // 0x004F3214
void SetConstantColor(nw::gfx::res::ResMaterialData* material, const nw::ut::FloatColor& color, s32 colorIdx); // 0x004F3298
// the translation of a texture coordinator of a material
void GetTextureTranslate(nn::math::VEC2* translate, const nw::gfx::res::ResModelData* model, const char* materialName, s32 coordinatorIdx); // 0x004F1E08
void GetTextureTranslate(nn::math::VEC2* translate, const nw::gfx::res::ResMaterialData* material, s32 coordinatorIdx); // 0x004F359C
// the last mesh whose mesh node has that name (NULL: none)
nw::gfx::res::ResMeshData* FindMeshByNodeName(const nw::gfx::res::ResModelData* model, const char* meshNodeName); // 0x004F1EB0
// the ambient color (its alpha kept) and the diffuse color (alpha 1) of all materials
void SetAmbientAndDiffuse(const nw::gfx::res::ResModelData* model, const nw::ut::FloatColor& color); // 0x004F1FA0
void SetAmbientAndDiffuse(const nw::gfx::res::ResModelData* model, f32 value); // 0x004F21EC
// the emission color (alpha 1) of all materials
void SetEmission(const nw::gfx::res::ResModelData* model, const nw::ut::FloatColor& color); // 0x004F2214
void SetEmission(const nw::gfx::res::ResModelData* model, f32 value); // 0x004F2364
// the light set of the scene environment for all materials (nothing for a negative index)
void SetLightSetIndex(const nw::gfx::res::ResModelData* model, s32 lightSetIndex); // 0x00317EE8
// all materials use the fog
void EnableFog(const nw::gfx::res::ResModelData* model); // 0x004F238C
// the buffer color of the texture combiners of a material; false without the material or its fragment shader
bool SetCombinerBufferColor(nw::gfx::res::ResMaterialData* material, s32 r, s32 g, s32 b, s32 a); // 0x004F3108
void GetDiffuse(nw::ut::FloatColor* color, const nw::gfx::res::ResMaterialData* material); // 0x004F31C4
void SetDiffuse(nw::gfx::res::ResMaterialData* material, const nw::ut::FloatColor& color); // 0x004F31E0
// the first texture of the file of the loader (set up first) for the textures of that name of the material
bool ReplaceTexture(Model* model, ResourceLoader* loader, nw::os::IAllocator* allocator, const char* materialName, const char* textureName); // 0x004F34A0
// the texture for the textures of that name of the material; false without the material
bool SetTexture(nw::gfx::res::ResMaterial material, const char* textureName, nw::gfx::res::ResTexture texture); // 0x004F3578
// the polygon offset of a material (0 without one; the second only if it is enabled)
f32 GetPolygonOffsetUnits(const nw::gfx::res::ResMaterialData* material); // 0x004F35D0
f32 GetEnabledPolygonOffsetUnits(const nw::gfx::res::ResMaterialData* material); // 0x004F35E8

// the engine in an ExpHeap "dG3d" of heap: the command lists of the saved scene, the updaters, light sets, scenes,
// the render context and queue and the loaders of the system files; always true
bool Initialize(sead::Heap* heap, s32 maxNodes, s32 maxCameras); // 0x004EAB88
// the shader and lookup table files of the engine; the lookup tables by name into s_LookupTables; always true
bool LoadSystemResources(); // 0x004EFBD0
// the frame begins / ends for the renderer
void BeginRender(); // 0x004F0998
void EndRender(); // 0x004F0638
bool IsRendering(); // 0x004EEBAC
// the scene of the screen (drawMode as ssys::ma::lyt::Layout) when rendering
void Draw(u32 drawMode); // 0x004EB3D8
// the scene with its camera for the screen: the commands of the scene are saved for the left image of the top
// screen (0x400) and used again for the other images
void Render(u32 drawMode, s32 sceneIdx); // 0x004F0194
// the meshes of the scene by translucency (the Func nodes in the first two passes)
void RenderScene(nw::gfx::SceneContext* sceneContext, nw::gfx::Camera* camera); // 0x004ECBFC
// the shadow scene with the camera of the ShadowRenderer of the game
void RenderShadow(); // 0x004EAB28
// the first camera of the scene with the matrices of the screen (the matrices of the camera are kept in
// s_SavedCameraMatrices for the top screen)
nw::gfx::Camera* SelectCamera(u32 drawMode, nw::gfx::SceneContext* sceneContext); // 0x004EB298
// the render context reset with the camera of the shadow renderer or the current camera
void SetupRenderContext(); // 0x004F0DBC
// the traversers begin again with empty scene contexts (and the sort mesh groups are emptied)
void BeginScenes(); // 0x004EB7D0
// the traversers end, the scenes begin again, the next saved command list
void EndFrame(); // 0x004F30A0
// a binary shader of the shader file of the engine (NULL: no binary shader at that index)
const nw::gfx::res::ResBinaryShaderData* GetShader(s32 shaderIdx); // 0x004F0BF0
void ResetRenderState(s32 hashFlags); // 0x004F0C54
// the world transform from a transform without a parent (the scale kept away from zero)
void UpdateWorldTransform(nw::gfx::CalculatedTransform* worldTransform, const nw::gfx::CalculatedTransform& transform); // 0x004F0C70

// the allocator of the engine objects (an ExpHeap made by Initialize)
ssys::ma::Allocator* GetAllocator(); // 0x004F0694

// the world matrix of a node from its local transform and its parent (the scale of the world transform is kept away
// from zero) and the matrices of its children
void UpdateWorldMatrix(nw::gfx::TransformNode* node); // 0x004F0A58
// the world matrices of the skeleton of a skeletal model (nothing for other nodes or a model that shares a skeleton)
void UpdateSkeleton(nw::gfx::SceneNode* node); // 0x004F09E0

// the view of the skeleton of a skeletal model for the current camera (nothing without one)
void UpdateSkeletonView(nw::gfx::SkeletalModel* model); // 0x004EB270
// the render context sends the fog again
void SetFogDirty(); // 0x004EB258
// the render context sends the fog and the fragment, ambient and hemisphere lights again
void SetEnvironmentDirty(); // 0x004EF2A8
nw::gfx::RenderContext* GetRenderContext(); // 0x004EF2CC

// the location flag of a buffer in a memory area of nngx (area | LOCATION_FLAG, as ResTexture::LOCATION_*)
const u32 LOCATION_FLAG = 0x02000000;
// the vertex streams of the shapes of a model into vertexArea and their index streams into indexArea
void SetupModelBuffers(const nw::gfx::res::ResModelData* model, u32 vertexArea, u32 indexArea); // 0x004F1C10
// the index streams of a shape into area
void SetupShapeBuffers(const nw::gfx::res::ResSeparateDataShapeData* shape, u32 area); // 0x004F23F4
// the images of a texture into area (a reference: its target)
void SetupTexture(const nw::gfx::res::ResTextureData* texture, u32 area); // 0x004F2894
// the image of src for dst (data, size and format)
void CopyTextureImage(nw::gfx::res::ResImageTextureData* dst, const nw::gfx::res::ResImageTextureData* src); // 0x004F2918
// other image data (a width or height of 0: unchanged)
void SetTextureImage(nw::gfx::res::ResImageTextureData* texture, const void* data, u32 width, u32 height); // 0x004F2A20
// the reference table of a lighting sampler bound to the table of that name in the lookup tables of the engine (only
// the tables of "Lut_Takumi" unless resolveAll)
void ResolveLookupTable(nw::gfx::res::ResLightingLookupTableData* sampler, bool resolveAll); // 0x004F1110

// the state of the engine
extern ssys::ma::Allocator* s_pAllocator;
extern nw::gfx::WorldMatrixUpdater* s_pWorldMatrixUpdater;
extern nw::gfx::SkeletonUpdater* s_pSkeletonUpdater;
extern nw::gfx::BillboardUpdater* s_pBillboardUpdater;
extern nw::gfx::SceneUpdater* s_pSceneUpdater;
extern nw::gfx::RenderContext* s_pRenderContext;
extern nw::gfx::MeshRenderer* s_pMeshRenderer;
// a command list of a saved scene (nngxStopCmdlistSave / nngxUseSavedCmdlist)
struct SavedCmdlist
{
    SavedCmdlist(); // 0x004ED614

    u32 cmdlist;        // 0x00
    u32 bufferOffset;   // 0x04
    s32 bufferSize;     // 0x08
    s32 requestOffset;  // 0x0C
    s32 requestCount;   // 0x10
};
ASSERT_SIZE(SavedCmdlist, 0x14);

// the camera of the scene being drawn (NULL: none)
extern nw::gfx::Camera* s_pCurrentCamera;
// the light sets and the scenes (a context with its traverser each)
const s32 LIGHT_SET_COUNT = 4;
const s32 SCENE_COUNT = 3;
extern nw::gfx::LightSet* s_pLightSets[LIGHT_SET_COUNT];
extern nw::gfx::SceneContext* s_pSceneContexts[SCENE_COUNT];
extern nw::gfx::SceneTraverser* s_pSceneTraversers[SCENE_COUNT];
// the graphics files of the engine: the shaders, the lookup tables
extern ResourceLoader* s_pShaderResource;
extern ResourceLoader* s_pLutResource;
// the lookup tables by name (LoadSystemResources)
extern sead::TreeMapImpl<nonameg3d::Char> s_LookupTables;
} // namespace g3d
