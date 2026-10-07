#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_UpdateApplicationDataJob.h"

namespace nn {
namespace pia {
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet32NexMatchUpdateApplicationDataJobE @ 0x008CFA8C
// vtable 0x009007A0 (vptr 0x009007A8), offset_to_top 0, 8 entries
//
// Changes the application data of the matchmake session through NexMatchmakeSession. The data
// is copied behind the first 128 bytes of the buffer; the step sends the bytes from m_DataOffset
// on. The step names are from the strings; the member names are ours.
class NexMatchUpdateApplicationDataJob : public ::nn::pia::session::UpdateApplicationDataJob
{
public:
    static const u32 APPLICATION_DATA_OFFSET = 128;
    static const u32 APPLICATION_DATA_SIZE_MAX = 384;

    NexMatchUpdateApplicationDataJob(); // 0x00411F78
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchUpdateApplicationDataJob(); // 0x00442D1C slot 0x00
    // 0x00411FA4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072FA6C slot 0x14
    virtual void Cleanup(); // 0x00411F4C slot 0x18
    virtual nn::Result vf_0x1C(const void* pData, u32 size); // 0x00411900 slot 0x1C

    // the steps
    common::ExecuteResult UpdateApplicationBuffer(); // 0x004119CC
    common::ExecuteResult WaitUpdateApplicationBuffer(); // 0x00411D8C

    NexMatchmakeSession* m_pSession; // 0x5C, the current matchmake session of Session
    u32 m_DataSize;                  // 0x60, of the sent bytes
    u32 m_DataOffset;                // 0x64, in m_Buffer
    u8 m_Buffer[512];                // 0x68
};
ASSERT_OFFSET(NexMatchUpdateApplicationDataJob, m_pSession, 0x5C);
ASSERT_OFFSET(NexMatchUpdateApplicationDataJob, m_Buffer, 0x68);
ASSERT_SIZE(NexMatchUpdateApplicationDataJob, 0x268);
} // namespace inet
} // namespace pia
} // namespace nn
