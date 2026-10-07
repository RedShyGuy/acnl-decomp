#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalSendMessageJobE @ 0x008CFBBC
// vtable 0x00900CFC (vptr 0x00900D04), offset_to_top 0, 6 entries
//
// Sends a system message again and again until all stations answered it (LocalAckMessage with
// the value of the message). The layout is from the constructor; the member names and the names of
// the unnamed functions are ours.
class LocalSendMessageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    static const u32 BUFFER_SIZE = 1500;
    static const s32 RESEND_INTERVAL_MSEC = 500;

    LocalSendMessageJob(); // 0x0041A660 | fefates:bytes [tier B]
    virtual ~LocalSendMessageJob(); // 0x0041A750 slot 0x00
    // 0x0041A6F8 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311D0 slot 0x14

    void ReceiveAck(u8 transportId, u32 ackValue); // 0x0041A404 | fefates:bytes [tier B]
    common::ExecuteResult SendMessage(); // 0x0041A430
    void EndSendMessage(u8 transportId); // 0x0041A570 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041A590
    nn::Result Startup(); // 0x0041A5B4 | fefates:bytes [tier B]
    // the next message; the stations of the bitmap have to answer (all at once with isBroadcast)
    nn::Result SetMessage(u8 type, const void* pData, u32 size, u32 ackValue, u32 destinationBitmap, bool isBroadcast); // 0x0041A5F8

    u32 m_DestinationBitmap;          // 0x40, the stations that did not answer yet
    bool m_IsBroadcast;               // 0x44
    u32 m_AckValue;                   // 0x48
    common::CriticalSection m_CriticalSection; // 0x4C
    u8 m_Type;                        // 0x58
    u32* m_pBuffer;                   // 0x5C
    u32 m_Size;                       // 0x60
    common::Time m_SendTime;          // 0x68
    s32 m_ResendIntervalMSec;         // 0x70
};
ASSERT_OFFSET(LocalSendMessageJob, m_pBuffer, 0x5C);
ASSERT_SIZE(LocalSendMessageJob, 0x78);
} // namespace local
} // namespace pia
} // namespace nn
