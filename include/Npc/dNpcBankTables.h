#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "Bank/dBankTable.h"
#include "Human/dHumanResTextureAnimBank.h"
#include "Npc/dNpcTopsBank.h"
#include "Other/dToolBank.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
class Heap;
} // namespace sead

// (the copies ARMCC keeps of BankTable::FinalizeBanks; the other instances are inlined)
template <>
DECOMP_NOINLINE bool BankTable<HumanResTextureAnimBank, 10u>::FinalizeBanks(); // 0x008253A4
template <>
DECOMP_NOINLINE bool BankTable<NpcTopsBank, 10u>::FinalizeBanks(); // 0x00823838
template <>
DECOMP_NOINLINE bool BankTable<ToolBank, 10u>::FinalizeBanks(); // 0x008264A8

// The tables of the banks of the NPC models (no RTTI; names are ours, after BsPlayerMgr::BankTables). BsNpcMgr has one
// set for the villagers and one for the special characters; the two differ only in the bank of the body model.
struct NpcBankTablesBase
{
    NpcBankTablesBase(); // 0x001CD0D0
    // (name is ours) the heaps of all banks are destroyed (the tools only when there are banks)
    bool Finalize(); // 0x001CCFB8

    BankTable<HumanResTextureAnimBank, 10u> m_EyeTextureAnim;                             // 0x0000
    BankTable<HumanResTextureAnimBank, 10u> m_MouthTextureAnim;                           // 0x00D8
    BankTable<NpcTopsBank, 10u> m_Tops;                                                   // 0x01B0
    BankTable<ToolBank, 10u> m_Tool;                                                      // 0x1980
    BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u> m_Extra; // 0x7020 (purpose unknown)
    u32 m_Num;                                                                            // 0x70F8 the banks per table
    sead::FixedSafeString<32> m_String0;                                                  // 0x70FC (purpose unknown)
    sead::FixedSafeString<32> m_String1;                                                  // 0x7128
    sead::FixedSafeString<32> m_String2;                                                  // 0x7154
    sead::FixedSafeString<32> m_String3;                                                  // 0x7180
    sead::FixedSafeString<32> m_String4;                                                  // 0x71AC
    sead::FixedSafeString<32> m_String5;                                                  // 0x71D8
};
ASSERT_SIZE(NpcBankTablesBase, 0x7204);

// TModelBank: the bank of the body model
template <typename TModelBank>
struct NpcBankTables : public NpcBankTablesBase
{
    NpcBankTables();
    ~NpcBankTables();
    // (inline; BsNpcMgr::FinalizeBanks; name is ours)
    bool Finalize()
    {
        bool isAll = m_Model.FinalizeBanks();
        return isAll & NpcBankTablesBase::Finalize();
    }

    BankTable<TModelBank, 10u> m_Model; // 0x7204
};

typedef Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > NpcModelBank;
typedef Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > NpcSpModelBank;

// the tables of the villagers: num banks per table with their heaps in heap (game code of the NPC models, not decompiled
// yet)
struct NpcNmlBankTables : public NpcBankTables<NpcModelBank>
{
    NpcNmlBankTables(); // 0x00116F44 (Thumb)
    bool Create(sead::Heap* heap, u32 num); // 0x0060F254
};
ASSERT_SIZE(NpcNmlBankTables, 0x72DC);

// the tables of the special characters
struct NpcSpBankTables : public NpcBankTables<NpcSpModelBank>
{
    NpcSpBankTables(); // 0x00118A74 (Thumb)
    bool Create(sead::Heap* heap, u32 num); // 0x006EF178
};
ASSERT_SIZE(NpcSpBankTables, 0x72DC);
