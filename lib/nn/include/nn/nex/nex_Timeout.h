#pragma once

#include "decomp.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {

// The expiration of an operation (e.g. a packet): a time relative to when it was set, and the
// time limit of the Core. The member names are ours.
// RTTI N2nn3nex7TimeoutE @ 0x008CF68C
// vtable 0x008FFB78 (vptr 0x008FFB80), offset_to_top 0, 2 entries
class Timeout : public ::nn::nex::RootObject
{
public:
    // (inline)
    Timeout() : m_Interval(0), m_RelativeTime(0), m_pCore(Core::GetInstance()) {}
    virtual ~Timeout(); // 0x003D3814 slot 0x00
    // 0x003D37F0 slot 0x04 (deleting dtor)

    void SetRelativeExpirationTime(int milliseconds); // 0x003D3794 | fefates:bytes [tier B]
    // the expiration time passed, or the core was interrupted before now
    bool IsExpired() const; // 0x0072E478 | fefates:bytes [tier B]

    u32 m_Interval;          // 0x04 (ms; the retransmission interval of a packet)
    Time m_Unknown0x08;      // 0x08
    Time m_ExpirationTime;   // 0x10 (0: none)
    s64 m_RelativeTime;      // 0x18
    Core* m_pCore;           // 0x20
};
ASSERT_SIZE(Timeout, 0x28);
} // namespace nex
} // namespace nn
