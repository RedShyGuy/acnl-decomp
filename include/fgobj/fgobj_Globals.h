#pragma once

#include "decomp.h"
#include "sead/seadNew.h"
#include "Item/dResLoader.h"
#include "ssys/ma/dHeapAllocator.h"

class SeadParticleHandle;
namespace sead {
class Heap;
} // namespace sead

// The globals of the field object code (ARMCC addresses those from 0x0094FE5C on over one base literal; only the ones the
// decompiled code uses so far; all names are ours).
namespace nw {
namespace gfx {
class MeshRenderer;
} // namespace gfx
} // namespace nw
namespace nn {
namespace math {
struct MTX34;
} // namespace math
} // namespace nn

namespace fgobj {
class ObjectBase;
class Proc;

// the textures of the designs (MyDesign) of the four players, ten each, for the pattern field objects (type unknown, only
// the field ObjectBase reads)
// the texture of the design of player (NULL without the design textures; name is ours)
const void* GetMyDesignTexture(u32 player, u32 design); // 0x005A32EC

struct MyDesignTextures
{
    // (names are ours) the number of players and of designs per player
    static const u32 PLAYER_NUM = 4;
    static const u32 DESIGN_NUM = 10;

    ssys::ma::HeapAllocator m_Allocator;    // 0x0000
    item::ResLoader m_Loaders[40];          // 0x0008 (player * 10 + design)
    u32 m_Addresses[40];                    // 0x2B28 the physical addresses of the textures (player * 10 + design)
};
ASSERT_SIZE(MyDesignTextures, 0x2BC8);

// (set by Proc::Draw: 0 / 1; meaning unknown) the shadows of the objects are registered while s_Unk94FE5D is set
extern u8 s_Unk94FE5C;
extern u8 s_Unk94FE5D;
// the draw functions of the objects by ObjectBase::m_Unk0x11D (DrawCached, DrawAll)
typedef void (ObjectBase::*DrawMethod)(nw::gfx::MeshRenderer* renderer, const nn::math::MTX34* view);
extern DrawMethod s_DrawMethods[2];
// the stage is indoors (stage::KIND_INDOOR) / outdoors (stage::KIND_OUTDOOR), set by the Proc
extern bool s_IsIndoor;
extern bool s_IsOutdoor;
// reset with s_pMagicLampObject
extern u8 s_Unk94FE60;
extern u8 s_Unk94FE61;
extern u8 s_Unk94FE62;
extern s16 s_Unk94FE64;
// the design textures of the Proc (NULL until they are created)
extern MyDesignTextures* s_pMyDesignTextures;
// the effect handles of the Proc (an array of PARTICLE_HANDLE_NUM)
extern SeadParticleHandle* s_pParticleHandles;
// the Proc (NULL: none)
extern Proc* s_pProc;
// the field object of the magic lamp outdoors (Proc::CreateFieldObject; it wobbles with UpdateWobble and the values
// below; cleared when the object is finalized)
extern ObjectBase* s_pMagicLampObject;
extern f32 s_Unk94FE78;
extern f32 s_Unk94FE7C;
// the material state of the last drawn object (-1 / NULL: none): the model byte and the material bits of its sort key,
// the mesh whose vertex attributes are active
extern s32 s_LastModelKey;
extern s32 s_LastMaterialKey;
extern const void* s_pLastMesh;
// the physical addresses of the textures treeA and treeA2 of the graphics file of the season (Proc::SetTreeTextureAddress;
// ObjectBase draws the trees with them)
extern u32 s_TreeTextureAddresses[2];
// (names are ours) the textures of the "sold out" signs of the shops by the region of the town (event::Region; the first
// is the one in the models): the normal and the wall signs, of the shop and of Gracie's shop (Proc::LoadItemResources)
const u32 REGION_TEXTURE_NUM = 10;
extern const char* s_SoldOutTextureNames[REGION_TEXTURE_NUM];
extern const char* s_GraceSoldOutTextureNames[REGION_TEXTURE_NUM];
extern const char* s_WallSoldOutTextureNames[REGION_TEXTURE_NUM];
extern const char* s_WallGraceSoldOutTextureNames[REGION_TEXTURE_NUM];
// the setting models of the item graphics file outdoors (Proc::m_pSettingModels; name is ours)
extern const char* s_SettingModelNames[2];
// the item models of the auto camp that get the fog (g3d::EnableFog)
const u32 FOG_MODEL_NUM = 3;
extern const char* s_FogModelNames[FOG_MODEL_NUM];

// the heaps of finalized objects for reuse: up to SMALL_HEAP_SIZE bytes / larger (NULL: free place)
const u32 PARTICLE_HANDLE_NUM = 20;
const u32 SMALL_HEAP_SIZE = 1728;
const u32 SMALL_HEAP_NUM = 550;
const u32 LARGE_HEAP_NUM = 20;
extern sead::Heap* s_SmallHeaps[SMALL_HEAP_NUM];
extern sead::Heap* s_LargeHeaps[LARGE_HEAP_NUM];

// (inline) a new object in the first free heap of the pool heaps (NULL if there is none); the heap is taken from the
// pool and becomes the heap of the allocator of the object (ObjectBase::Finalize gives it back)
template <typename T>
inline T* NewObject(sead::Heap** heaps, u32 num)
{
    for (u32 i = 0; i < num; i++) {
        sead::Heap* heap = heaps[i];
        if (heap != NULL) {
            heaps[i] = NULL;
            T* object = new (heap, 4) T();
            object->m_Allocator.SetHeap(heap);
            return object;
        }
    }
    return NULL;
}

// the offsets of weeds and shells from the centre of their unit, by z & 3 and x & 3 ({x, z})
extern const f32 s_ScatterOffsets[4][4][2];
} // namespace fgobj
