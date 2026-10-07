#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_ListBase.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace transport {
class PacketHandler;
class ProtocolEvent;

// RTTI N2nn3pia9transport8ProtocolE @ 0x008D02B4
// vtable 0x00902050 (vptr 0x00902058), offset_to_top 0, 9 entries
//
// Base of the protocols; ProtocolManager keeps them in a list and gives them the packet handler.
// Layout from the constructor; the member names and the names marked so are ours.
class Protocol : public ::nn::pia::common::RootObject
{
public:
    Protocol(); // 0x0045FA28 | fefates:bytes [tier B]
    virtual ~Protocol(); // 0x0045FA58 slot 0x00 | fefates:callgraph
    // 0x0045FA50 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00736E80 slot 0x08 (name after StepSequenceJob::Trace)
    virtual u16 GetProtocolType() const = 0; // 0x0011C12F slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x0045FA18 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x0045F9D8 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
    virtual nn::Result Dispatch(); // 0x0045FA20 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x0045F9D0 slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
    virtual bool IsEnableProtocolFiltering() const; // 0x00736E78 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol

    void SetPort(unsigned short port); // 0x0045F9DC | fefates:bytes [tier B]

    common::ListNode m_ListNode;       // 0x04, in the list of ProtocolManager
    ProtocolId m_ProtocolId;           // 0x0C
    PacketHandler* m_pPacketHandler;   // 0x10, set by ProtocolManager::Startup
};
ASSERT_SIZE(Protocol, 0x14);
} // namespace transport
} // namespace pia
} // namespace nn
