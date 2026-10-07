#pragma once

#include "decomp.h"
#include "nn/pia/common/common_LatestMedian.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace transport {
// The round trip times to one station (RttProtocol has an array of them, allocated with the
// operator new[] of RootObject). The members and SetSendTime are ours.
class RttCalculator : public common::RootObject
{
public:
    RttCalculator(); // 0x0044EE54 | fefates:bytes [tier B]
    // (empty, but out of line: RttProtocol deletes the array with a cookie)
    ~RttCalculator(); // 0x0044EE80 (name is ours)
    void Startup(); // 0x0044EE2C | fefates:bytes [tier B]
    void Cleanup(); // 0x0044EDFC | fefates:bytes [tier B]

    // the time of the last request (RttProtocol)
    void SetSendTime(const common::Time& time); // 0x0044EDB0 (name is ours)
    void Update(int rtt); // 0x0044EDC0 | fefates:bytes [tier B]

    // the median of the latest num round trip times, -1 without values or when not started
    int GetRtt(unsigned int num) const; // 0x00734D58 | fefates:bytes [tier B]
    int GetRtt() const; // 0x00734D80 | fefates:bytes [tier B]
    // no answer since the last request for 2000 ms (200 ms while the ring is not full)
    bool IsTimeOut() const; // 0x00734DAC | fefates:bytes [tier B]

    // the timeout in ms with a full ring / with fewer values; in .data, not .rodata, so not
    // const (name is ours)
    static s32 s_TimeOutMSec[2];

    bool m_IsActive;                        // 0x00
    s32 m_LastRtt;                          // 0x04
    common::Time m_SendTime;                // 0x08
    common::LatestMedian<int, 16> m_Rtts;   // 0x10
};
ASSERT_OFFSET(RttCalculator, m_Rtts, 0x10);
ASSERT_SIZE(RttCalculator, 0x60);
} // namespace transport
} // namespace pia
} // namespace nn
