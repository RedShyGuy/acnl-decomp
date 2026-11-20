#include "Npc/dNpcBankTables.h"

template <>
bool BankTable<HumanResTextureAnimBank, 10u>::FinalizeBanks()
{
    return FinalizeBanksImpl();
}

template <>
bool BankTable<NpcTopsBank, 10u>::FinalizeBanks()
{
    return FinalizeBanksImpl();
}

template <>
bool BankTable<ToolBank, 10u>::FinalizeBanks()
{
    return FinalizeBanksImpl();
}

// 0x001CD0D0
NpcBankTablesBase::NpcBankTablesBase() : m_Num(0)
{
}

// 0x001CCFB8
bool NpcBankTablesBase::Finalize()
{
    bool isAll = m_Extra.FinalizeBanks();
    isAll &= m_MouthTextureAnim.FinalizeBanks();
    isAll &= m_EyeTextureAnim.FinalizeBanks();
    isAll &= m_Tops.FinalizeBanks();
    if (m_Tool.m_Size != 0) {
        isAll &= m_Tool.FinalizeBanks();
    }
    m_Num = 0;
    return isAll;
}

template <typename TModelBank>
NpcBankTables<TModelBank>::NpcBankTables()
{
}

template <typename TModelBank>
NpcBankTables<TModelBank>::~NpcBankTables()
{
    // (only the members are destroyed)
}

// 0x007CB6E0
template NpcBankTables<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > >::NpcBankTables();
// 0x007CB72C
template NpcBankTables<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > >::~NpcBankTables();
// 0x007CB650
template NpcBankTables<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > >::NpcBankTables();
// 0x007CB69C
template NpcBankTables<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> > >::~NpcBankTables();
