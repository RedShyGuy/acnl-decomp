#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// An instance that is registered in the InstanceTable under its kind and context while it exists.
// The member names are ours.
// RTTI N2nn3nex15InstanceControlE @ 0x008CE4B4
// vtable 0x008FCEC4 (vptr 0x008FCECC), offset_to_top 0, 2 entries
class InstanceControl : public ::nn::nex::RootObject
{
public:
    InstanceControl(u32 kind, u32 context); // 0x00378514
    virtual ~InstanceControl(); // 0x00378644 slot 0x00
    // 0x00378610 slot 0x04 (deleting dtor)
    // the object that contains the instance (under the lock of the instance table)
    void SetOwner(void* pOwner); // 0x003784D8 (name is ours)

    u32 m_Context;       // 0x04
    u32 m_Kind;          // 0x08
    // the object that contains the instance (e.g. the Core; null if it is the object itself)
    void* m_pOwner;      // 0x0C
    bool m_IsRegistered; // 0x10
};
ASSERT_SIZE(InstanceControl, 0x14);
} // namespace nex
} // namespace nn
