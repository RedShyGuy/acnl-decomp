#include "ssys/ma/dAllocator.h"
#include "sead/seadHeap.h"

namespace ssys {
namespace ma {
// (the alignment is also added to the size)
// 0x0056B21C slot 0x08 (name after nw::os::IAllocator)
void* ssys::ma::Allocator::Alloc(size_t size, u8 alignment)
{
    if (alignment == 0) {
        alignment = 1;
    }
    return m_pHeap->tryAlloc(size + alignment, alignment);
}

// 0x0056B20C slot 0x0C (name after nw::os::IAllocator)
void ssys::ma::Allocator::Free(void* p)
{
    m_pHeap->free(p);
}

// 0x0056B23C slot 0x00 | libgarden
// 0x0056B238 (deleting dtor)
ssys::ma::Allocator::~Allocator()
{
    // empty (in the original too)
}

} // namespace ma
} // namespace ssys
