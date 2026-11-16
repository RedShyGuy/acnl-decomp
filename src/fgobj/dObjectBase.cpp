#include "fgobj/dObjectBase.h"
#include "Camera/dCameraMgr.h"
#include "Other/dSeadParticleHandle.h"
#include "Other/dWorldCurve.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvMail.h"
#include "fgobj/dProc.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/fgobj_Globals.h"
#include "field/dFdInfo.h"
#include "g3d/g3d_System.h"
#include "nn/gr/CTR/gr_Combiner.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gx/gx_Api.h"
#include "nn/math/math_Api.h"
#include "nw/gfx/gfx_Api.h"
#include "nw/gfx/gfx_MeshRenderer.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "nw/math/math_SPHERE.h"
#include "sead/seadHeap.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"

namespace fgobj {
namespace {
// the stage where items lie 16 further in x (names are ours)
const stage::Name STAGE_ITEMS_SHIFTED = stage::NAME_RECYCLE_SHOP;
// the effects of StartEffect (meaning unknown)
const u32 EFFECT_ID_DEFAULT = 248;
const u32 EFFECT_ID_FLAG_80 = 249;
// the size of the shadow of an item
const f32 ITEM_SHADOW_SIZE = 8.0f;
// the alpha of the flowers (faded / normal)
const u8 FLOWER_ALPHA_FADED = 128;
const u8 FLOWER_ALPHA_NORMAL = 178;
const u8 FLOWER_ALPHA_STEP = 5;
// the designs of the players (four players with ten designs each)
const u32 DESIGN_NUM = 40;
// the model bytes of the sort keys of cracks and holes (after the model indices)
const s32 SORT_MODEL_CRACK_SOIL_PITFALL = 251;
const s32 SORT_MODEL_CRACK_SAND_PITFALL = 252;
const s32 SORT_MODEL_CRACK_SOIL = 253;
const s32 SORT_MODEL_CRACK_SAND = 254;
const s32 SORT_MODEL_HOLE_SOIL = 249;
const s32 SORT_MODEL_HOLE_SAND = 250;
const s32 SORT_MODEL_WRAPPING = 253;
// Func300B7C: the unit is sand
const s32 GROUND_SAND = 1;
// the parameter of nngxGetCmdlistParameteri for the current command address
const GLenum CMDLIST_CURRENT_ADDRESS = 0x208;
// PICA command headers (registers after 3dbrew): GPUREG_TEXUNIT0_ADDR1 / GPUREG_TEXUNIT0_PARAM with all bytes, and
// GPUREG_TEXENV0_COLOR (the header of the two 3D commands of the flowers)
const u32 COMMAND_TEXUNIT0_ADDR1 = 0x000F0085;
const u32 COMMAND_TEXUNIT0_PARAM = 0x000F0083;
const u32 COMMAND_TEXENV0_COLOR = 0x800F00C3;
// the value of GPUREG_TEXUNIT0_PARAM for patterns (linear magnification and minification)
const u32 PATTERN_TEXTURE_PARAM = 6;
// all material hashes of the render context (g3d::ResetRenderState)
const s32 RESET_ALL_HASHES = 0x1FFF;
// the radii of the spheres of IsVisible and the height of the sphere of the grown trees
const f32 VISIBLE_RADIUS = 32.0f;
const f32 TREE_VISIBLE_RADIUS = 64.0f;
const f32 TREE_VISIBLE_HEIGHT = 60.0f;

// writes one register command into the current command buffer
inline void PutCommand(u32 value, u32 header)
{
    u32* command;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    u32* start = command;
    command[0] = value;
    command[1] = header;
    command += 2;
    nngxMoveCommandbufferPointer((reinterpret_cast<uptr>(command) - reinterpret_cast<uptr>(start)) & ~3u);
}

// the effect handle at index (NULL out of range)
inline SeadParticleHandle* GetParticleHandle(s32 index)
{
    if (index >= 0 && index < static_cast<s32>(PARTICLE_HANDLE_NUM)) {
        return &s_pParticleHandles[index];
    }
    return NULL;
}

// the Proc outdoors (NULL indoors or without it)
inline Proc* GetOutdoorProc()
{
    return (s_IsOutdoor && s_pProc != NULL) ? s_pProc : NULL;
}

// a model of the graphics file of the items (NULL without the Proc)
inline const void* GetItemModel(const char* name)
{
    if (s_pProc == NULL) {
        return NULL;
    }
    return nw::gfx::GetModel(reinterpret_cast<const void* const*>(&s_pProc->m_ItemLoader.m_GraphicsFile), name);
}

// the vertex attributes of the last mesh are deactivated
inline void DeactivateLastMesh(nw::gfx::MeshRenderer* renderer)
{
    if (s_pLastMesh != NULL) {
        renderer->DeactivateVertexAttribute(nw::gfx::res::ResMesh(s_pLastMesh));
        s_pLastMesh = NULL;
    }
}

// the 3D command that sets the alpha of the constant colour of the first combiner stage
inline void PutFlowerAlpha(u32* command, u32 alpha)
{
    command[0] = 0x00FFFFFF | (alpha << 24);
    nngxAdd3DCommand(command, 8, true);
}
} // namespace

// 0x00591A04 (name is ours)
fgobj::ObjectBase::ObjectBase()
    : m_ShadowSize(0.0f), m_X(-1), m_Z(-1), m_BlockHandle(-1), m_Flags(0), m_CurveAngle(), m_RotateY(0), m_RotateZ(0), m_SortKey(0),
      m_pDrawFunc(NULL), m_pModel(NULL), m_pSkeletalModel(NULL), m_Unk0x110(-1), m_Unk0x114(-1), m_FtrHandle(-1),
      m_Unk0x11A(0), m_Unk0x11B(0), m_Unk0x11C(0), m_Unk0x11D(0), m_DrawValue(0), m_EffectIndex(-1), m_Alpha(255),
      m_FadeFrame(0), m_ShadowKind(0), m_TextureSetting(0)
{
    m_DrawNode.m_pObject = this;
    if (s_IsIndoor) {
        m_FtrHandle = Func150210(this);
    }
}

// 0x00591BA0 (name is ours)
// 0x00591B70 (deleting dtor)
fgobj::ObjectBase::~ObjectBase()
{
    // (only the members are destroyed)
}

// 0x005908B0 slot 0x08 (name is ours)
bool fgobj::ObjectBase::IsBefore(const ::ssys::st::ListNode* other) const
{
    return static_cast<const ObjectBase*>(other)->m_SortKey >= m_SortKey;
}

// 0x00590428 slot 0x10
bool fgobj::ObjectBase::vf_0x10()
{
    return false;
}

// 0x005915C0 slot 0x14 (name is ours)
void fgobj::ObjectBase::Finalize()
{
    if (s_pMagicLampObject == this) {
        s_pMagicLampObject = NULL;
        s_Unk94FE64 = 0;
        s_Unk94FE78 = 0.0f;
        s_Unk94FE60 = 0;
        s_Unk94FE7C = 0.0f;
        s_Unk94FE61 = 0;
        s_Unk94FE62 = 0;
    }
    if ((m_Flags & FLAG_EFFECT) && m_EffectIndex != -1) {
        SeadParticleHandle* handle = GetParticleHandle(m_EffectIndex);
        if (handle != NULL && handle->GetEmitterSet() != NULL) {
            handle->Kill();
        }
    }
    m_EffectIndex = -1;
    if (m_Flags & FLAG_FTR) {
        Func150200(this);
        m_Flags &= ~FLAG_FTR;
    }
    if (s_IsIndoor) {
        Func150208(m_FtrHandle);
    }
    sead::Heap* heap = m_Allocator.m_pHeap;
    heap->freeAll();
    sead::Heap** heaps;
    u32 num;
    if (heap->getSize() <= SMALL_HEAP_SIZE) {
        heaps = s_SmallHeaps;
        num = SMALL_HEAP_NUM;
    } else {
        heaps = s_LargeHeaps;
        num = LARGE_HEAP_NUM;
    }
    for (u32 i = 0; i < num; i++) {
        if (heaps[i] == NULL) {
            heaps[i] = heap;
            return;
        }
    }
}

// 0x005902F8 slot 0x18 (name is ours)
u8 fgobj::ObjectBase::GetFallDirection()
{
    return 255;
}

// 0x00590300 slot 0x1C (name is ours)
void fgobj::ObjectBase::StartFall(u8)
{
    // empty (in the original too)
}

// 0x00591208 (name is ours)
void fgobj::ObjectBase::Setup(const SvFgName& item, s32 x, s32 z, bool useObjectHeight)
{
    m_Item = item;
    if (item.IsBuried()) {
        s32 sortModel = SORT_MODEL_CRACK_SAND;
        const void* model;
        if (Func300B7C(x, z, false) == GROUND_SAND) {
            if (m_Item.IsName(SvFgName::NAME_TRICKY_PITFALL_SEED)) {
                model = GetFgModel("fg_crack_sand_s");
                sortModel = SORT_MODEL_CRACK_SAND_PITFALL;
            } else {
                model = GetFgModel("fg_crack_sand");
            }
        } else if (m_Item.IsName(SvFgName::NAME_TRICKY_PITFALL_SEED)) {
            model = GetFgModel("fg_crack_soil_s");
            sortModel = SORT_MODEL_CRACK_SOIL_PITFALL;
        } else {
            model = GetFgModel("fg_crack_soil");
            sortModel = SORT_MODEL_CRACK_SOIL;
        }
        Initialize(model, x, z, false, 0, false);
        SetSortKey(z, sortModel, 255, false, true, false, false);
        return;
    }
    if (item.IsName(SvFgName::NAME_HOLE)) {
        s32 sortModel = SORT_MODEL_HOLE_SAND;
        const void* model;
        if (Func300B7C(x, z, false) == GROUND_SAND) {
            model = GetFgModel("fg_hole_sand");
        } else {
            model = GetFgModel("fg_hole_soil");
            sortModel = SORT_MODEL_HOLE_SOIL;
        }
        Initialize(model, x, z, false, 0, false);
        SetSortKey(z, sortModel, 255, false, true, false, false);
        m_Flags |= FLAG_UNK_20000;
        return;
    }
    if (item.IsFg()) {
        u32 index = item::GetParamFgEntry(m_Item)->fgModel;
        Proc* proc = GetOutdoorProc();
        if (index >= Proc::FG_MODEL_NUM) {
            index = 0;
        }
        if (proc == NULL) {
            return;
        }
        FgModel* fgModel;
        if (proc->GetFgModels() != NULL && index < Proc::FG_MODEL_NUM) {
            fgModel = &proc->GetFgModels()[index];
        } else {
            fgModel = NULL;
        }
        if (fgModel == NULL) {
            return;
        }
        fgModel->nameEnd = 0;
        const void* model = GetFgModel(fgModel->name);
        if (model == NULL) {
            return;
        }
        Initialize(model, x, z, false, 0, false);
        s32 material = fgModel->material;
        bool ownMaterial = true;
        if (material == 0) {
            ownMaterial = false;
            material = index;
        }
        SetSortKey(z, index, material, ownMaterial, true, false, false);
        return;
    }
    if (item.IsUsual()) {
        SetupItem(x, z, useObjectHeight);
        return;
    }
    if (Func769190(&item)) {
        const void* model;
        if (s_IsOutdoor) {
            model = GetItemModel("fg_wrapping_out");
        } else {
            model = GetItemModel("fg_wrapping");
        }
        Initialize(model, x, z, useObjectHeight, 0, false);
        SetSortKey(z, SORT_MODEL_WRAPPING, SORT_MODEL_WRAPPING, false, false, false, false);
    }
}

// 0x00590040 (name is ours)
void fgobj::ObjectBase::SetupItem(s32 x, s32 z, bool useObjectHeight)
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(m_Item);
    item::ModelID modelID = entry->modelID < item::MODEL_ID_COUNT ? static_cast<item::ModelID>(entry->modelID)
                                                                  : item::MODEL_ID_DEFAULT;
    if ((m_Flags & FLAG_UNK_40000) && m_Item.IsName(SvFgName::NAME_LUCKY_CLOVERS_2959)) {
        modelID = item::MODEL_ID_4LEAF_CLOVER;
    }
    if (m_Item.Func76D664()) {
        modelID = m_Item.Func3132A8() ? item::MODEL_ID_LEAF_M : item::MODEL_ID_LEAF_R;
    }
    if (m_Item.IsName(SvFgName::NAME_NOOKLINGS_SOLD_OUT_SIGN_WALL) ||
        m_Item.IsName(SvFgName::NAME_GRACIE_SOLD_OUT_SIGN_WALL)) {
        m_Flags |= FLAG_ALWAYS_VISIBLE;
    }
    const char* name = GetModelName(modelID, s_IsOutdoor);
    const void* model = GetItemModel(name);
    s32 lightSetIndex = Func1E8920();
    if (lightSetIndex != 0) {
        g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model), lightSetIndex);
    }
    Initialize(model, x, z, useObjectHeight, 0, false);
    const s32 modelIndex = modelID;
    s32 texture = GetTextureID(entry, s_IsOutdoor);
    if (modelID == item::MODEL_ID_4LEAF_CLOVER) {
        texture = TEXTURE_ID_T_FG_OUTSIDE_2;
    }
    bool ownMaterial = true;
    bool useTextureSetting = false;
    if (s_IsOutdoor && texture == TEXTURE_ID_T_FG_OUTSIDE) {
        useTextureSetting = true;
        m_TextureSetting = 0;
    } else if (s_IsOutdoor && texture == TEXTURE_ID_T_FG_OUTSIDE_2) {
        useTextureSetting = true;
        m_TextureSetting = 1;
    } else if (texture == TEXTURE_ID_DEFAULT) {
        texture = modelIndex;
        ownMaterial = false;
    }
    SetSortKey(z, modelIndex, texture, ownMaterial, false, false, useTextureSetting);
}

// 0x00590588 (name is ours)
void fgobj::ObjectBase::Initialize(const void* model, s32 x, s32 z, bool useObjectHeight, u32 bufferOption,
                                   bool isAnimationEnabled)
{
    m_Model.Create(nw::gfx::res::ResSceneObject(model), &m_Allocator, &m_Allocator, bufferOption, isAnimationEnabled, 1);
    m_pModel = m_Model.GetModel();
    m_pSkeletalModel = m_Model.GetSkeletalModel();
    m_X = x;
    m_Z = z;
    FdInfo::ToCenterWorldWithUnitHeight(&m_UnitPosition, x, z);
    m_Position = m_UnitPosition;
    if (useObjectHeight) {
        m_Position.y = Func4EA670(x, z);
    }
    const stage::Name current = stage::GetCurrent();
    if (current == STAGE_ITEMS_SHIFTED || stage::IsKind(stage::KIND_AUTO_CAMP, current)) {
        m_Position.x += 16.0f;
    } else if (s_IsOutdoor && !m_Item.IsBuried()) {
        const u8 fgKind = m_Item.GetFgKind();
        const item::Kind kind = m_Item.GetKind();
        if (fgKind == SvFgName::FG_KIND_WEED || kind == item::KIND_SHELL) {
            const f32* offset = s_ScatterOffsets[z & 3][x & 3];
            m_Position.x += offset[0];
            m_Position.z += offset[1];
        }
    }
    m_RotateY = 0;
    m_RotateZ = 0;
    UpdateMatrix(false, 0.0f);
    InitializeFlags();
    InitializeDrawFunc();
}

// 0x00591874 (name is ours)
void fgobj::ObjectBase::SetPosition(s32 x, s32 z, bool useObjectHeight)
{
    m_X = x;
    m_Z = z;
    FdInfo::ToCenterWorldWithUnitHeight(&m_UnitPosition, x, z);
    m_Position = m_UnitPosition;
    if (useObjectHeight) {
        m_Position.y = Func4EA670(x, z);
        UpdateBlockHandle(true);
    } else {
        UpdateBlockHandle(false);
    }
    UpdateMatrix(false, 0.0f);
}

// 0x005916FC (name is ours)
void fgobj::ObjectBase::UpdateMatrix(bool isCurvedFromCamera, f32 yOffset)
{
    nn::math::VEC3 position(m_Position.x, m_Position.y + yOffset, m_Position.z);
    if (isCurvedFromCamera) {
        m_CurveAngle = GetWorldCurvePositionFromCamera(&m_DrawPosition, position);
    } else {
        m_CurveAngle = GetWorldCurvePosition(&m_DrawPosition, position);
    }
    nn::math::MTX34 matrix(1.0f, 0.0f, 0.0f, m_DrawPosition.x, 0.0f, 1.0f, 0.0f, m_DrawPosition.y, 0.0f, 0.0f, 1.0f,
                           m_DrawPosition.z);
    ssys::ma::RotateX(&matrix, m_CurveAngle.value);
    if (m_Flags & FLAG_FACE_CAMERA) {
        ssys::ma::RotateY(&matrix, camera::Mgr::s_pInstance->m_pUnk0x1A4->m_RotateY);
    } else {
        ssys::ma::RotateY(&matrix, m_RotateY);
    }
    ssys::ma::RotateZ(&matrix, m_RotateZ);
    m_Model.SetMatrix(matrix);
    m_Model.Update();
    if (m_Flags & FLAG_HAS_SHADOW) {
        m_Shadow.Set(&m_DrawPosition, m_CurveAngle.value, true, m_ShadowKind, 0, m_ShadowSize, m_ShadowSize, 0.0f);
        m_Flags |= FLAG_SHADOW_SET;
    }
}

// 0x005908C8 (name is ours)
void fgobj::ObjectBase::SetSortKey(s32 z, s32 model, s32 material, bool ownMaterial, bool bit25, bool noBit26,
                                   bool useTextureSetting)
{
    m_SortKey = static_cast<u8>(z) | (static_cast<u8>(model) << 8) | (static_cast<u8>(material) << 16);
    if (ownMaterial) {
        m_SortKey |= SORT_KEY_OWN_MATERIAL;
    }
    if (bit25) {
        m_SortKey |= SORT_KEY_UNK_25;
    }
    if (!noBit26) {
        m_SortKey |= SORT_KEY_UNK_26;
    }
    if (material == -1) {
        m_SortKey |= SORT_KEY_NO_MATERIAL;
    }
    if (useTextureSetting) {
        m_SortKey |= SORT_KEY_TEXTURE_SETTING;
    }
}

// 0x00590720 (name is ours)
void fgobj::ObjectBase::UpdateBlockHandle(bool enable)
{
    if (enable) {
        m_Flags |= FLAG_BLOCK_HANDLE;
        m_BlockHandle = Func7464A0(Func4E1338(), m_X, m_Z, 0);
        const void* entry = Func4E9C14(Func4EA6D0(), m_BlockHandle);
        if (entry != NULL) {
            if (Func7529D0(entry)) {
                m_Flags |= FLAG_BLOCK_UNK;
            } else {
                m_Flags &= ~FLAG_BLOCK_UNK;
            }
        }
    } else {
        m_Flags &= ~(FLAG_BLOCK_HANDLE | FLAG_BLOCK_UNK);
        m_BlockHandle = -1;
    }
}

// 0x00590304 (name is ours)
void fgobj::ObjectBase::InitializeFlags()
{
    if (!m_Item.IsFg()) {
        return;
    }
    const u8 fgKind = m_Item.GetFgKind();
    if (SvFgName::IsGrownTreeFgKind(fgKind)) {
        m_Flags |= FLAG_UNK_200 | FLAG_UNK_400 | FLAG_GROWN_TREE;
        return;
    }
    if (SvFgName::IsBushFgKind(fgKind)) {
        m_Flags |= FLAG_UNK_200 | FLAG_UNK_400 | FLAG_BUSH;
        return;
    }
    if (SvFgName::IsFlowerFgKind(fgKind) || fgKind == SvFgName::FG_KIND_WILTED_FLOWER) {
        if (m_Item.IsWatered()) {
            m_Flags |= FLAG_UNK_10;
        }
        m_Unk0x11C = 0;
        m_Flags |= FLAG_UNK_8;
        m_Unk0x11B = 0;
        return;
    }
    if (fgKind == SvFgName::FG_KIND_WEED || SvFgName::IsSaplingFgKind(fgKind) || fgKind == SvFgName::FG_KIND_RAFFLESIA ||
        fgKind == SvFgName::FG_KIND_WILTED_PLANT) {
        m_Unk0x11C = 0;
        m_Flags |= FLAG_UNK_8;
        return;
    }
    if (m_Item.Func769DE0(true)) {
        m_Flags |= FLAG_UNK_400;
    }
}

// 0x00590934 (name is ours)
void fgobj::ObjectBase::InitializeShadow()
{
    if (m_Item.IsBuried()) {
        return;
    }
    if (m_Item.IsFg()) {
        Proc* proc = GetOutdoorProc();
        if (proc == NULL || !m_Item.IsFg()) {
            return;
        }
        const u32 index = item::GetParamFgEntry(m_Item)->fgModel;
        const FgModel* fgModel;
        if (index >= Proc::FG_MODEL_NUM) {
            fgModel = NULL;
        } else {
            if (index == 0) {
                return;
            }
            fgModel = proc->GetFgModels() != NULL ? &proc->GetFgModels()[index] : NULL;
        }
        if (fgModel == NULL || fgModel->shadowSize <= 0) {
            return;
        }
        m_ShadowSize = fgModel->shadowSize;
        m_Flags |= FLAG_HAS_SHADOW;
        switch (fgModel->shadowKind) {
        case 1:
            m_ShadowKind = 2;
            break;
        case 2:
            m_ShadowKind = 3;
            break;
        case 3:
            m_ShadowKind = 4;
            break;
        default:
            m_ShadowKind = 0;
            break;
        }
        return;
    }
    if (m_Item.IsUsual() || Func769190(&m_Item)) {
        m_Flags |= FLAG_HAS_SHADOW;
        m_ShadowSize = ITEM_SHADOW_SIZE;
        m_ShadowKind = 0;
    }
}

// 0x00590A7C (name is ours)
void fgobj::ObjectBase::InitializeDrawFunc()
{
    m_Unk0x11D = 0;
    m_pDrawFunc = NULL;
    const u8 fgKind = m_Item.GetFgKind();
    if (SvFgName::IsFlowerFgKind(fgKind)) {
        m_pDrawFunc = DrawFuncFlower;
        m_DrawValue = (m_Flags & FLAG_UNK_10) ? FLOWER_ALPHA_FADED : FLOWER_ALPHA_NORMAL;
        return;
    }
    if (fgKind == SvFgName::FG_KIND_WILTED_FLOWER) {
        m_pDrawFunc = DrawFuncWiltedFlower;
        m_DrawValue = (m_Flags & FLAG_UNK_10) ? FLOWER_ALPHA_NORMAL : FLOWER_ALPHA_FADED;
        return;
    }
    if (m_Item.IsPattern()) {
        m_pDrawFunc = DrawFuncPattern;
        // the scale of a pattern on sand / elsewhere
        // 0x0098B458 (guard 0x0094FE8C)
        static const nn::math::VEC3 s_PatternScaleSand(1.0f, 1.0f, 0.86f);
        // 0x0098B464 (guard 0x0094FE88)
        static const nn::math::VEC3 s_PatternScale(1.0f, 1.0f, 1.01f);
        if (Func300B7C(m_X, m_Z, true) == GROUND_SAND) {
            m_Model.SetScale(s_PatternScaleSand);
        } else {
            m_Model.SetScale(s_PatternScale);
        }
        m_Model.Update();
        u8 design = m_Item.Func76CAE0() + m_Item.Func76C1FC() * 10;
        if (design >= DESIGN_NUM) {
            design = 0;
        }
        m_DrawValue = design;
        m_Flags |= FLAG_UNK_20000;
        return;
    }
    if (fgKind == SvFgName::FG_KIND_TREE) {
        m_DrawValue = (item::GetParamFgEntry(m_Item)->unk_0xF & 4) ? 1 : 0;
        m_pDrawFunc = DrawFuncTree;
        return;
    }
    if (m_Item.GetKind() == item::KIND_DUMMY_WALL_2) {
        m_Unk0x11D = 1;
        m_Flags |= FLAG_UNK_20000;
    }
}

// 0x00590D20 (name is ours)
void fgobj::ObjectBase::DrawFuncFlower(ObjectBase* object, nw::gfx::MeshRenderer*)
{
    u32 alpha;
    if (!(object->m_Flags & FLAG_UNK_10)) {
        alpha = FLOWER_ALPHA_NORMAL;
    } else {
        alpha = FLOWER_ALPHA_FADED;
        if (object->m_DrawValue > FLOWER_ALPHA_FADED) {
            const u8 value = object->m_DrawValue - FLOWER_ALPHA_STEP;
            if (value > FLOWER_ALPHA_FADED) {
                alpha = value;
            }
            object->m_DrawValue = value;
            if (value <= FLOWER_ALPHA_FADED) {
                object->m_DrawValue = FLOWER_ALPHA_FADED;
            }
        }
    }
    // 0x0094FEB4
    static u32 s_FlowerAlphaCommand[2] = {0, COMMAND_TEXENV0_COLOR};
    PutFlowerAlpha(s_FlowerAlphaCommand, alpha);
}

// 0x00590FA0 (name is ours)
void fgobj::ObjectBase::DrawFuncWiltedFlower(ObjectBase* object, nw::gfx::MeshRenderer*)
{
    u32 alpha;
    if (!(object->m_Flags & FLAG_UNK_10)) {
        alpha = FLOWER_ALPHA_FADED;
    } else {
        alpha = FLOWER_ALPHA_NORMAL;
        if (object->m_DrawValue < FLOWER_ALPHA_NORMAL) {
            const u8 value = object->m_DrawValue + FLOWER_ALPHA_STEP;
            if (value < FLOWER_ALPHA_NORMAL) {
                alpha = value;
            }
            object->m_DrawValue = value;
            if (value >= FLOWER_ALPHA_NORMAL) {
                object->m_DrawValue = FLOWER_ALPHA_NORMAL;
            }
        }
    }
    // 0x0094FEBC
    static u32 s_WiltedFlowerAlphaCommand[2] = {0, COMMAND_TEXENV0_COLOR};
    PutFlowerAlpha(s_WiltedFlowerAlphaCommand, alpha);
}

// 0x00590CC4 (name is ours)
void fgobj::ObjectBase::DrawFuncPattern(ObjectBase* object, nw::gfx::MeshRenderer*)
{
    const u32 address = s_pMyDesignTextures->m_Addresses[object->m_DrawValue];
    PutCommand(address >> 3, COMMAND_TEXUNIT0_ADDR1);
}

// 0x00591000 (name is ours)
void fgobj::ObjectBase::DrawFuncTree(ObjectBase* object, nw::gfx::MeshRenderer*)
{
    const u32 address = s_TreeTextureAddresses[object->m_DrawValue];
    PutCommand(address >> 3, COMMAND_TEXUNIT0_ADDR1);
}

// 0x00590430 (name is ours)
void fgobj::ObjectBase::PlayLandSound(bool isNormal)
{
    if (isNormal) {
        u32 sound = SE_OBJ_ITEM_CMN_LAND;
        if (!Func769190(&m_Item)) {
            if (!m_Item.IsUsual()) {
                return;
            }
            const item::ModelID modelID = m_Item.GetModelID();
            const item::Kind kind = m_Item.GetKind();
            if (modelID == item::MODEL_ID_COIN) {
                sound = SE_OBJ_ITEM_COIN_LAND;
            } else if (modelID == item::MODEL_ID_MONEYBAG) {
                sound = SE_OBJ_ITEM_MONEY_LAND;
            } else if (kind == item::KIND_DUST && modelID == item::MODEL_ID_CAN) {
                sound = SE_OBJ_ITEM_CAN_LAND;
            }
        }
        SoundMgr::SPlaySharedSound(sound, &m_Position, this, true);
        return;
    }
    if (m_Item.IsUsual() && (m_Item.GetModelID() == item::MODEL_ID_COIN || m_Item.GetModelID() == item::MODEL_ID_MONEYBAG ||
                             m_Item.GetModelID() == item::MODEL_ID_CAN)) {
        return;
    }
    SoundMgr::SPlaySharedSound(SE_OBJ_ITEM_CMN_LAND_S, &m_Position, this, true);
}

// 0x00590200 (name is ours)
void fgobj::ObjectBase::StartEffect()
{
    m_Flags |= FLAG_EFFECT | FLAG_UNK_10;
    m_Unk0x11B = 200;
    nn::math::VEC3 position = m_Position;
    s8 index;
    for (index = 0; index < static_cast<s32>(PARTICLE_HANDLE_NUM); index++) {
        SeadParticleHandle* handle = &s_pParticleHandles[index];
        if (handle->GetEmitterSet() == NULL || !handle->IsAlive()) {
            break;
        }
    }
    if (index >= static_cast<s32>(PARTICLE_HANDLE_NUM)) {
        index = -1;
    }
    m_EffectIndex = index;
    const u32 effectId = (m_Flags & FLAG_UNK_80) ? EFFECT_ID_FLAG_80 : EFFECT_ID_DEFAULT;
    if (index != -1) {
        Func207998(GetParticleHandle(index), effectId, &position, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
    }
}

// 0x005918FC (name is ours)
bool fgobj::ObjectBase::IsVisible()
{
    if (m_Flags & FLAG_SKIP_VISIBLE) {
        m_Flags &= ~FLAG_SKIP_VISIBLE;
        return false;
    }
    if (m_Flags & FLAG_ALWAYS_VISIBLE) {
        return true;
    }
    camera::Mgr* mgr = camera::Mgr::s_pInstance;
    if (m_Flags & FLAG_GROWN_TREE) {
        nw::math::SPHERE sphere;
        sphere.center.x = m_DrawPosition.x;
        sphere.center.y = m_DrawPosition.y + nn::math::CosIdx(m_CurveAngle.value) * TREE_VISIBLE_HEIGHT;
        sphere.center.z = m_DrawPosition.z + nn::math::SinIdx(m_CurveAngle.value) * TREE_VISIBLE_HEIGHT;
        sphere.radius = TREE_VISIBLE_RADIUS;
        if (mgr->m_Frustum.IntersectSphere(sphere)) {
            return true;
        }
    }
    nw::math::SPHERE sphere;
    sphere.center = m_DrawPosition;
    sphere.radius = VISIBLE_RADIUS;
    if (!mgr->m_Frustum.IntersectSphere(sphere)) {
        return false;
    }
    return true;
}

// 0x0059104C (name is ours)
void fgobj::ObjectBase::UpdateViewMatrix(const nn::math::MTX34* view)
{
    nw::gfx::SkeletalModel* skeletalModel = m_pSkeletalModel;
    if (skeletalModel != NULL) {
        nw::gfx::Skeleton* skeleton = skeletalModel->GetSkeleton();
        skeleton->m_IsWorldCalculated = false;
        if (!skeletalModel->m_IsSharingSkeleton) {
            g3d::UpdateSkeletonView(skeletalModel);
        }
        const bool isModelCoordinate =
            (nw::gfx::res::ResSkeleton(skeleton->GetResSceneObject().ptr()).ptr()->flags &
             nw::gfx::res::ResSkeletonData::FLAG_IS_MODEL_COORDINATE) != 0;
        m_pModel->UpdateNormalMatrix(*view, isModelCoordinate);
        return;
    }
    m_pModel->UpdateNormalMatrix(*view, false);
}

// 0x00590D80 (name is ours)
void fgobj::ObjectBase::DrawCached(nw::gfx::MeshRenderer* renderer, const nn::math::MTX34* view)
{
    nw::gfx::Model* model = m_pModel;
    UpdateViewMatrix(view);
    const u32 key = m_SortKey;
    const s32 material = (key << 4) >> 20;
    const s32 modelKey = (key & 0xFF00) >> 8;
    const nw::gfx::res::ResMesh mesh = model->GetResMeshes()[0];
    if (!(key & SORT_KEY_NO_MATERIAL) && material == s_LastMaterialKey) {
        if (modelKey == s_LastModelKey) {
            goto draw;
        }
        DeactivateLastMesh(renderer);
    } else {
        if (key & SORT_KEY_TEXTURE_SETTING) {
            nw::gfx::Model* settingModel;
            if (s_pProc != NULL && m_TextureSetting < 2) {
                settingModel = s_pProc->m_pSettingModels[m_TextureSetting];
                if (settingModel == NULL) {
                    settingModel = m_pModel;
                }
            } else {
                settingModel = m_pModel;
            }
            renderer->ActivateMaterial(settingModel->GetResMeshes()[0], settingModel);
        } else {
            renderer->ActivateMaterial(mesh, model);
        }
        if (m_Item.IsPattern()) {
            PutCommand(PATTERN_TEXTURE_PARAM, COMMAND_TEXUNIT0_PARAM);
        }
        s_LastMaterialKey = material;
        DeactivateLastMesh(renderer);
    }
    s_pLastMesh = mesh.ptr();
    s_LastModelKey = modelKey;
    renderer->ActivateVertexAttribute(mesh);
draw:
    if (m_pDrawFunc != NULL) {
        m_pDrawFunc(this, renderer);
    }
    if (m_Flags & FLAG_ALPHA_STATE) {
        PutAlphaRenderState();
    }
    renderer->RenderShape(mesh, model);
    if (m_Flags & FLAG_ALPHA_STATE) {
        s_LastModelKey = -1;
        s_LastMaterialKey = -1;
        s_pLastMesh = NULL;
        g3d::ResetRenderState(RESET_ALL_HASHES);
    }
}

// 0x00591140 (name is ours)
void fgobj::ObjectBase::DrawAll(nw::gfx::MeshRenderer* renderer, const nn::math::MTX34* view)
{
    nw::gfx::Model* model = m_pModel;
    UpdateViewMatrix(view);
    s_LastModelKey = -1;
    s_LastMaterialKey = -1;
    s_pLastMesh = NULL;
    g3d::ResetRenderState(RESET_ALL_HASHES);
    const nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits> meshes =
        model->GetResMeshes();
    for (const nw::ut::Offset* it = meshes.m_pBegin; it != meshes.m_pEnd; ++it) {
        renderer->RenderMesh(nw::ut::internal::ResArrayClassTraits::Get<nw::gfx::res::ResMesh>(*it), model);
    }
}

// 0x00590798 (name is ours)
void fgobj::ObjectBase::PutAlphaRenderState()
{
    nn::gr::CTR::Combiner::Stage stage(5);
    nn::gr::CTR::RenderState renderState;
    const u8 alphaRef = m_Alpha;
    if (m_Flags & FLAG_ALPHA_FROM_TEXTURE) {
        stage.m_Rgb.m_Combine = 8;
        stage.m_Rgb.m_Source[0] = 3;
        stage.m_Rgb.m_Source[1] = 13;
        stage.m_Rgb.m_Source[2] = 15;
    }
    stage.m_Alpha.m_Combine = 1;
    stage.m_Alpha.m_Source[0] = 15;
    stage.m_Alpha.m_Source[1] = 14;
    renderState.m_AlphaTest.m_IsEnable = true;
    renderState.m_AlphaTest.m_RefValue = 16;
    renderState.m_AlphaTest.m_Func = 6;
    renderState.m_Blend.m_SrcAlpha = 1;
    renderState.m_Blend.m_IsEnable = true;
    renderState.m_Blend.m_DstAlpha = 0;
    stage.m_ColorA = alphaRef;
    u32* command;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    u32* start = command;
    if (alphaRef < 128) {
        renderState.m_DepthTest.m_IsEnableWrite = false;
        command = renderState.m_DepthTest.MakeCommand(command, false);
    }
    command = renderState.m_AlphaTest.MakeCommand(command, false);
    command = renderState.m_Blend.MakeCommand(command, true);
    command = stage.MakeCommand(command);
    nngxMoveCommandbufferPointer((reinterpret_cast<uptr>(command) - reinterpret_cast<uptr>(start)) & ~3u);
}
} // namespace fgobj
