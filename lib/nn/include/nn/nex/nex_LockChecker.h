#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The lock checker of a CriticalSection: the level of the lock and the state of the checks (only
// initialized in this build). The member names are ours.
// RTTI N2nn3nex11LockCheckerE @ 0x008CE030
// vtable 0x008FC2FC (vptr 0x008FC304), offset_to_top 0, 2 entries
class LockChecker : public ::nn::nex::RootObject
{
public:
    explicit LockChecker(u32 level) : m_IsChecked(false), m_Level(level), m_Unknown0x0C(0), m_Unknown0x10(0) {} // (inline)
    virtual ~LockChecker(); // 0x0035874C slot 0x00
    // 0x00358734 slot 0x04 (deleting dtor)

    bool m_IsChecked;  // 0x04
    u32 m_Level;       // 0x08
    u32 m_Unknown0x0C; // 0x0C
    u32 m_Unknown0x10; // 0x10
};
ASSERT_SIZE(LockChecker, 0x14);
} // namespace nex
} // namespace nn
