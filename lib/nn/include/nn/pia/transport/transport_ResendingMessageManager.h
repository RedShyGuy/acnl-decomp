#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport23ResendingMessageManagerE @ 0x008D026C
// vtable 0x00901FAC (vptr 0x00901FB4), offset_to_top 0, 1 entries
class ResendingMessageManager : public ::nn::pia::common::RootObject
{
public:
    ResendingMessageManager(); // ctor candidate(s) 0x0045CA08, 0x0045CBEC (unverified)
    virtual void vf_0x00(); // 0x0073676C slot 0x00 | virtual slot, introduced by nn::pia::transport::ResendingMessageManager
    void Initialize(unsigned int); // 0x0045C6B0 | fefates:bytes [tier B]
    void StopResending(unsigned int); // 0x0045C988 | fefates:bytes [tier B]
    void CreateInstance(); // 0x0045CA08 | fefates:bytes [tier B]
    void SetSendMessage(unsigned int*, const unsigned char*, unsigned int, nn::pia::StationIndex, const nn::pia::common::StationAddress&, nn::pia::transport::ProtocolId, long long); // 0x0045CA80 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x0045CBEC | fefates:bytes [tier B]
    void Cleanup(); // 0x0045CC30 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::PacketHandler*); // 0x0045CC44 | fefates:bytes [tier B]
    void Dispatch(); // 0x0045CC68 | fefates:bytes [tier B]
    void Finalize(); // 0x0045CE04 | fefates:bytes [tier B]
    void CheckNowResending(unsigned int) const; // 0x007366C0 | fefates:bytes [tier B]
    void ExtractAckIdFromMessage(const unsigned char*, unsigned int) const; // 0x00736750 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
