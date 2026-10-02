#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15ProtocolManagerE @ 0x008D01A0
// vtable 0x00901DE0 (vptr 0x00901DE8), offset_to_top 0, 1 entries
class ProtocolManager : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x007352EC slot 0x00 | virtual slot, introduced by nn::pia::transport::ProtocolManager
    void AllocProtocol(unsigned int); // 0x00450858 | fefates:bytes [tier B]
    void SearchProtocol(nn::pia::transport::ProtocolId, unsigned short); // 0x004508A4 | fefates:bytes [tier B]
    void DestroyProtocol(unsigned int); // 0x00450904 | fefates:bytes [tier B]
    void CleanupProtocols(); // 0x004509BC | fefates:bytes [tier B]
    void StartupProtocols(nn::pia::StationIndex); // 0x00450A14 | fefates:bytes [tier B]
    void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x00450E4C | fefates:bytes [tier B]
    void Cleanup(); // 0x00450ED8 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::PacketHandler*); // 0x00450F30 | fefates:bytes [tier B]
    void Dispatch(); // 0x00450FBC | fefates:bytes [tier B]
    void Finalize(); // 0x00451034 | fefates:bytes [tier B]
    ProtocolManager(); // 0x00451090 | fefates:bytes [tier B]
    ~ProtocolManager(); // 0x004510CC | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
