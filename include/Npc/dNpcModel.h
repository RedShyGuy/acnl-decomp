#pragma once

#include "decomp.h"
#include "Npc/dNpcAccessoryModel.h"
#include "Npc/dNpcModelBase.h"

struct NpcBankTablesBase;
class NpcTopsBank;
class ToolBank;

// The model of a villager or a special character with its items (class name from the RTTI; the members and the
// function names are ours): the banks of the model and the face come from the bank tables of BsNpcMgr (the villagers
// or the special characters by the kind of the info), the tops (m_Item37C on the material "cloth"), the tool in the
// right hand (m_Item380, with its own animation in slot 2) and the accessory of the season.
// RTTI 8NpcModel @ 0x008CD51C
// vtable 0x008F9CC4 (vptr 0x008F9CCC), offset_to_top 0, 20 entries
class NpcModel : public ::NpcModelBase
{
public:
    // m_ItemFlags
    enum ItemFlag
    {
        ITEM_FLAG_TOPS = 1 << 0,
        ITEM_FLAG_ACCESSORY = 1 << 1,
        ITEM_FLAG_TOOL = 1 << 2,
    };
    // the animation slot of the tool
    static const u32 ANIM_SLOT_TOOL = 2;

    NpcModel(bool hasTops, bool hasTool, bool hasAccessory); // 0x006AD25C
    // PlayAnim of NpcModelBase; a new animation also changes the tool animation
    bool PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed, u8 morphType) override; // 0x006ACEFC slot 0x00
    // the animation of the tool of m_Item380
    void vf_0x04() override; // 0x006ACD48 slot 0x04
    void vf_0x08() override; // 0x006ACD70 slot 0x08
    void vf_0x0C() override; // 0x006ACFE4 slot 0x0C
    void vf_0x10() override; // 0x006AD1D8 slot 0x10
    void vf_0x14() override; // 0x006ACDD0 slot 0x14
    virtual ~NpcModel(); // 0x006AD30C slot 0x1C
    // 0x006AD2FC slot 0x20 (deleting dtor)
    // StartLoad of NpcModelBase, then the banks of the items of m_ItemFlags
    bool StartLoad(const stage::NpcInfo* info, u16 modelIndex) override; // 0x006ACE34 slot 0x30
    // the item banks are given back, then Unload of NpcModelBase
    bool Unload() override; // 0x006AD048 slot 0x34
    bool AcquireBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB) override; // 0x006ABFD0 slot 0x38
    ssys::ma::Allocator* GetModelAllocator(BankHandle* handle) override; // 0x006ACBE4 slot 0x3C
    HumanResTextureAnimBank* GetFaceBankA(BankHandle* handle) override; // 0x006ACB8C slot 0x40
    HumanResTextureAnimBank* GetFaceBankB(BankHandle* handle) override; // 0x006ACCEC slot 0x44
    void ReleaseBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB) override; // 0x00213968 slot 0x48
    // the tops and the tool are exchanged for m_Item37C and m_Item380 (m_ItemsReady: both shown); true: one changed
    u32 vf_0x4C() override; // 0x006AC450 slot 0x4C

    // the banks of the tops and of the tool (NULL: none)
    NpcTopsBank* GetTopsBank(); // 0x006ABF68
    ToolBank* GetToolBank(); // 0x006ABEF4
    // slot 0x4C, then m_ItemsReady
    bool UpdateItems(); // 0x006AC090
    // the animation of the tool of item in slot 2 (armsOnly: all arms of the animation; force: also when the tool does
    // not move; restart: also when it plays already); false: no animation
    bool PlayToolAnim(const SvFgName* item, bool armsOnly, bool force, f32 duration, bool restart); // 0x006AC0B4
    void StopToolAnim(f32 duration); // 0x006ACB60
    // the keywords of the tops and of the tool are cleared
    void ClearItems(); // 0x006AC2F8
    // the accessory of kind (info: the info of the NPC)
    bool SetupAccessory(u16 kind, const stage::NpcInfo* info); // 0x006AD148
    void FinalizeAccessory(); // 0x006AD238

    NpcAccessoryModel m_Accessory;        // 0x384
    u8 m_ToolArmFlags;                    // 0x4C8 HumanModel::ArmFlag of the tool animation
    NpcBankTablesBase* m_pBankTables;     // 0x4CC (of BsNpcMgr)
    bool m_ItemsReady;                    // 0x4D0
    bool m_IsAccessoryVisible;            // 0x4D1
};
ASSERT_OFFSET(NpcModel, m_Accessory, 0x384);
ASSERT_OFFSET(NpcModel, m_ToolArmFlags, 0x4C8);
ASSERT_SIZE(NpcModel, 0x4D4);
