#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_HeapBase.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd11ExpHeapBaseE @ 0x008CDE40
// vtable 0x008FC030 (vptr 0x008FC038), offset_to_top 0, 7 entries
class ExpHeapBase : public ::nn::fnd::HeapBase
{
public:
    struct AllocationMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ExpHeapBase(); // ctor address unknown
    virtual ~ExpHeapBase(); // 0x003523A4 slot 0x00, 0x00352360 slot 0x04 (deleting)
    virtual void FreeV(void* p); // 0x00352340 slot 0x08
    virtual void* GetHeapStart() const; // 0x00729710 slot 0x0C (name is ours)
    virtual size_t GetHeapSize() const; // 0x00729700 slot 0x10 (name is ours)
    virtual void PrintState(); // 0x0072971C slot 0x14 (name is ours)
    virtual bool HasAddress(const void* p) const; // 0x007296E0 slot 0x18 | fefates:bytes
    void Initialize(unsigned, unsigned, unsigned); // 0x0011FF0C | nintendogs:bytes [tier A]
    void Invalidate(); // 0x001368EC | nintendogs:bytes [tier A]
    void Allocate(unsigned, int, unsigned char, nn::fnd::ExpHeapBase::AllocationMode, bool); // 0x00136918 | nintendogs:bytes [tier A]
    void Finalize(); // 0x0013697C | nintendogs:bytes [tier A]
    void Free(void*); // 0x0013EC84 | nintendogs:callgraph [tier A]
};
} // namespace fnd
} // namespace nn
