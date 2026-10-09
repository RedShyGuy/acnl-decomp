#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_IntrusiveLinkedList.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd8HeapBaseE @ 0x008CDEF4
// vtable 0x008FC0FC (vptr 0x008FC104), offset_to_top 0, 7 entries (all pure)
//
// The base of the heaps. RTTI: derives from IntrusiveLinkedList<HeapBase, void>::Item at +4 (the
// heaps can form a tree: the destructor stops the program while the heap still has a parent or
// children). Names of the unnamed virtual functions and of the members are ours.
class HeapBase : public IntrusiveLinkedList<HeapBase, void>::Item
{
public:
    HeapBase() : mParent(0), mOption(0) {}
    virtual ~HeapBase() = 0; // 0x0013B4C4 | nintendogs:bytes [tier A]
    virtual void FreeV(void* p) = 0; // slot 0x08
    virtual void* GetHeapStart() const = 0; // slot 0x0C
    virtual size_t GetHeapSize() const = 0; // slot 0x10
    virtual void PrintState() = 0; // slot 0x14 (empty in the heaps of ACNL)
    virtual bool HasAddress(const void* p) const = 0; // slot 0x18

    static void FillMemory32(uptr begin, uptr end, bit32 value); // 0x00136C28 | nintendogs:bytes [tier A]

protected:
    // mOption: fill allocated memory with 0
    static const bit32 OPTION_ZERO_CLEAR = 1 << 0;

    HeapBase* mParent;                            // 0x0C
    IntrusiveLinkedList<HeapBase, void> mChildren; // 0x10
    bit32 mOption;                                // 0x14
};
ASSERT_SIZE(HeapBase, 0x18);
} // namespace fnd
} // namespace nn
