#pragma once

#include "decomp.h"
#include "sead/seadHeap.h"

namespace sead {
// RTTI N4sead7ExpHeapE @ 0x008D2138
// vtable 0x00906C00 (vptr 0x00906C08), offset_to_top 0, 27 entries
// vtable 0x00906C74 (vptr 0x00906C7C), offset_to_top -24, 1 entries
class ExpHeap : public ::sead::Heap
{
public:
    struct FindMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ExpHeap(); // ctor candidate(s) 0x00133578 (unverified)
    virtual ~ExpHeap(); // 0x0055F184 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0055F154 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074F558 slot 0x08 | virtual slot, introduced by sead::Heap
    virtual void vf_0x0C(); // 0x0074F0C4 slot 0x0C | virtual slot, introduced by sead::Heap
    virtual void vf_0x10(); // 0x0055EDF4 slot 0x10 | virtual slot, introduced by sead::Heap
    virtual void vf_0x14(); // 0x0055EBF0 slot 0x14 | virtual slot, introduced by sead::Heap
    virtual void vf_0x18(); // 0x0055EEE0 slot 0x18 | virtual slot, introduced by sead::Heap
    virtual void vf_0x1C(); // 0x0055EAEC slot 0x1C | virtual slot, introduced by sead::Heap
    virtual void vf_0x20(); // 0x0055E588 slot 0x20 | virtual slot, introduced by sead::Heap
    virtual void vf_0x24(); // 0x0055E19C slot 0x24 | virtual slot, introduced by sead::Heap
    virtual void vf_0x28(); // 0x0055E2D0 slot 0x28 | virtual slot, introduced by sead::Heap
    virtual void vf_0x2C(); // 0x0055EE64 slot 0x2C | virtual slot, introduced by sead::Heap
    virtual void vf_0x30(); // 0x0074F0BC slot 0x30 | virtual slot, introduced by sead::Heap
    virtual void vf_0x34(); // 0x0074F0AC slot 0x34 | virtual slot, introduced by sead::Heap
    virtual void vf_0x38(); // 0x0074F668 slot 0x38 | virtual slot, introduced by sead::Heap
    virtual void vf_0x3C(); // 0x0074EFA4 slot 0x3C | virtual slot, introduced by sead::Heap
    virtual void vf_0x40(); // 0x0074F110 slot 0x40 | virtual slot, introduced by sead::Heap
    virtual void vf_0x44(); // 0x0074F904 slot 0x44 | virtual slot, introduced by sead::Heap
    virtual void vf_0x48(); // 0x0074EF28 slot 0x48 | virtual slot, introduced by sead::Heap
    virtual void vf_0x4C(); // 0x0074F020 slot 0x4C | virtual slot, introduced by sead::Heap
    virtual void vf_0x50(); // 0x0074F0A4 slot 0x50 | virtual slot, introduced by sead::Heap
    virtual void vf_0x54(); // 0x0074F60C slot 0x54 | virtual slot, introduced by sead::Heap
    virtual void vf_0x58(); // 0x0074F670 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x0054E1E0 slot 0x5C | virtual slot, introduced by sead::Heap
    virtual void vf_0x64(); // 0x0055E710 slot 0x64 | virtual slot, introduced by sead::ExpHeap
    virtual void vf_0x68(); // 0x0074F09C slot 0x68 | virtual slot, introduced by sead::ExpHeap
    void CreateRoot(void*, unsigned int, sead::SafeStringBase<char> const*, bool); // 0x0013351C | libgarden [tier A]
    void allocFromHead_(unsigned, int); // 0x0053C9AC | nintendogs:bytes [tier A]
    void allocFromTail_(unsigned); // 0x0053CAAC | nintendogs:bytes [tier A]
    void allocFromTail_(unsigned, int); // 0x0053CC14 | nintendogs:bytes [tier B]
    void pushToFreeList_(sead::MemBlock*); // 0x0053CD1C | nintendogs:bytes [tier A]
    void mergeFreeList_(); // 0x0053CDA4 | nintendogs:bytes [tier A]
    void createMaxSizeFreeMemBlock_(sead::ExpHeap*); // 0x0053CE64 | nintendogs:bytes [tier A]
    void tryCreate(void*, unsigned, const sead::SafeStringBase<char>&, bool); // 0x0053D400 | nintendogs:callseq [tier A]
    void tryCreate(unsigned, const sead::SafeStringBase<char>&, sead::Heap*, sead::Heap::HeapDirection, bool); // 0x0053D490 | nintendogs:callseq [tier A]
    ExpHeap(const sead::SafeStringBase<char>&, sead::Heap*, void*, unsigned, sead::Heap::HeapDirection, bool); // 0x0053D59C | nintendogs:bytes [tier A]
    void dumpUseList() const; // 0x007495CC | nintendogs:bytes [tier A]
    void dumpFreeList() const; // 0x007496B8 | nintendogs:bytes [tier A]
    void findFreeMemBlockFromHead_(unsigned, sead::ExpHeap::FindMode) const; // 0x00749928 | nintendogs:bytes [tier A]
    void findFreeMemBlockFromHead_(unsigned, int, sead::ExpHeap::FindMode) const; // 0x007499C4 | nintendogs:bytes [tier A]
    void findFreeMemBlockFromTail_(unsigned, int, sead::ExpHeap::FindMode) const; // 0x00749A90 | nintendogs:bytes [tier A]
};
} // namespace sead
