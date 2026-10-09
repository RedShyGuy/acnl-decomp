#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace err {
namespace CTR {
// what ThrowFatalErr reports to the err:f service (3dbrew: ERR:Throw, FatalErrInfo; the field
// names after 3dbrew)
struct FatalErrInfo
{
    u8 type;            // 0x00, nnerrFatalErrType
    u8 revisionHigh;    // 0x01
    u16 revisionLow;    // 0x02
    bit32 resultCode;   // 0x04
    uptr pcAddress;     // 0x08
    u32 processId;      // 0x0C
    u64 titleId;        // 0x10
    u64 appletTitleId;  // 0x18
    u8 data[0x60];      // 0x20, by type (exception data, failure message)
};
ASSERT_OFFSET(FatalErrInfo, processId, 0x0C);
ASSERT_SIZE(FatalErrInfo, 0x80);

// the session of the err:f port (the class name is from the reference symbols; the member name is
// ours)
class FatalErr
{
public:
    explicit FatalErr(nn::Handle session) : m_Session(session) {}

    // err:f command 0x0001 (3dbrew: ThrowFatalError)
    nn::Result Throw(const nn::err::CTR::FatalErrInfo& info); // 0x00130C84 | nintendogs:bytes [tier A]

    nn::Handle m_Session; // 0x0
};
} // namespace CTR
} // namespace err
} // namespace nn
