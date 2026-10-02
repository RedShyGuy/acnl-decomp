#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport21ReliableSlidingWindowE @ 0x008D023C
// vtable 0x00901F48 (vptr 0x00901F50), offset_to_top 0, 3 entries
class ReliableSlidingWindow : public ::nn::pia::common::RootObject
{
public:
    virtual ~ReliableSlidingWindow(); // 0x0045B45C slot 0x00 | fefates:bytes
    // 0x0045B44C slot 0x04 | slot vf_0x04 of nn::pia::transport::ReliableSlidingWindow (deleting dtor)
    virtual void vf_0x08(); // 0x00736224 slot 0x08 | virtual slot, introduced by nn::pia::transport::ReliableSlidingWindow
    void Initialize(unsigned int, unsigned int); // 0x0045A540 | fefates:bytes [tier B]
    void CanPushData(unsigned int); // 0x0045A6D8 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045AB34 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::PacketHandler*, unsigned int, nn::pia::StationIndex, nn::pia::StationIndex); // 0x0045ACF4 | fefates:bytes [tier B]
    void Dispatch(nn::pia::transport::PacketHandler*); // 0x0045ADE8 | fefates:bytes-fuzzy [tier B]
    void Finalize(); // 0x0045B14C | fefates:bytes [tier B]
    void PushData(const void*, unsigned int); // 0x0045B1A8 | fefates:bytes [tier B]
    ReliableSlidingWindow(); // 0x0045B3E0 | fefates:bytes [tier B]
    void IsInCommunication() const; // 0x00736204 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
