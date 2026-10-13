#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/os/os_Atomic.h"

namespace nn {
namespace nex {
// An object with an atomic reference count that deletes itself when the last reference is
// released. The names of the members and of the inline functions are ours.
// RTTI N2nn3nex16RefCountedObjectE @ 0x008CE614
// vtable 0x008FD234 (vptr 0x008FD23C), offset_to_top 0, 2 entries
class RefCountedObject : public ::nn::nex::RootObject
{
public:
    RefCountedObject(); // 0x003839C4 | fefates:bytes [tier B]
    virtual ~RefCountedObject(); // 0x00383A10 slot 0x00 | fefates:callgraph
    // 0x003839E4 slot 0x04 (deleting dtor)

    // (inline)
    void AcquireRef() { nn::os::detail::AtomicAdd(&m_RefCount, 1); }
    // (inline) true if the object was deleted
    bool ReleaseRef()
    {
        if (nn::os::detail::AtomicAdd(&m_RefCount, -1) == 0 && !m_IsDeleted) {
            m_IsDeleted = true;
            delete this;
            return true;
        }
        return false;
    }

    volatile s32 m_RefCount; // 0x4
    bool m_IsDeleted;        // 0x8
};
ASSERT_SIZE(RefCountedObject, 0xC);
} // namespace nex
} // namespace nn
