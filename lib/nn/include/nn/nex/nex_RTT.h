#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The round trip time of an end point: the smoothed time (times 8) and its deviation (times 4)
// after the usual estimation. The names of the members and functions are ours.
// RTTI N2nn3nex3RTTE @ 0x008CF47C
// vtable 0x008FF8C0 (vptr 0x008FF8C8), offset_to_top 0, 2 entries
class RTT : public ::nn::nex::RootObject
{
public:
    // the smallest deviation part of the timeout (ms)
    static const u32 MIN_DEVIATION = 33;

    // (inline)
    explicit RTT(u32 initial) : m_Smoothed(initial << 3), m_Deviation(0), m_Last(initial), m_IsFirst(true) {}
    virtual ~RTT(); // 0x003CD050 slot 0x00
    // 0x003CD038 slot 0x04 (deleting dtor)

    // (inline) a new sample
    void Adjust(u32 sample)
    {
        if (m_IsFirst) {
            m_IsFirst = false;
            m_Last = sample;
            m_Smoothed = sample << 3;
            m_Deviation = sample << 1;
            return;
        }
        s32 error = sample - (m_Smoothed >> 3);
        m_Smoothed += error;
        m_Last = sample;
        if (error < 0) {
            error = -error;
        }
        m_Deviation += error - (m_Deviation >> 2);
    }
    // (inline) the retransmission timeout
    u32 GetRTO() const
    {
        u32 deviation = (m_Deviation >> 2) << 2;
        return (m_Smoothed >> 3) + (deviation < MIN_DEVIATION ? MIN_DEVIATION : deviation);
    }

    u32 m_Smoothed;   // 0x04
    u32 m_Deviation;  // 0x08
    u32 m_Last;       // 0x0C
    bool m_IsFirst;   // 0x10
};
ASSERT_SIZE(RTT, 0x14);
} // namespace nex
} // namespace nn
