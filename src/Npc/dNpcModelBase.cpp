#include "Npc/dNpcModelBase.h"

#include "Bank/bank_Extern.h"
#include "Bs/dBsNpcMgr.h"
#include "Human/human_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "g3d/dSortMeshGroup.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "sead/seadBufferedSafeStringBase.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadFormatFixedSafeString.h"
#include "sead/seadMathCalcCommon.h"
#include "script/dScriptExtern.h"
#include "ssys/ma/dAllocator.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"
#include <cstring>

// the render priorities of the materials of the villagers (filled by game code)
// 0x00A84490
NpcModelBase::MeshPriorityTable g_NpcMeshPriorities;

namespace {
// the user data of a resource (the fields the game reads; names are ours)
struct ResUserData
{
    u32 _0[2];      // 0x00
    s32 type;       // 0x08 (0: floats, 1: integers)
    u32 _C;         // 0x0C
    union {
        f32 floats[1];
        s32 ints[1];
    };              // 0x10
};

// the user data of name of a scene object (NULL: none)
inline const ResUserData* FindUserData(const nw::gfx::res::ResSceneObject& object, const char* name)
{
    const s32 index = object.GetUserDataIndex(name);
    if (index < 0 || object.GetUserDataCount() <= index) {
        return NULL;
    }
    return static_cast<const ResUserData*>(object.GetUserData(index));
}

// the name of the material of a mesh
inline const char* GetMaterialName(const nw::gfx::res::ResModelData* model, const nw::gfx::res::ResMeshData* mesh)
{
    const nw::gfx::res::ResMaterialData* material =
        static_cast<const nw::gfx::res::ResMaterialData*>(model->materials.GetData(mesh->materialIndex));
    return material->name.To<char>();
}

// the render priorities of the meshes by the names of their materials (true: a mesh of priority 2)
inline bool SetMeshPriorities(nw::gfx::Model* model)
{
    const nw::gfx::res::ResModelData* res = static_cast<const nw::gfx::res::ResModelData*>(model->GetResSceneObject().ptr());
    const nw::ut::Offset* it = model->GetResMeshes().m_pBegin;
    const nw::ut::Offset* end = model->GetResMeshes().m_pEnd;
    bool hasFace = false;
    for (; it != end; it++) {
        const char* name = GetMaterialName(res, static_cast<const nw::gfx::res::ResMeshData*>(it->ToPtr()));
        u8 priority;
        const NpcModelBase::MeshPriority* entry = g_NpcMeshPriorities.entries;
        for (; entry != g_NpcMeshPriorities.entries + g_NpcMeshPriorities.num; entry++) {
            if (std::strncmp(entry->name, name, entry->length) == 0) {
                break;
            }
        }
        if (entry != g_NpcMeshPriorities.entries + g_NpcMeshPriorities.num) {
            priority = entry->priority;
        } else {
            priority = g_NpcMeshPriorities.defaultPriority;
        }
        if (priority == 2) {
            hasFace = true;
        }
        static_cast<nw::gfx::res::ResMeshData*>(const_cast<void*>(it->ToPtr()))->renderPriority = priority;
    }
    return hasFace;
}
} // namespace

// 0x008821B0
extern const f32 NpcModelBase::s_DefaultChairOffset[2] = {0.0f, 0.0f};

// 0x00204228
void NpcModelBase::SetMtx(const nn::math::MTX34* mtx)
{
    nn::math::MTX34 world(*mtx);
    nn::math::MTX34 scale;
    scale.m[0][0] = 1.0f;
    scale.m[0][1] = 0.0f;
    scale.m[0][2] = 0.0f;
    scale.m[0][3] = 0.0f;
    scale.m[1][0] = 0.0f;
    scale.m[1][1] = 1.0f;
    scale.m[1][2] = 0.0f;
    scale.m[1][3] = m_HeightOffset * m_Scale;
    scale.m[2][0] = 0.0f;
    scale.m[2][1] = 0.0f;
    scale.m[2][2] = 1.0f;
    scale.m[2][3] = m_Unknown34C * m_Scale;
    sead::Matrix34CalcCtr<f32>::multiply(world, world, scale);
    m_RootCtrl.ApplyWorldCurve(this, &world);
    m_Model.SetMatrix(world);
}

// the common part of the setup functions
inline bool NpcModelBase::SetupModelImpl(g3d::ResourceLoader* loader, u16 modelIndex, u32 bufferOption,
                                         ssys::ma::Allocator* allocator)
{
    void* res = loader->m_GraphicsFile.GetModels(modelIndex);
    if (!m_Model.Create(nw::gfx::res::ResSceneObject(res), allocator, allocator, bufferOption, true, 2)) {
        return false;
    }
    InitializeBones();
    BuildAnim(HumanAnimName::NML_WAIT_1, allocator, Func6E6258());
    HumanResTextureAnimBank* bankB = GetFaceBankB(&m_FaceHandleB);
    HumanResTextureAnimBank* bankA = GetFaceBankA(&m_FaceHandleA);
    m_FaceCtrl.Initialize(&m_Model, bankA, bankB, loader, loader, false);
    if (SetMeshPriorities(m_Model.GetModel())) {
        BindFaceCtrl();
    } else {
        BindFaceCtrlAnim();
    }
    m_RootCtrl.Bind(this);
    PlayAnim(HumanAnimName::NML_WAIT_1, PLAY_MODE_OF_ANIM, 0, 0.0f, 1.0f, 0.0f, NULL, 0);
    const s32 foot = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_L_FOOT_1]);
    nw::gfx::Skeleton* skeleton = m_Model.GetSkeletalModel()->GetSkeleton();
    m_FootOffsetX = skeleton->GetOriginalBones()->GetBoneTransform(foot).translate.x - s_NpcFootOffset;
    m_pBaseTransform = &m_Model.GetSkeletalModel()->GetSkeleton()->GetLocalTransforms()->begin()[m_BoneIndices[BONE_BASE]];
    const nw::gfx::res::ResSceneObject object = m_Model.GetModel()->GetResSceneObject();
    const ResUserData* chair = FindUserData(object, "chair");
    if (chair != NULL && chair->type == 0) {
        m_pChairOffset = chair->floats;
    }
    return true;
}

// 0x002042C0
bool NpcModelBase::SetupModel()
{
    m_Anim.Finalize();
    m_FaceCtrl.Finalize();
    if (m_Model.GetNode() != NULL) {
        m_Model.Finalize();
    }
    m_ModelIndex = 0;
    ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
    return SetupModelImpl(&m_ResourceLoader, m_ModelIndex, 0, allocator);
}

// 0x00204638
bool NpcModelBase::SetTexture(const char* textureName, const char* materialName, const char* slotName)
{
    const nw::gfx::res::ResModelData* model =
        static_cast<const nw::gfx::res::ResModelData*>(m_ResourceLoader.m_GraphicsFile.GetModels(m_ModelIndex));
    if (model == NULL) {
        return true;
    }
    void* texture = NULL;
    if (textureName != NULL) {
        texture = m_ResourceLoader.m_GraphicsFile.ptr()->textures.GetData(textureName);
    }
    void* material = NULL;
    if (materialName != NULL) {
        material = model->materials.GetData(materialName);
    }
    if (material == NULL || texture == NULL) {
        return false;
    }
    g3d::SetTexture(nw::gfx::res::ResMaterial(material), slotName, nw::gfx::res::ResTexture(texture));
    return true;
}

// 0x00204758
void NpcModelBase::BuildModelPath(sead::BufferedSafeStringBase<char>* path, const stage::NpcInfo* info)
{
    if (info->kind == stage::NpcInfo::KIND_ID) {
        const char* name = m_pModelName;
        if (name == NULL) {
            name = GetVillagerModelName(reinterpret_cast<const u16*>(&info->value));
        }
        path->format("Npc/Normal/Model/%s.bcres", name);
        return;
    }
    if (info->kind != stage::NpcInfo::KIND_MODEL) {
        path->format("Npc/Normal/Model/cat00.bcres");
        return;
    }
    if (m_pModelName != NULL) {
        path->format("Npc/Special/Model/%s.bcres", m_pModelName);
        return;
    }
    char buffer[6];
    sead::BufferedSafeStringBase<char> name(buffer, sizeof(buffer));
    name.copy(sead::SafeStringBase<char>(info->model), -1);
    if (info->kind == stage::NpcInfo::KIND_MODEL && info->flag == 1) {
        name.append(sead::SafeStringBase<char>("_h"), -1);
    } else {
        s32 variant = MODEL_VARIANT_NONE;
        if (stage::GetCurrent() == stage::NAME_MINI_GAME_0) {
            variant = MODEL_VARIANT_MINI_GAME_0;
        } else if (stage::GetCurrent() == stage::NAME_MINI_GAME_1) {
            variant = MODEL_VARIANT_MINI_GAME_1;
        } else if (stage::GetCurrent() == stage::NAME_AMIIBO_CAMERA) {
            variant = MODEL_VARIANT_AMIIBO_CAMERA;
        }
        for (s32 i = 0; i < 5; i++) {
            if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                buffer[i] += 'a' - 'A';
            }
        }
        if (variant == MODEL_VARIANT_NONE) {
            if (name.isEqual(sead::SafeStringBase<char>("sza"))) {
                nn::fnd::DateTimeParameters date = time::GetSavedTime();
                const u32 part = time::GetSeasonPart(date);
                if (part - 3 <= 11) {
                    name.copy(sead::SafeStringBase<char>("sza"), -1);
                } else {
                    name.copy(sead::SafeStringBase<char>("szb"), -1);
                }
            }
            if (name.isEqual(sead::SafeStringBase<char>("szo")) &&
                !stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
                const u8 weather = Func62FB14();
                if (weather == 3 || weather == 4 || weather == 5 || weather == 6) {
                    name.copy(sead::SafeStringBase<char>("szr"), -1);
                }
            }
            if (name.isEqual(sead::SafeStringBase<char>("spn")) &&
                GetGardenPlus()->data.townData.townFlags.unk_0xF_4) {
                name.copy(sead::SafeStringBase<char>("spr"), -1);
            }
        }
        ApplyModelVariant(&name, &m_Item37C, variant);
    }
    path->format("Npc/Special/Model/%s.bcres", name.getCString());
}

// 0x00204B24
void NpcModelBase::SetupModelNoGuest()
{
    SetupModel();
    m_Flags &= ~FLAG_GUEST_MODEL;
}

// 0x00204B40
void NpcModelBase::SetupModelNoMka()
{
    SetupModel();
    m_Flags &= ~FLAG_MKA_MODEL;
}

// 0x00204B5C
void NpcModelBase::SetupModelNoPkn()
{
    SetupModel();
    m_Flags &= ~FLAG_PKN_MODEL;
}

// 0x00204B78
bool NpcModelBase::SetupGuestModel()
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr == NULL || !mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_GUEST)) {
        return false;
    }
    m_Anim.Finalize();
    m_FaceCtrl.Finalize();
    if (m_Model.GetNode() != NULL) {
        m_Model.Finalize();
    }
    ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
    if (!SetupModelImpl(&mgr->m_GstLoader, mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_GUEST_SEO), 0x1FFF, allocator)) {
        return false;
    }
    m_Flags |= FLAG_GUEST_MODEL;
    return true;
}

// 0x00204F28
bool NpcModelBase::SetupMkaModel()
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr == NULL || !mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_MKA)) {
        return false;
    }
    m_Anim.Finalize();
    m_FaceCtrl.Finalize();
    if (m_Model.GetNode() != NULL) {
        m_Model.Finalize();
    }
    ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
    if (!SetupModelImpl(&mgr->m_MkaLoader, 0, 0x1FFF, allocator)) {
        return false;
    }
    m_Flags |= FLAG_MKA_MODEL;
    return true;
}

// 0x002052D8
bool NpcModelBase::SetupPknModel(u32, u8 bodyTexture)
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr == NULL || !mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_PKN)) {
        return false;
    }
    if (m_BodyTexture == 255) {
        m_BodyTexture = bodyTexture;
    }
    m_Anim.Finalize();
    m_FaceCtrl.Finalize();
    if (m_Model.GetNode() != NULL) {
        m_Model.Finalize();
    }
    ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
    u16 modelIndex = 0;
    if (m_Unknown368 == 0) {
        modelIndex = 2;
    } else if (m_Unknown368 != 1 && m_Unknown368 == 2) {
        modelIndex = 1;
    }
    void* res = mgr->m_PknLoader.m_GraphicsFile.GetModels(modelIndex);
    SetBodyTexture(static_cast<const nw::gfx::res::ResModelData*>(res), m_BodyTexture);
    if (!SetupModelImpl(&mgr->m_PknLoader, modelIndex, 0x1FFF, allocator)) {
        return false;
    }
    m_Flags |= FLAG_PKN_MODEL;
    return true;
}

// 0x002056CC
bool NpcModelBase::SetBodyTexture(const nw::gfx::res::ResModelData* model, u8 variant)
{
    if (model == NULL || variant > 4) {
        return true;
    }
    m_BodyTexture = variant;
    sead::FormatFixedSafeString<6> name("b.%d", variant);
    g3d::ResourceLoader* loader = &m_ResourceLoader;
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr != NULL && mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_PKN)) {
        loader = &mgr->m_PknLoader;
    }
    void* texture = NULL;
    if (name.getCString() != NULL) {
        texture = loader->m_GraphicsFile.ptr()->textures.GetData(name.getCString());
    }
    void* material = model->materials.GetData("b");
    if (material != NULL && texture != NULL) {
        g3d::SetTexture(nw::gfx::res::ResMaterial(material), "b", nw::gfx::res::ResTexture(texture));
    }
    return true;
}

// 0x002058A8
bool NpcModelBase::StartLoad(const stage::NpcInfo* info, u16 modelIndex)
{
    m_pInfo = info;
    return Load(info, modelIndex, false);
}

// 0x002058B4
bool NpcModelBase::Load(const stage::NpcInfo* info, u16 modelIndex, bool skipCommon)
{
    switch (m_LoadState) {
    case LOAD_STATE_BANKS:
        m_ModelIndex = modelIndex;
        if (!AcquireBanks(&m_ModelHandle, &m_FaceHandleA, &m_FaceHandleB)) {
            return false;
        }
        m_LoadState = LOAD_STATE_FILE;
        // fall through
    case LOAD_STATE_FILE: {
        ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
        g3d::ResourceLoader* loader = &m_ResourceLoader;
        sead::Heap* heap = allocator->m_pHeap;
        sead::FixedSafeString<64> path;
        const stage::NpcInfo copy = *info;
        BuildModelPath(&path, &copy);
        if (loader->LoadAsync(path, heap, 128) != true) {
            return false;
        }
        m_LoadState = LOAD_STATE_SETUP;
    }
        // fall through
    case LOAD_STATE_SETUP: {
        BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
        if (!mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_CMN)) {
            if (!skipCommon) {
                return false;
            }
            m_LoadState = LOAD_STATE_DONE;
            return true;
        }
        ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
        m_ResourceLoader.Read(0x10000, 0x30000, 0x10000);
        m_ResourceLoader.InitializeCgfx(allocator, NULL, true);
        m_ResourceLoader.InitializeCgfx(allocator, &mgr->m_CmnLoader, true);
        const u32 num = m_ResourceLoader.m_GraphicsFile.ptr()->models.count;
        for (u32 i = 0; i < num; i++) {
            void* model = m_ResourceLoader.m_GraphicsFile.GetModels(i);
            g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model), Func1E891C());
        }
        m_LoadState = LOAD_STATE_MODEL;
    }
        // fall through
    case LOAD_STATE_MODEL: {
        ssys::ma::Allocator* allocator = GetModelAllocator(&m_ModelHandle);
        void* res = m_ResourceLoader.m_GraphicsFile.GetModels(m_ModelIndex);
        if (!m_Model.Create(nw::gfx::res::ResSceneObject(res), allocator, allocator, 0, true, 2)) {
            return false;
        }
        InitializeBones();
        BuildAnim(HumanAnimName::NML_WAIT_1, allocator, Func6E6258());
        HumanResTextureAnimBank* bankB = GetFaceBankB(&m_FaceHandleB);
        HumanResTextureAnimBank* bankA = GetFaceBankA(&m_FaceHandleA);
        m_FaceCtrl.Initialize(&m_Model, bankA, bankB, &m_ResourceLoader, &m_ResourceLoader, false);
        if (SetMeshPriorities(m_Model.GetModel())) {
            BindFaceCtrl();
        } else {
            BindFaceCtrlAnim();
        }
        m_RootCtrl.Bind(this);
        PlayAnim(HumanAnimName::NML_WAIT_1, PLAY_MODE_OF_ANIM, 0, 0.0f, 1.0f, 0.0f, NULL, 0);
        const s32 foot = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_L_FOOT_1]);
        const s32 chest = m_Model.GetBoneIndex(s_BoneNames[BONE_NAME_CHEST]);
        nw::gfx::Skeleton* skeleton = m_Model.GetSkeletalModel()->GetSkeleton();
        m_FootOffsetX = skeleton->GetOriginalBones()->GetBoneTransform(foot).translate.x - s_NpcFootOffset;
        nw::gfx::Skeleton::TransformArray* transforms = m_Model.GetSkeletalModel()->GetSkeleton()->GetLocalTransforms();
        m_pBaseTransform = &transforms->begin()[m_BoneIndices[BONE_BASE]];
        m_pChestTransform = &transforms->begin()[chest];
        const nw::gfx::res::ResSceneObject object = m_Model.GetModel()->GetResSceneObject();
        const ResUserData* chair = FindUserData(object, "chair");
        if (chair != NULL && chair->type == 0) {
            m_pChairOffset = chair->floats;
        }
        const ResUserData* move = FindUserData(object, "move");
        if (move == NULL) {
            m_Unknown368 = 1;
        } else if (move->type == 1) {
            m_Unknown368 = move->ints[0] & 0xFF;
        }
        m_Flags |= FLAG_LOADED;
        m_LoadState = LOAD_STATE_DONE;
        return true;
    }
    default:
        return true;
    }
}

// 0x00205E9C
u32 NpcModelBase::vf_0x4C()
{
    return 0;
}

// 0x00205EA4
void NpcModelBase::SetMeshVisible(const char* materialName, bool visible)
{
    nw::gfx::Model* model = m_Model.GetModel();
    const nw::gfx::res::ResModelData* res = static_cast<const nw::gfx::res::ResModelData*>(model->GetResSceneObject().ptr());
    const nw::ut::Offset* it = model->GetResMeshes().m_pBegin;
    const nw::ut::Offset* end = model->GetResMeshes().m_pEnd;
    for (; it != end; it++) {
        nw::gfx::res::ResMeshData* mesh = static_cast<nw::gfx::res::ResMeshData*>(const_cast<void*>(it->ToPtr()));
        if (mesh->materialIndex < 0) {
            continue;
        }
        const nw::gfx::res::ResMaterialData* material =
            static_cast<const nw::gfx::res::ResMaterialData*>(res->materials.GetData(mesh->materialIndex));
        if (material == NULL) {
            continue;
        }
        if (sead::SafeStringBase<char>(materialName).isEqual(sead::SafeStringBase<char>(material->name.To<char>()))) {
            mesh->isVisible = visible;
        }
    }
}

// 0x0020601C
bool NpcModelBase::ApplyModelVariant(sead::BufferedSafeStringBase<char>* name, SvFgName* out, s32 variant)
{
    if (name->isEqual(sead::SafeStringBase<char>("upb"))) {
        name->copy(sead::SafeStringBase<char>("upa"), -1);
        *out = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_DR_SHRUNKS_JACKET));
        return true;
    }
    if (variant == MODEL_VARIANT_NONE) {
        return false;
    }
    // (the flags: 1..8 "_a" for MiniGame1, MiniGame0, AmiiboCamera, the variant 0; 0x10 "_e" for MiniGame0; 0x20 "mo" ->
    // "mp"; the image 0x008821F8)
    struct Variant
    {
        const char* name;
        u8 flags;
    };
    const Variant variants[18] = {
        {"sza", 0x10}, {"szb", 0x10}, {"szd", 0x10}, {"sze", 0x10}, {"otg", 0x10}, {"ots", 0x10},
        {"tap", 0x10}, {"pck", 0x10}, {"end", 0x0F}, {"moo", 0x24}, {"mof", 0x20}, {"moc", 0x24},
        {"mod", 0x20}, {"ott", 0x04}, {"ttl", 0x04}, {"wrl", 0x04}, {"chm", 0x04}, {"snt", 0x04},
    };
    bool changed = false;
    for (s32 i = 0; i < 18; i++) {
        const Variant& entry = variants[i];
        if (!name->isEqual(sead::SafeStringBase<char>(entry.name))) {
            continue;
        }
        if (entry.flags & 0x20) {
            bool truncated = false;
            sead::BufferedSafeStringBase<char>::replaceStringImpl(
                name->getMutableStringTop(), NULL, name->getBufferSize(), name->getCString(), name->getLength(),
                sead::SafeStringBase<char>("mo"), sead::SafeStringBase<char>("mp"), &truncated);
            changed = true;
        }
        switch (variant) {
        case 0:
            if (entry.flags & 1) {
                name->append(sead::SafeStringBase<char>("_a"), -1);
                changed = true;
            }
            break;
        case MODEL_VARIANT_MINI_GAME_1:
            if (entry.flags & 2) {
                name->append(sead::SafeStringBase<char>("_a"), -1);
                changed = true;
            }
            break;
        case MODEL_VARIANT_MINI_GAME_0:
            if (entry.flags & 4) {
                name->append(sead::SafeStringBase<char>("_a"), -1);
                changed = true;
            } else if (entry.flags & 0x10) {
                name->append(sead::SafeStringBase<char>("_e"), -1);
                changed = true;
            }
            break;
        case MODEL_VARIANT_AMIIBO_CAMERA:
            if (entry.flags & 8) {
                name->append(sead::SafeStringBase<char>("_a"), -1);
                changed = true;
            }
            break;
        }
        break;
    }
    return changed;
}

// 0x0020630C
HumanFaceAnimControl* NpcModelBase::GetFaceCtrl()
{
    return &m_FaceCtrl;
}

// 0x00206314
void NpcModelBase::SetEmission(const nw::ut::FloatColor& color)
{
    g3d::SetEmission(static_cast<const nw::gfx::res::ResModelData*>(m_ResourceLoader.m_GraphicsFile.GetModels(m_ModelIndex)),
                     color);
}

// 0x0020633C
void NpcModelBase::SetAmbientAndDiffuse(f32 value)
{
    g3d::SetAmbientAndDiffuse(
        static_cast<const nw::gfx::res::ResModelData*>(m_ResourceLoader.m_GraphicsFile.GetModels(m_ModelIndex)), value);
}

// 0x0020636C
void NpcModelBase::SetEmission(f32 value)
{
    g3d::SetEmission(static_cast<const nw::gfx::res::ResModelData*>(m_ResourceLoader.m_GraphicsFile.GetModels(m_ModelIndex)),
                     value);
}

// 0x0020639C
bool NpcModelBase::PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                            u8 morphType)
{
    const u32 isNew = id < HumanAnimName::COUNT ? Func6E6268(id)->flags >> 7 : 0;
    const u16 cur = m_AnimIds[ANIM_SLOT_BODY];
    const u32 isCur = cur < HumanAnimName::COUNT ? Func6E6268(cur)->flags >> 7 : 0;
    if (isCur != isNew) {
        m_BlendFrames = duration;
    }
    bool dummy = false;
    if (changed == NULL) {
        changed = &dummy;
    }
    bool result;
    if (m_Flags & FLAG_UPPER_BODY) {
        result = PlayAnimImpl(id, ANIM_SLOT_ARMS, playMode, frame, speed, duration, flags, changed, morphType);
        m_Anim.AssignBones(ANIM_SLOT_ARMS, s_BoneNames[BONE_NAME_CHEST]);
    } else {
        result = HumanModel::PlayAnim(id, playMode, flags, frame, speed, duration, changed, morphType);
    }
    if (!result) {
        return false;
    }
    m_RootCtrl.Reset();
    if ((*changed == true || m_Unknown36D) && m_pAnimCallback != NULL) {
        m_pAnimCallback->invoke(this);
    }
    return true;
}

// 0x0020650C
void NpcModelBase::SetItem37C(SvFgName item)
{
    m_Item37C = item;
}

// 0x00206534
void NpcModelBase::vf_0x0C()
{
    m_Model.EvaluateAnims();
    const s32 base = m_BoneIndices[BONE_BASE];
    m_FaceCtrl.EndFallbackB(false, true);
    const u16 cur = m_AnimIds[ANIM_SLOT_BODY];
    f32 offset;
    if (cur < HumanAnimName::COUNT && (Func6E6268(cur)->flags >> 7) != 0) {
        offset = 0.0f;
    } else if (IsChairWaitAnim(cur)) {
        offset = m_FootOffsetX + m_pChairOffset[0];
        m_Unknown34C = m_pChairOffset[1] + s_NpcChairHeight;
    } else {
        const nn::math::Transform3& transform =
            m_Model.GetSkeletalModel()->GetSkeleton()->GetOriginalBones()->GetBoneTransform(base);
        m_Unknown34C = 0.0f;
        offset = (transform.translate.y / s_NpcBaseHeightUnit - 1.0f) * m_pBaseTransform->m_TransformMatrix.m[1][3];
    }
    const f32 frames = m_BlendFrames;
    const f32 left = frames - 1.0f;
    m_BlendFrames = left;
    if (left <= 0.0f) {
        m_HeightOffset = offset;
    } else {
        const f32 ratio = left / frames;
        m_HeightOffset = m_HeightOffset * ratio + (1.0f - ratio) * offset;
    }
    m_RootCtrl.Save(this);
}

// 0x00206668
bool NpcModelBase::Unload()
{
    if (m_LoadState != LOAD_STATE_DONE && !Load(m_pInfo, m_ModelIndex, true)) {
        return false;
    }
    if (m_Model.GetNode() != NULL) {
        m_Anim.Finalize();
        m_FaceCtrl.Finalize();
        m_Model.Finalize();
    }
    m_ResourceLoader.Unload();
    ReleaseBanks(&m_ModelHandle, &m_FaceHandleA, &m_FaceHandleB);
    return true;
}

// 0x002066F8
void NpcModelBase::SetScale(f32 scale)
{
    m_Scale = scale;
    m_Model.SetScale(nn::math::VEC3(scale, scale, scale));
}

// 0x0020674C
NpcModelBase::NpcModelBase() : HumanModel(true)
{
    m_LoadState = LOAD_STATE_BANKS;
    m_Flags = 0;
    m_ItemFlags = 0;
    m_RootCtrl.m_RootTranslation = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    m_RootCtrl.m_BaseTranslation = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    m_RootCtrl.m_pPosition = NULL;
    m_RootCtrl.m_pRootTransform = NULL;
    m_RootCtrl.m_pBaseTransform = NULL;
    m_RootCtrl.m_pRootBone = NULL;
    m_RootCtrl.m_pBaseBone = NULL;
    m_Scale = 1.0f;
    m_HeightOffset = 0.0f;
    m_Unknown34C = 0.0f;
    m_FootOffsetX = 0.0f;
    m_BlendFrames = 0.0f;
    m_pBaseTransform = NULL;
    m_pChairOffset = s_DefaultChairOffset;
    m_pChestTransform = NULL;
    m_BodyTexture = 255;
    m_Unknown36D = false;
    m_pModelName = NULL;
    m_pUpdateCallback = NULL;
    m_pAnimCallback = NULL;
    m_Item37C = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
    m_Item380 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
}

// 0x00206954
// 0x002068B8 (deleting dtor)
NpcModelBase::~NpcModelBase()
{
    // (only the members are destroyed)
}

// 0x004EE434
void NpcModelBase::vf_0x14()
{
    vf_0x4C();
    g3d::GetSortMeshGroups()->Add(m_Model.GetModel());
}

// 0x004F14A0
void NpcModelBase::vf_0x08()
{
    if (m_pUpdateCallback != NULL) {
        m_pUpdateCallback->invoke(this);
    }
    m_FaceCtrl.Update(false);
    m_Model.UpdateAnimFrames();
}

// 0x00304260
bool NpcModelBase::vf_0x2C()
{
    return Unload();
}

// 0x001AC808
void NpcModelBase::vf_0x10()
{
    m_HeadCtrl.Update();
    m_Model.Update();
    m_HeadCtrl.Restore();
}
