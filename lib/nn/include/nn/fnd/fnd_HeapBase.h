#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd8HeapBaseE @ 0x008CDEF4
// vtable 0x008FC0FC (vptr 0x008FC104), offset_to_top 0, 7 entries (all pure)
//
// The base of the heaps. RTTI: derives from IntrusiveLinkedList<HeapBase, void>::Item at +4 (the
// heaps form a tree); that part (+0x04..+0x13) is not worked out yet and kept as words.
// Names of the unnamed virtual functions and of the members are ours.
class HeapBase
{
public:
    HeapBase() : mLinks(), mOption(0) {}
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

    uptr mLinks[4];     // 0x04, IntrusiveLinkedList<HeapBase, void>::Item and the child heaps
    bit32 mOption;      // 0x14
};
ASSERT_SIZE(HeapBase, 0x18);
} // namespace fnd
} // namespace nn
