#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport7StationE @ 0x008D02A8
// vtable 0x00902044 (vptr 0x0090204C), offset_to_top 0, 1 entries
class Station : public ::nn::pia::common::RootObject
{
public:
    struct IdentificationInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PlayerName { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Station(); // ctor candidate(s) 0x0045F7E4 (unverified)
    virtual void vf_0x00(); // 0x00736DC4 slot 0x00 | virtual slot, introduced by nn::pia::transport::Station
    void Initialize(nn::pia::transport::NetworkFactory*); // 0x0045F338 | fefates:bytes [tier B]
    void CleanupJobs(); // 0x0045F3A0 | fefates:bytes [tier B]
    void GetPrincipalId(unsigned int*); // 0x0045F43C | fefates:bytes [tier B]
    void GetPlayerName(nn::pia::transport::Station::PlayerName*); // 0x0045F5C0 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045F5FC | fefates:bytes [tier B]
    void Startup(nn::pia::transport::StationProtocol*); // 0x0045F658 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::StationProtocol*, nn::pia::StationIndex, const nn::pia::common::StationAddress&); // 0x0045F6D4 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::StationProtocol*, const nn::pia::common::StationAddress&); // 0x0045F740 | fefates:bytes [tier B]
    void Finalize(); // 0x0045F784 | fefates:bytes [tier B]
    ~Station(); // 0x0045F894 | fefates:bytes [tier B]
    void IsConnectionRouteRelay() const; // 0x00736D90 | fefates:bytes [tier B]
    void IsConnectionRouteDirect() const; // 0x00736DAC | fefates:bytes [tier B]
    void GetRtt(unsigned int) const; // 0x00736DC8 | fefates:bytes [tier B]
    void GetRtt() const; // 0x00736E24 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
