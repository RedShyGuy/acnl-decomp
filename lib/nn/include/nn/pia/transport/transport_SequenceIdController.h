#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport20SequenceIdControllerE @ 0x008D0218
// vtable 0x00901EEC (vptr 0x00901EF4), offset_to_top 0, 3 entries
class SequenceIdController : public ::nn::pia::common::RootObject
{
public:
    SequenceIdController(); // ctor candidate(s) 0x00458A48 (unverified)
    virtual void vf_0x00(); // 0x00458A5C slot 0x00 | virtual slot, introduced by nn::pia::transport::SequenceIdController
    virtual void vf_0x04(); // 0x00458A58 slot 0x04 | virtual slot, introduced by nn::pia::transport::SequenceIdController
    virtual void vf_0x08(); // 0x007360AC slot 0x08 | virtual slot, introduced by nn::pia::transport::SequenceIdController
    void GetNextSendSequenceId(); // 0x004588EC | fefates:bytes [tier B]
    void CheckReceivedSequenceId(unsigned short); // 0x00458910 | fefates:bytes [tier B]
    void Startup(); // 0x00458A1C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
