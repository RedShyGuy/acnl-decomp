#pragma once

#include "decomp.h"
#include "fgobj/dDrawListNode.h"
#include "g3d/dSkeletalModel.h"
#include "graphic/dShadow.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/st/dListNodePriorityBase.h"
#include "Item/item_Api.h"
#include "Sv/dSvFgName.h"

namespace nn {
namespace math {
struct MTX34;
} // namespace math
} // namespace nn
namespace nw {
namespace gfx {
class MeshRenderer;
class Model;
class SkeletalModel;
} // namespace gfx
} // namespace nw

namespace fgobj {
// the texture of the model of an item (libgarden; the enumerators after its names)
enum TextureID : u8
{
    TEXTURE_ID_DEFAULT,
    TEXTURE_ID_T_FG_OUTSIDE,     // the model "setting_t_outside" of the Proc
    TEXTURE_ID_T_FG_OUTSIDE_2,   // "setting_t_outside_2"
};

// the models of an item outdoors and indoors and their textures (libgarden)
struct ModelInfo
{
    const char* nameOutside;    // 0x0
    const char* nameInside;     // 0x4
    TextureID textureOutside;   // 0x8
    TextureID textureInside;    // 0x9
};
ASSERT_SIZE(ModelInfo, 0xC);

// by item::ModelID (libgarden)
extern const ModelInfo s_ModelInfos[];

// the name of the model of an item (models out of range: the first outdoor model)
const char* GetModelName(const item::ModelID& modelID, bool isOutside); // 0x0053678C
// the texture of the model of an item (inline; the name is ours)
DECOMP_EMIT_COPY inline TextureID GetTextureID(const item::ParamItemEntry* entry, bool isOutside) // 0x007492E4
{
    u32 id = entry->modelID;
    if (id >= item::MODEL_ID_COUNT) {
        id = 0;
    }
    return isOutside ? s_ModelInfos[id].textureOutside : s_ModelInfos[id].textureInside;
}

// A field object: the model of an item or field object on a unit (libgarden has only the size). The class name is from the
// RTTI; the members and the functions are ours.
// RTTI N5fgobj10ObjectBaseE @ 0x008D2910
// vtable 0x00908A3C (vptr 0x00908A44), offset_to_top 0, 8 entries
class ObjectBase : public ::ssys::st::ListNodePriorityBase
{
public:
    // m_Flags (names only where the code makes the meaning clear)
    enum Flag
    {
        FLAG_BUSH = 0x1,                    // a bush (with 0x600)
        FLAG_GROWN_TREE = 0x2,              // a grown tree (with 0x600): IsVisible also tests a sphere 60 above
        FLAG_UNK_4 = 0x4,                   // (Proc::RemoveObjectsAt)
        FLAG_UNK_8 = 0x8,
        FLAG_UNK_10 = 0x10,                 // flowers: the flag bit 14 of the item; StartEffect
        FLAG_GLITTERED = 0x20,              // the watered flower glittered (FlowerGlitter)
        FLAG_EFFECT = 0x40,                 // StartEffect started an effect (m_EffectIndex)
        FLAG_UNK_80 = 0x80,                 // StartEffect starts the effect 249 instead of 248
        FLAG_EFFECT_DELAYED = 0x100,        // StartEffect after the frames of m_Unk0x11A (StartWaterEffects)
        FLAG_UNK_200 = 0x200,
        FLAG_UNK_400 = 0x400,
        FLAG_ALPHA_STATE = 0x800,           // drawn with the render state of PutAlphaRenderState, which is reset after it
        FLAG_FACE_CAMERA = 0x1000,          // turned about y to the camera
        FLAG_UNK_2000 = 0x2000,             // (Proc::PlaceDeco)
        FLAG_HAS_SHADOW = 0x4000,
        FLAG_SHADOW_SET = 0x8000,
        FLAG_ALWAYS_VISIBLE = 0x10000,
        FLAG_UNK_20000 = 0x20000,
        FLAG_UNK_40000 = 0x40000,           // the lucky clovers NAME_LUCKY_CLOVERS_2959 get the model 4LEAF_CLOVER
        FLAG_ALPHA_FROM_TEXTURE = 0x80000,  // PutAlphaRenderState: the combiner takes the colour from texture 0
        FLAG_CURVED = 0x100000,             // outdoors: the matrix is curved from the camera (rows near the camera; Proc::Calc)
        FLAG_FTR = 0x200000,                // registered with ModuleFtr (Func150200 when finalized)
        FLAG_BLOCK_HANDLE = 0x400000,       // m_BlockHandle is set (UpdateBlockHandle)
        FLAG_MODULE_SCALE = 0x2000000,      // indoors: the scale comes from a module (Func150238; Proc::Calc)
        FLAG_SKIP_VISIBLE = 0x4000000,      // IsVisible returns false once
        FLAG_BLOCK_UNK = 0x8000000,         // Func7529D0 of the entry of m_BlockHandle
    };

    // the bits of the sort key above the three bytes z, model and material (SetSortKey)
    enum SortKeyBit
    {
        SORT_KEY_OWN_MATERIAL = 1 << 24,        // the material byte is not the model byte
        SORT_KEY_UNK_25 = 1 << 25,
        SORT_KEY_UNK_26 = 1 << 26,
        SORT_KEY_NO_MATERIAL = 1 << 27,         // the material byte is -1: the material is always activated
        SORT_KEY_TEXTURE_SETTING = 1 << 28,     // the material comes from the setting model of m_TextureSetting
    };

    // a function called before the shape of the object is drawn (m_pDrawFunc)
    typedef void (*DrawFunc)(ObjectBase* object, nw::gfx::MeshRenderer* renderer);

    ObjectBase(); // 0x00591A04
    virtual ~ObjectBase(); // 0x00591BA0 slot 0x00
    // 0x00591B70 (deleting dtor)
    // the object goes in front of other when its sort key is not larger
    bool IsBefore(const ::ssys::st::ListNode* other) const override; // 0x005908B0 slot 0x08
    virtual bool vf_0x10(); // 0x00590428 slot 0x10
    // stops the effect, unregisters from ModuleFtr and gives the heap back to the pool of the heaps (s_SmallHeaps /
    // s_LargeHeaps)
    virtual void Finalize(); // 0x005915C0 slot 0x14
    // the direction of the fall of a falling object (ObjectFall; 255: none)
    virtual u8 GetFallDirection(); // 0x005902F8 slot 0x18
    // a falling object falls onto the neighbour unit of direction (255: the one of its own direction; ObjectFall)
    virtual void StartFall(u8 direction); // 0x00590300 slot 0x1C

    // the object of an item at the unit x, z: the model of the item, a crack for buried items, a hole, the model of a field
    // object or the wrapping
    void Setup(const SvFgName& item, s32 x, s32 z, bool useObjectHeight); // 0x00591208
    // the model of an item (IsUsual) at the unit x, z
    void SetupItem(s32 x, s32 z, bool useObjectHeight); // 0x00590040
    // creates the model and puts the object on the unit (useObjectHeight: the height from Func4EA670)
    void Initialize(const void* model, s32 x, s32 z, bool useObjectHeight, u32 bufferOption, bool isAnimationEnabled); // 0x00590588
    // the object on another unit
    void SetPosition(s32 x, s32 z, bool useObjectHeight); // 0x00591874
    // the position on the curve of the world and the matrix of the model (yOffset is added to the height)
    void UpdateMatrix(bool isCurvedFromCamera, f32 yOffset); // 0x005916FC
    // the sort key: z, the model and the material byte and the bits SORT_KEY_*
    DECOMP_NOINLINE void SetSortKey(s32 z, s32 model, s32 material, bool ownMaterial, bool bit25, bool noBit26, bool useTextureSetting); // 0x005908C8
    // m_BlockHandle for the unit (or none)
    DECOMP_NOINLINE void UpdateBlockHandle(bool enable); // 0x00590720
    // the flags of the kind of field object
    void InitializeFlags(); // 0x00590304
    // the shadow of the field object or item
    void InitializeShadow(); // 0x00590934
    // m_pDrawFunc and m_DrawValue of the kind of field object
    void InitializeDrawFunc(); // 0x00590A7C
    // the sound when the item lands (isNormal: else the small one, none for coins, money bags and cans)
    void PlayLandSound(bool isNormal); // 0x00590430
    // starts the effect 248 / 249 at the position with a free handle of the Proc
    void StartEffect(); // 0x00590200
    // whether the object is in the view of the camera
    bool IsVisible(); // 0x005918FC
    // the normal matrix of the model for the view (and the view of the skeleton)
    void UpdateViewMatrix(const nn::math::MTX34* view); // 0x0059104C
    // draws the first mesh of the model; the material and the vertex attributes are only activated when they change
    // (s_LastMaterialKey / s_LastModelKey)
    void DrawCached(nw::gfx::MeshRenderer* renderer, const nn::math::MTX34* view); // 0x00590D80
    // draws all meshes of the model and resets the cached state
    void DrawAll(nw::gfx::MeshRenderer* renderer, const nn::math::MTX34* view); // 0x00591140
    // the render state of objects with FLAG_ALPHA_STATE: alpha test, blending and the last combiner stage with the alpha
    // m_Alpha (no depth writes below 128)
    void PutAlphaRenderState(); // 0x00590798

    // the draw functions of InitializeDrawFunc: flowers fade to 128 / wilted flowers to 178 (m_DrawValue is the alpha,
    // FLAG_UNK_10 starts the fade), patterns get the texture of their design, trees one of two textures
    static void DrawFuncFlower(ObjectBase* object, nw::gfx::MeshRenderer* renderer); // 0x00590D20
    static void DrawFuncWiltedFlower(ObjectBase* object, nw::gfx::MeshRenderer* renderer); // 0x00590FA0
    static void DrawFuncPattern(ObjectBase* object, nw::gfx::MeshRenderer* renderer); // 0x00590CC4
    static void DrawFuncTree(ObjectBase* object, nw::gfx::MeshRenderer* renderer); // 0x00591000

    DrawListNode m_DrawNode;                    // 0x00C
    f32 m_ShadowSize;                           // 0x01C
    ssys::ma::HeapAllocator m_Allocator;        // 0x020 the heap of the model
    s32 m_X;                                    // 0x028 the unit (-1: none)
    s32 m_Z;                                    // 0x02C
    s32 m_BlockHandle;                          // 0x030 (-1: none; UpdateBlockHandle)
    nn::math::VEC3 m_UnitPosition;              // 0x034 the centre of the unit with its height
    nn::math::VEC3 m_Position;                  // 0x040
    nn::math::VEC3 m_DrawPosition;              // 0x04C on the curve of the world
    g3d::SkeletalModel m_Model;                 // 0x058
    SvFgName m_Item;                            // 0x064
    u32 m_Flags;                                // 0x068 Flag
    math::Angle16 m_CurveAngle;                 // 0x06C the angle of the curve of the world at the position
    s16 m_RotateY;                              // 0x06E
    s16 m_RotateZ;                              // 0x070
    u32 m_SortKey;                              // 0x074 SetSortKey
    DrawFunc m_pDrawFunc;                       // 0x078 (NULL: none)
    graphic::Shadow m_Shadow;                   // 0x07C
    nw::gfx::Model* m_pModel;                   // 0x108 of m_Model
    nw::gfx::SkeletalModel* m_pSkeletalModel;   // 0x10C of m_Model (NULL: none)
    s32 m_Unk0x110;                             // 0x110
    s32 m_Unk0x114;                             // 0x114
    s16 m_FtrHandle;                            // 0x118 of ModuleFtr indoors (-1: none)
    u8 m_Unk0x11A;                              // 0x11A
    u8 m_Unk0x11B;                              // 0x11B (StartEffect: 200)
    u8 m_Unk0x11C;                              // 0x11C
    u8 m_Unk0x11D;                              // 0x11D
    u8 m_DrawValue;                             // 0x11E the value of m_pDrawFunc (alpha, design or texture)
    s8 m_EffectIndex;                           // 0x11F the effect handle of StartEffect (-1: none)
    u8 m_Alpha;                                 // 0x120 the alpha of PutAlphaRenderState (255; the stumps fade with it)
    u8 m_FadeFrame;                             // 0x121 frames until a falling tree has faded (ObjectSwing)
    u8 m_ShadowKind;                            // 0x122
    u8 m_TextureSetting;                        // 0x123 the setting model (SORT_KEY_TEXTURE_SETTING): TextureID - 1
};
ASSERT_OFFSET(ObjectBase, m_DrawNode, 0xC);
ASSERT_OFFSET(ObjectBase, m_Allocator, 0x20);
ASSERT_OFFSET(ObjectBase, m_Model, 0x58);
ASSERT_OFFSET(ObjectBase, m_Item, 0x64);
ASSERT_OFFSET(ObjectBase, m_Shadow, 0x7C);
ASSERT_OFFSET(ObjectBase, m_pModel, 0x108);
ASSERT_OFFSET(ObjectBase, m_FtrHandle, 0x118);
ASSERT_SIZE(ObjectBase, 0x124);
} // namespace fgobj
