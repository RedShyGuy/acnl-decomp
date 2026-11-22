#include "Npc/dNpcModel.h"

#include "Bank/bank_Extern.h"
#include "Bs/dBsNpcMgr.h"
#include "Human/human_Extern.h"
#include "Npc/dNpcBankTables.h"
#include "Npc/dNpcTopsBank.h"
#include "Other/dToolBank.h"
#include "g3d/dSortMeshGroup.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "stage/stage_Api.h"
#include <cmath>

namespace {
// the tool moves (its bank turns it at least this much per frame)
const f32 TOOL_MOVE_MIN = 0.01f;
} // namespace

// 0x006ABEF4
ToolBank* NpcModel::GetToolBank()
{
    if (m_pBankTables->m_Tool.m_Size == 0) {
        return NULL;
    }
    return m_pBankTables->m_Tool.GetBank(&m_ToolHandle);
}

// 0x006ABF68
NpcTopsBank* NpcModel::GetTopsBank()
{
    if (m_pBankTables->m_Tops.m_Size == 0) {
        return NULL;
    }
    return m_pBankTables->m_Tops.GetBank(&m_TopsHandle);
}

// 0x006ABFD0
bool NpcModel::AcquireBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB)
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    switch (m_pInfo->kind) {
    case stage::NpcInfo::KIND_ID:
        m_pBankTables = &mgr->m_NmlBanks;
        if (!mgr->m_NmlBanks.m_Model.Alloc(model)) {
            return false;
        }
        break;
    case stage::NpcInfo::KIND_MODEL:
        m_pBankTables = &mgr->m_SpBanks;
        if (!mgr->m_SpBanks.m_Model.Alloc(model)) {
            return false;
        }
        break;
    }
    if (!m_pBankTables->m_EyeTextureAnim.Alloc(faceA)) {
        return false;
    }
    return m_pBankTables->m_MouthTextureAnim.Alloc(faceB);
}

// 0x006AC090
bool NpcModel::UpdateItems()
{
    vf_0x4C();
    return m_ItemsReady;
}

// 0x006AC0B4
bool NpcModel::PlayToolAnim(const SvFgName* item, bool armsOnly, bool force, f32 duration, bool restart)
{
    u16 id = Func6E6050(item);
    const u16 armAnim = m_AnimIds[ANIM_SLOT_ARMS];
    u32 idx;
    if (armAnim != HumanAnimName::COUNT && (Func6E6268(armAnim)->armFlags & 3) == 0) {
        idx = ANIM_SLOT_ARMS;
    } else {
        idx = ANIM_SLOT_BODY;
    }
    const HumanAnimInfo* info = Func6E6268(m_AnimIds[idx]);
    if (id == 0) {
        if (info->flags & 0x40) {
            id = HumanAnimName::TRANS_F_HAND_1;
        } else {
            StopToolAnim(duration);
            return false;
        }
    } else if (!force) {
        ToolBank* tool = GetToolBank();
        const f32 move = tool != NULL ? static_cast<ExchangeModelBankBase*>(tool)->unknown : 0.0f;
        if (std::fabs(move) < TOOL_MOVE_MIN) {
            StopToolAnim(duration);
            return false;
        }
    }
    const u8 freeArms = info->freeArms;
    u8 arms = Func6E6268(id)->armFlags & 15;
    if (!armsOnly) {
        arms &= freeArms;
    }
    if (!restart && m_AnimIds[ANIM_SLOT_TOOL] == id && m_ToolArmFlags == arms) {
        return true;
    }
    if (m_ToolArmFlags != 0) {
        StopAnim(ANIM_SLOT_TOOL, duration);
    }
    m_ToolArmFlags = 0;
    if (arms == 0) {
        return true;
    }
    m_PrevAnimIds[ANIM_SLOT_TOOL] = m_AnimIds[ANIM_SLOT_TOOL];
    m_AnimIds[ANIM_SLOT_TOOL] = id;
    m_Anim.ChangeAnim(ANIM_SLOT_TOOL, Func770970(&g_HumanAnimTable, id)->anim0, 0, duration);
    if (arms & ARM_FLAG_RIGHT) {
        m_Anim.AssignBones(ANIM_SLOT_TOOL, s_BoneNames[BONE_NAME_R_ARM_1]);
    }
    if (arms & ARM_FLAG_LEFT) {
        m_Anim.AssignBones(ANIM_SLOT_TOOL, s_BoneNames[BONE_NAME_L_ARM_1]);
    }
    m_ToolArmFlags = arms;
    return true;
}

// 0x006AC2F8
void NpcModel::ClearItems()
{
    NpcTopsBank* tops = GetTopsBank();
    if (tops != NULL) {
        for (u32 i = 0; i < 2; i++) {
            tops->m_Keywords[i] = NpcTopsBank::Keyword();
        }
    }
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        for (u32 i = 0; i < 2; i++) {
            tool->m_Keywords[i] = ToolBank::Keyword();
        }
    }
    m_ItemsReady = false;
}

// 0x006AC450
u32 NpcModel::vf_0x4C()
{
    bool isChanged = false;
    bool isReady = true;
    NpcTopsBank* tops = GetTopsBank();
    if (tops != NULL) {
        const NpcTopsBank::Keyword keyword(m_Item37C);
        if (m_Item37C.IsEmpty()) {
            SetTexture("cloth", "cloth", "cloth");
        } else {
            const s32 result = tops->Exchange(keyword);
            if (result == 0) {
                isReady = false;
            } else if (result == 2) {
                tops->SetClothTexture(
                    static_cast<const nw::gfx::res::ResModelData*>(m_ResourceLoader.m_GraphicsFile.GetModels(0)));
                isChanged = true;
            }
        }
    }
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        const ToolBank::Keyword keyword(m_Item380);
        const s32 result = tool->Exchange(keyword);
        if (result == 0) {
            isReady = false;
        } else if (result == 2) {
            Func3286CC(tool, &m_Model, m_BoneIndices[BONE_R_HAND]);
            isChanged = true;
        }
    }
    m_ItemsReady = isReady;
    return isChanged;
}

// 0x006ACB60
void NpcModel::StopToolAnim(f32 duration)
{
    if (m_ToolArmFlags != 0) {
        StopAnim(ANIM_SLOT_TOOL, duration);
    }
    m_ToolArmFlags = 0;
}

// 0x006ACB8C
HumanResTextureAnimBank* NpcModel::GetFaceBankA(BankHandle* handle)
{
    return m_pBankTables->m_EyeTextureAnim.GetBank(handle);
}

// 0x006ACBE4
ssys::ma::Allocator* NpcModel::GetModelAllocator(BankHandle* handle)
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >* bank;
    switch (m_pInfo->kind) {
    case stage::NpcInfo::KIND_ID:
        bank = mgr->m_NmlBanks.m_Model.GetBank(handle);
        break;
    case stage::NpcInfo::KIND_MODEL:
        bank = reinterpret_cast<NpcModelBank*>(mgr->m_SpBanks.m_Model.GetBank(handle));
        break;
    default:
        bank = mgr->m_NmlBanks.m_Model.GetBank(handle);
        break;
    }
    return &bank->m_Allocators[0];
}

// 0x006ACCEC
HumanResTextureAnimBank* NpcModel::GetFaceBankB(BankHandle* handle)
{
    return m_pBankTables->m_MouthTextureAnim.GetBank(handle);
}

// 0x006ACD48
void NpcModel::vf_0x04()
{
    PlayToolAnim(&m_Item380, false, false, 0.0f, true);
}

// 0x006ACD70
void NpcModel::vf_0x08()
{
    NpcModelBase::vf_0x08();
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        tool->UpdateFrames();
    }
}

// 0x006ACDD0
void NpcModel::vf_0x14()
{
    NpcModelBase::vf_0x14();
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        tool->AddToScene(0);
    }
    if (m_Accessory.m_State != NpcAccessoryModel::STATE_NONE && m_IsAccessoryVisible) {
        m_Accessory.m_Model.AddToScene(0);
    }
}

// 0x006ACE34
bool NpcModel::StartLoad(const stage::NpcInfo* info, u16 modelIndex)
{
    m_pInfo = info;
    if (!Load(info, modelIndex, false)) {
        return false;
    }
    NpcBankTablesBase* tables = m_pBankTables;
    if ((m_ItemFlags & ITEM_FLAG_TOPS) && !tables->m_Tops.Alloc(&m_TopsHandle)) {
        return false;
    }
    if (tables->m_Tool.m_Size != 0 && (m_ItemFlags & ITEM_FLAG_TOOL) && !tables->m_Tool.Alloc(&m_ToolHandle)) {
        return false;
    }
    if ((m_ItemFlags & ITEM_FLAG_ACCESSORY) && !tables->m_Extra.Alloc(&m_AccessoryHandle)) {
        return false;
    }
    return true;
}

// 0x006ACEFC
bool NpcModel::PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                        u8 morphType)
{
    bool dummy = false;
    if (changed == NULL) {
        changed = &dummy;
    }
    const bool result = NpcModelBase::PlayAnim(id, playMode, flags, frame, speed, duration, changed, morphType);
    ToolBank* tool = GetToolBank();
    if (*changed == true) {
        PlayToolAnim(&m_Item380, false, false, duration, false);
        if (tool != NULL) {
            const f32 bodySpeed = GetAnimSpeed(ANIM_SLOT_BODY);
            const u8 bodyPlayMode = GetAnimPlayMode(ANIM_SLOT_BODY);
            const f32 bodyFrame = GetAnimFrame(ANIM_SLOT_BODY);
            tool->PlayToolAnim(m_AnimIds[ANIM_SLOT_BODY], bodyPlayMode, flags == 0, bodyFrame, bodySpeed, duration, 0);
        }
    }
    return result;
}

// 0x006ACFE4
void NpcModel::vf_0x0C()
{
    NpcModelBase::vf_0x0C();
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        tool->ApplyBoneRotate();
    }
}

// 0x006AD048
bool NpcModel::Unload()
{
    NpcBankTablesBase* tables = m_pBankTables;
    if (m_AccessoryHandle.m_pTable != NULL) {
        m_Accessory.m_Loader.Reset();
        m_Accessory.m_Model.Finalize();
        m_Accessory.m_State = NpcAccessoryModel::STATE_NONE;
        tables->m_Extra.Free(&m_AccessoryHandle);
    }
    if (m_ToolHandle.m_pTable != NULL) {
        tables->m_Tool.Free(&m_ToolHandle);
    }
    if (m_TopsHandle.m_pTable != NULL) {
        tables->m_Tops.Free(&m_TopsHandle);
    }
    return NpcModelBase::Unload();
}

// 0x006AD148
bool NpcModel::SetupAccessory(u16 kind, const stage::NpcInfo* info)
{
    m_ResourceLoader.m_GraphicsFile.GetModels(0);
    Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >* bank =
        m_pBankTables->m_Extra.GetBank(&m_AccessoryHandle);
    return m_Accessory.Setup(kind, &bank->m_Allocators[0], &m_Model, info);
}

// 0x006AD1D8
void NpcModel::vf_0x10()
{
    NpcModelBase::vf_0x10();
    ToolBank* tool = GetToolBank();
    if (tool != NULL) {
        Func3286CC(tool, &m_Model, m_BoneIndices[BONE_R_HAND]);
    }
    if (m_Accessory.m_State != NpcAccessoryModel::STATE_NONE) {
        m_Accessory.Update(&m_Model);
    }
}

// 0x006AD238
void NpcModel::FinalizeAccessory()
{
    m_Accessory.m_Loader.Reset();
    m_Accessory.m_Model.Finalize();
    m_Accessory.m_State = NpcAccessoryModel::STATE_NONE;
}

// 0x006AD25C
NpcModel::NpcModel(bool hasTops, bool hasTool, bool hasAccessory)
{
    m_ToolArmFlags = 0;
    m_pBankTables = NULL;
    m_ItemsReady = false;
    m_IsAccessoryVisible = true;
    if (hasTops) {
        m_ItemFlags |= ITEM_FLAG_TOPS;
    } else {
        m_ItemFlags &= ~ITEM_FLAG_TOPS;
    }
    if (hasTool) {
        m_ItemFlags |= ITEM_FLAG_TOOL;
    } else {
        m_ItemFlags &= ~ITEM_FLAG_TOOL;
    }
    if (hasAccessory) {
        m_ItemFlags |= ITEM_FLAG_ACCESSORY;
    } else {
        m_ItemFlags &= ~ITEM_FLAG_ACCESSORY;
    }
}

// 0x006AD30C
// 0x006AD2FC (deleting dtor)
NpcModel::~NpcModel()
{
    // (only the members are destroyed)
}

// 0x00213968
void NpcModel::ReleaseBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB)
{
    NpcBankTablesBase* tables = m_pBankTables;
    if (m_pInfo->kind == stage::NpcInfo::KIND_ID || m_pInfo->kind == stage::NpcInfo::KIND_MODEL) {
        static_cast<NpcNmlBankTables*>(tables)->m_Model.Free(model);
    }
    tables->m_EyeTextureAnim.Free(faceA);
    tables->m_MouthTextureAnim.Free(faceB);
}
