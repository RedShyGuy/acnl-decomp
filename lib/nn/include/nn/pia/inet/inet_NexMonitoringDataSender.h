#pragma once

#include "decomp.h"
#include "nn/pia/common/common_MonitoringDataSender.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexMonitoringDataSenderE @ 0x008CF9C0
// vtable 0x009004D8 (vptr 0x009004E0), offset_to_top 0, 6 entries
//
// Sends the monitoring data to the nex server (NgsBridgeInterface::SendReport): serialized,
// compressed with zlib, padded to 16 bytes and encrypted with AES. The member names are ours.
class NexMonitoringDataSender : public ::nn::pia::common::MonitoringDataSender
{
public:
    // the sizes of the buffers
    static const u32 DATA_BUFFER_SIZE = 2224;
    static const u32 COMPRESS_BUFFER_SIZE = 2288;
    static const u32 ZLIB_WORK_BUFFER_SIZE = 0x26C4;

    // the result of sendCore (names are ours)
    enum SendResult
    {
        SEND_RESULT_SUCCESS = 0,
        SEND_RESULT_TOO_LARGE = 1,
        SEND_RESULT_FAILURE = 2,
    };

    NexMonitoringDataSender(); // 0x00404E3C
    virtual ~NexMonitoringDataSender(); // 0x00404FB8 slot 0x00
    // 0x00404F24 slot 0x04 (deleting dtor)
    // too large data goes once more without the state content
    virtual void Send(u8 phase); // 0x00404AC4 slot 0x10
    virtual void Trace(u64 flag) const; // 0x0072F52C slot 0x14

    // (name is ours)
    SendResult sendCore(u8 phase, bool isShort); // 0x00404B00

    u8* m_pDataBuffer;      // 0x08
    u8* m_pCompressBuffer;  // 0x0C
    u8* m_pZlibWorkBuffer;  // 0x10
};
ASSERT_SIZE(NexMonitoringDataSender, 0x14);
} // namespace inet
} // namespace pia
} // namespace nn
