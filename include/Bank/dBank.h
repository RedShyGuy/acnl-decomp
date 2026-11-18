#pragma once

#include "decomp.h"
#include "Bank/dBankVramConfig.h"
#include "ssys/ma/dHeapAllocator.h"
#include "sead/seadHeap.h"
#include "ssys/ma/dVramHeap.h"
#include "ssys/ma/dVramExpHeap.h"
#include "graphic/dMemoryMgr.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadSafeStringBase.h"
#include "sead/seadStringFormatter.h"

// A bank of NUM slots for resources that are loaded at run time (class name from the RTTI; the members and the parameter
// names are ours): each slot has a heap of HEAP_SIZE bytes (created by the users of the bank) and two VRAM heaps
// (TVramConfig). FLAG: the slots get frame heaps (else exp heaps; CreateHeaps).
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
    // (inline; BsPlayerMgr::BankTables::Initialize) the heaps of all slots in parent: a heap of HEAP_SIZE bytes (a
    // frame heap for FLAG, else an exp heap; named name) and the two VRAM heaps of TVramConfig (named name + "VramA" /
    // "VramB" in the VRAM A / B of graphic::MemoryMgr; vramA selects the getter of the first one, the room views pass
    // it); false: a heap could not be made (the VRAM heaps are not checked)
    bool CreateHeaps(sead::Heap* parent, const char* name, u32 vramA = 0)
    {
        bool isCreated = true;
        for (u32 slot = 0; slot < NUM; slot++) {
            if (FLAG) {
                isCreated &= m_Allocators[slot].CreateFrameHeap(HEAP_SIZE, parent, sead::SafeStringBase<char>(name),
                                                                sead::Heap::HEAP_DIRECTION_FORWARD);
            } else {
                isCreated &= m_Allocators[slot].CreateExpHeap(HEAP_SIZE, parent, sead::SafeStringBase<char>(name),
                                                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
            }
            if (TVramConfig::VRAM0_SIZE != 0) {
                // (the names of the VRAM heaps of a bank type and the VRAM heaps they are made in)
                // 0x00AF9474 (guard 0x00983320) [<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >]
                // 0x00AF936C (guard 0x009832F0) [<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >]
                // 0x00AF9524 (guard 0x00983340) [<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >]
                // 0x00AF93C4 (guard 0x00983300) [<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >]
                // 0x00AF9314 (guard 0x009832E0) [<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >]
                // 0x00AF92BC (guard 0x009832D0) [<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >]
                // 0x00AF94CC (guard 0x00983330) [<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >]
                // 0x00AF941C (guard 0x00983310) [<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >]
                // 0x00AF95D4 (guard 0x00983360) [<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >]
                // 0x00AF9600 (guard 0x0098336C) [<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >]
                static sead::FixedSafeString<32> s_VramAName;
                // 0x00983328 [<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >]
                // 0x009832F8 [<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >]
                // 0x00983348 [<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >]
                // 0x00983308 [<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >]
                // 0x009832E8 [<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >]
                // 0x009832D8 [<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >]
                // 0x00983338 [<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >]
                // 0x00983318 [<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >]
                // 0x00983364 [<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >]
                // 0x00983370 [<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >]
                static ssys::ma::VramHeap* (*s_GetVramHeaps[2])() = {&graphic::MemoryMgr::GetVramAHeap,
                                                                    &graphic::MemoryMgr::GetVramBHeap};
                ssys::ma::VramHeap* vramAHeap = s_GetVramHeaps[vramA]();
                {
                    sead::StringFormatter formatter(&s_VramAName);
                    formatter.append("%sVramA").append(name).flush();
                }
                m_pVramAHeaps[slot] =
                    ssys::ma::VramExpHeap::Create(TVramConfig::VRAM0_SIZE, sead::SafeStringBase<char>(s_VramAName.getCString()), vramAHeap,
                                                  ssys::ma::VramExpHeap::GetHeapSize(TVramConfig::VRAM0_PARAM), parent);
                // (no B heap for a configuration without one)
                if (TVramConfig::VRAM1_SIZE != 0) {
                    // 0x00AF94A0 (guard 0x00983324) [<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >]
                    // 0x00AF9398 (guard 0x009832F4) [<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >]
                    // 0x00AF9550 (guard 0x00983344) [<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >]
                    // 0x00AF93F0 (guard 0x00983304) [<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >]
                    // 0x00AF9340 (guard 0x009832E4) [<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >]
                    // 0x00AF92E8 (guard 0x009832D4) [<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >]
                    // 0x00AF94F8 (guard 0x00983334) [<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >]
                    // 0x00AF9448 (guard 0x00983314) [<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >]
                    static sead::FixedSafeString<32> s_VramBName;
                    ssys::ma::VramHeap* vramB = s_GetVramHeaps[1]();
                    {
                        sead::StringFormatter formatter(&s_VramBName);
                        formatter.append("%sVramB").append(name).flush();
                    }
                    m_pVramBHeaps[slot] =
                        ssys::ma::VramExpHeap::Create(TVramConfig::VRAM1_SIZE, sead::SafeStringBase<char>(s_VramBName.getCString()), vramB,
                                                      ssys::ma::VramExpHeap::GetHeapSize(TVramConfig::VRAM1_PARAM), parent);
                }
            }
        }
        return isCreated;
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
inline bool Bank<HEAP_SIZE, NUM, FLAG, TVramConfig>::Release(u32 /* slot */)
{
    return true;
}
