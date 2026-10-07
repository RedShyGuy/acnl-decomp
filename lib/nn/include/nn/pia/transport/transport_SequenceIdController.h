#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport20SequenceIdControllerE @ 0x008D0218
// vtable 0x00901EEC (vptr 0x00901EF4), offset_to_top 0, 3 entries
//
// The sequence ids of the messages to one station (Station has one at 0x38): the id of the next
// message sent and the check of a received one, with counters of the received and the lost
// messages. Id 0 is not used (sent messages skip it, received ones with id 0 are not counted).
// Layout from Startup; the member names are ours.
class SequenceIdController : public ::nn::pia::common::RootObject
{
public:
    SequenceIdController(); // 0x00458A48 | fefates:bytes [tier B]
    virtual ~SequenceIdController(); // 0x00458A5C slot 0x00
    // 0x00458A58 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007360AC slot 0x08

    nn::Result Startup(); // 0x00458A1C | fefates:bytes [tier B]
    u16 GetNextSendSequenceId(); // 0x004588EC | fefates:bytes [tier B]
    // false for an old or repeated id
    bool CheckReceivedSequenceId(unsigned short sequenceId); // 0x00458910 | fefates:bytes [tier B]

    u16 m_SendSequenceId;     // 0x04
    u16 m_ReceivedSequenceId; // 0x06
    // the counters while m_IsCounting (16 bit, it stops when one is full)
    u16 m_ReceivedNum;        // 0x08
    u16 m_LostNum;            // 0x0A
    u16 m_NoSequenceIdNum;    // 0x0C
    bool m_IsCounting;        // 0x0E
    // the counters in total (32 bit, they stay at their maximum)
    u32 m_TotalReceivedNum;   // 0x10
    u32 m_TotalLostNum;       // 0x14
};
ASSERT_SIZE(SequenceIdController, 0x18);
} // namespace transport
} // namespace pia
} // namespace nn
