// Classes from the anonymous namespace of the original dNetCec.cpp

#include "decomp.h"
#include "nn/fnd/fnd_IAllocator.h"

namespace {

// vtable 0x0087C0B4 (vptr 0x0087C0BC), offset_to_top 0, 4 entries
class CecAllocator : public nn::fnd::IAllocator
{
public:
    virtual void* Allocate(size_t size, s32 alignment) {} // 0x004DC280 slot 0x00
    virtual void Free(void* p) {} // 0x004DC25C slot 0x04
    virtual ~CecAllocator() {} // 0x004DC2A8 slot 0x08, 0x004DC2A4 slot 0x0C (deleting)
};

} // namespace
