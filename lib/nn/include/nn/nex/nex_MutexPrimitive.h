#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace nex {
// The mutex of a CriticalSection (an os critical section). The member name is ours.
// RTTI N2nn3nex14MutexPrimitiveE @ 0x008CE36C
// vtable 0x008FCBF0 (vptr 0x008FCBF8), offset_to_top 0, 2 entries
class MutexPrimitive : public ::nn::nex::RootObject
{
public:
    MutexPrimitive() { m_CriticalSection.Initialize(); } // (inline)
    virtual ~MutexPrimitive(); // 0x003718FC slot 0x00
    // 0x003718D8 slot 0x04 (deleting dtor)

    void Enter() { m_CriticalSection.Enter(); }
    void Leave() { m_CriticalSection.Exit(); }

    nn::os::CriticalSection m_CriticalSection; // 0x04
};
ASSERT_SIZE(MutexPrimitive, 0x10);
} // namespace nex
} // namespace nn
