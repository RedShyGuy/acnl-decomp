#pragma once

#include "decomp.h"
#include "Bank/dBankVramConfig.h"
#include "ssys/ma/dHeapAllocator.h"
#include "sead/seadHeap.h"
#include "ssys/ma/dVramHeap.h"

// A bank of NUM slots for resources that are loaded at run time (class name from the RTTI; the members and the parameter
// names are ours): each slot has a heap of HEAP_SIZE bytes (created by the users of the bank) and two VRAM heaps
// (TVramConfig). FLAG: meaning unknown (true for some banks of the player parts).
// Instantiations found in the binary (typeinfo / vtable):
//   Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD178  vtable 0x008F834C
//   Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD180  vtable 0x008F8360
//   Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD188  vtable 0x008F8374
//   Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD190  vtable 0x008F8388
//   Bank<11776u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD198  vtable 0x008F839C
//   Bank<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >  typeinfo 0x008CD1A0  vtable 0x008F83B0
//   Bank<15872u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1A8  vtable 0x008F83C4
//   Bank<17152u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1B0  vtable 0x008F83D8
//   Bank<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >  typeinfo 0x008CD1B8  vtable 0x008F83EC
//   Bank<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >  typeinfo 0x008CD1C0  vtable 0x008F8400
//   Bank<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >  typeinfo 0x008CD1C8  vtable 0x008F8414
//   Bank<18944u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1D0  vtable 0x008F8428
//   Bank<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >  typeinfo 0x008CD1D8  vtable 0x008F843C
//   Bank<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >  typeinfo 0x008CD1E0  vtable 0x008F8450
//   Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1E8  vtable 0x008F8464
//   Bank<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >  typeinfo 0x008CD1F0  vtable 0x008F8478
//   Bank<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >  typeinfo 0x008CD1F8  vtable 0x008F848C
//   Bank<35840u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD200  vtable 0x008F84A0
//   Bank<35840u, 1u, false, BankVramConfig<8704u, 6u, 1536u, 6u, false> >  typeinfo 0x008CD208  vtable 0x008F84B4
//   Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD210  vtable 0x008F84C8
//   Bank<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >  typeinfo 0x008CD218  vtable 0x008F84DC
//   Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD220  vtable 0x008F84F0
//   Bank<8984u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD228  vtable 0x008F8504
//   Bank<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >  typeinfo 0x008CD230  vtable 0x008F8518
//   Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >  typeinfo 0x008CD238  vtable 0x008F852C
template <u32 HEAP_SIZE, u32 NUM, bool FLAG, typename TVramConfig>
class Bank
{
public:
    static const u32 SLOT_NUM = NUM;

    Bank();
    virtual ~Bank();

    // the resources of the slot are released (true: done, the heap may be cleared; false: not yet)
    virtual bool Release(u32 slot); // slot 0x08

    ssys::ma::HeapAllocator m_Allocators[NUM]; // 0x04
    ssys::ma::VramHeap* m_pVramAHeaps[NUM]; // (the VRAM heap names after libgarden)
    ssys::ma::VramHeap* m_pVramBHeaps[NUM];

    // (inline) the allocator of the slot (the first one for a slot out of range)
    ssys::ma::HeapAllocator* GetAllocator(u32 slot) { return slot < NUM ? &m_Allocators[slot] : &m_Allocators[0]; }
    // (inline) the VRAM heaps of the slot (the first ones for a slot out of range)
    ssys::ma::VramHeap* GetVramAHeap(u32 slot) const { return slot < NUM ? m_pVramAHeaps[slot] : m_pVramAHeaps[0]; }
    ssys::ma::VramHeap* GetVramBHeap(u32 slot) const { return slot < NUM ? m_pVramBHeaps[slot] : m_pVramBHeaps[0]; }
    // (inline) the resources of the slot are released (false: the slot has no heap or they are not released yet)
    bool ReleaseSlot(u32 slot)
    {
        if (GetAllocator(slot)->m_pHeap == NULL) {
            return false;
        }
        return Release(slot);
    }
    // (inline) the heaps of all slots are destroyed
    void FinalizeBank()
    {
        for (u32 i = 0; i < NUM; i++) {
            if (m_Allocators[i].m_pHeap != NULL) {
                m_Allocators[i].DestroyHeap();
            }
            if (m_pVramAHeaps[i] != NULL) {
                m_pVramAHeaps[i]->Destroy();
                m_pVramAHeaps[i] = NULL;
            }
            if (m_pVramBHeaps[i] != NULL) {
                m_pVramBHeaps[i]->Destroy();
                m_pVramBHeaps[i] = NULL;
            }
        }
    }

protected:
    // (inline) no VRAM heaps
    void ClearVramHeaps()
    {
        for (u32 i = 0; i < NUM; i++) {
            m_pVramAHeaps[i] = NULL;
            m_pVramBHeaps[i] = NULL;
        }
    }
};

template <u32 HEAP_SIZE, u32 NUM, bool FLAG, typename TVramConfig>
inline Bank<HEAP_SIZE, NUM, FLAG, TVramConfig>::Bank()
{
    ClearVramHeaps();
}

template <u32 HEAP_SIZE, u32 NUM, bool FLAG, typename TVramConfig>
inline Bank<HEAP_SIZE, NUM, FLAG, TVramConfig>::~Bank()
{
    FinalizeBank();
    ClearVramHeaps();
}

template <u32 HEAP_SIZE, u32 NUM, bool FLAG, typename TVramConfig>
inline bool Bank<HEAP_SIZE, NUM, FLAG, TVramConfig>::Release(u32 slot)
{
    return true;
}
