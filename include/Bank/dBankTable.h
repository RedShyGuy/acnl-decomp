#pragma once

#include <cstring>

#include "decomp.h"
#include "Bank/dBankTableBase.h"

// A table of N banks of type T (class name from the RTTI; the member names are ours): m_Size of them are in use,
// m_BankBits marks the handed-out ones (BankTableBase::Alloc/Free).
// Instantiations found in the binary:
//   BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD650  vtable 0x008FA158
//   BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD65C  vtable 0x008FA180
//   BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>  typeinfo 0x008CD668  vtable 0x008FA1A8
//   BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>  typeinfo 0x008CD674  vtable 0x008FA1D0
//   BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD680  vtable 0x008FA1F8
//   BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>  typeinfo 0x008CD68C  vtable 0x008FA220
//   BankTable<HumanResTextureAnimBank, 10u>  typeinfo 0x008CD638  vtable 0x008FA108
//   BankTable<HumanResTextureAnimBank, 14u>  typeinfo 0x008CD644  vtable 0x008FA130
//   BankTable<NpcTopsBank, 10u>  typeinfo 0x008CD5CC  vtable 0x008F9FA0
//   BankTable<PlayerAcceBank, 7u>  typeinfo 0x008CD5E4  vtable 0x008F9FF0
//   BankTable<PlayerBottomsBank, 7u>  typeinfo 0x008CD62C  vtable 0x008FA0E0
//   BankTable<PlayerCapBank, 7u>  typeinfo 0x008CD5D8  vtable 0x008F9FC8
//   BankTable<PlayerFaceBank, 14u>  typeinfo 0x008CD5F0  vtable 0x008FA018
//   BankTable<PlayerHeadBank, 7u>  typeinfo 0x008CD5FC  vtable 0x008FA040
//   BankTable<PlayerLegsBank, 7u>  typeinfo 0x008CD608  vtable 0x008FA068
//   BankTable<PlayerShoesBank, 7u>  typeinfo 0x008CD620  vtable 0x008FA0B8
//   BankTable<PlayerTopsBank, 7u>  typeinfo 0x008CD614  vtable 0x008FA090
//   BankTable<ToolBank, 10u>  typeinfo 0x008CD698  vtable 0x008FA248
//   BankTable<ToolBank, 7u>  typeinfo 0x008CD6A4  vtable 0x008FA270
template <typename T, u32 N>
class BankTable : public BankTableBase
{
public:
    BankTable() { std::memset(m_BankBits, 0, sizeof(m_BankBits)); }

    virtual bool Release(u32 index) override; // slot 0x00
    virtual bool IsValid(u32 index) override; // slot 0x04
    virtual bool CheckBit(u32 index) override; // slot 0x08
    virtual void SetBit(u32 index) override; // slot 0x0C
    virtual void ClearBit(u32 index) override; // slot 0x10
    virtual void ResetBit() override; // slot 0x14
    virtual ~BankTable(); // slot 0x18

    // (inline; BsPlayerMgr::BankTables) num of the banks with heaps in heap (named name; false: fewer than num or a
    // heap is missing)
    bool Initialize(sead::Heap* heap, u32 num, const char* name)
    {
        m_Size = Min(num, N);
        bool isAll = m_Size == num;
        for (T* bank = m_Banks; bank < m_Banks + m_Size; bank++) {
            if (!bank->CreateHeaps(heap, name)) {
                isAll = false;
            }
        }
        return isAll;
    }
    // (inline; BsPlayerMgr::BankTables) the heaps of all banks are destroyed (false: a bank is handed out still)
    bool FinalizeTable()
    {
        if (m_UsedNum != 0) {
            return false;
        }
        u32 index = 0;
        for (T* bank = m_Banks; bank < m_Banks + m_Size; bank++, index++) {
            CheckBit(index); // (the result is not used)
            bank->FinalizeBank();
        }
        m_Size = 0;
        return true;
    }

    T m_Banks[N];                 // 0x0C
    u8 m_BankBits[(N + 7) / 8];
};

template <typename T, u32 N>
inline bool BankTable<T, N>::Release(u32 index)
{
    if (index >= m_Size) {
        return true;
    }
    T& bank = m_Banks[index];
    bool isReleased = true;
    for (u32 slot = 0; slot < T::SLOT_NUM; slot++) {
        isReleased &= bank.ReleaseSlot(slot);
    }
    return isReleased;
}

template <typename T, u32 N>
inline bool BankTable<T, N>::IsValid(u32 index)
{
    if (index >= m_Size) {
        return false;
    }
    T& bank = m_Banks[index];
    for (u32 slot = 0; slot < T::SLOT_NUM; slot++) {
        if (bank.m_Allocators[slot].m_pHeap != NULL) {
            return true;
        }
    }
    return true;
}

template <typename T, u32 N>
inline bool BankTable<T, N>::CheckBit(u32 index)
{
    if (index >= N) {
        return false;
    }
    return (m_BankBits[index >> 3] >> (index & 7)) & 1;
}

template <typename T, u32 N>
inline void BankTable<T, N>::SetBit(u32 index)
{
    if (index < N) {
        m_BankBits[index >> 3] |= 1 << (index & 7);
    }
}

template <typename T, u32 N>
inline void BankTable<T, N>::ClearBit(u32 index)
{
    if (index < N) {
        m_BankBits[index >> 3] &= ~(1 << (index & 7));
    }
}

template <typename T, u32 N>
inline void BankTable<T, N>::ResetBit()
{
    std::memset(m_BankBits, 0, sizeof(m_BankBits));
}

template <typename T, u32 N>
inline BankTable<T, N>::~BankTable()
{
    u32 index = 0;
    for (T* bank = m_Banks; bank < m_Banks + m_Size; bank++, index++) {
        CheckBit(index); // (the result is not used)
        bank->FinalizeBank();
    }
    m_Size = 0;
    Clear();
    std::memset(m_BankBits, 0, sizeof(m_BankBits));
}
