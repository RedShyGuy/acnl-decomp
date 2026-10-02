#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet9NexFacadeE @ 0x008CFAB0
// vtable 0x00900804 (vptr 0x0090080C), offset_to_top 0, 16 entries
class NexFacade : public ::nn::pia::common::RootObject
{
public:
    struct LoginInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    NexFacade(); // ctor candidate(s) 0x0041307C (unverified)
    virtual void Bind(nn::pia::inet::NexFacade::LoginInfo*); // 0x00413624 slot 0x00 | fefates:bytes
    virtual void Unbind(); // 0x00413694 slot 0x04 | slot vf_0x04 of nn::pia::inet::NexFacade
    virtual void Startup(nn::nex::MatchMakingClient*); // 0x00413748 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexFacade
    virtual void Cleanup(); // 0x004136A4 slot 0x0C | fefates:bytes
    virtual void vf_0x10(); // 0x004131B0 slot 0x10 | virtual slot, introduced by nn::pia::inet::NexFacade
    virtual void StartNatSessionAsync(); // 0x00413268 slot 0x14 | slot vf_0x14 of nn::pia::inet::NexFacade
    virtual void IsCompletedStartNatSession(); // 0x004133B0 slot 0x18 | fefates:bytes
    virtual void GetStartNatSessionResult(); // 0x00413360 slot 0x1C | fefates:bytes
    virtual void vf_0x20(); // 0x00413388 slot 0x20 | virtual slot, introduced by nn::pia::inet::NexFacade
    virtual void StopNatSession(); // 0x004268B8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NexFacade
    virtual void vf_0x28(); // 0x0072FAD0 slot 0x28 | virtual slot, introduced by nn::pia::inet::NexFacade
    virtual void vf_0x2C(); // 0x0041395C slot 0x2C | virtual slot, introduced by nn::pia::inet::NexFacade
    virtual ~NexFacade(); // 0x0041393C slot 0x30 | slot vf_0x30 of nn::pia::inet::NexFacade
    virtual void initialize(); // 0x00412FB4 slot 0x34 | fefates:bytes
    virtual void finalize(); // 0x004138B0 slot 0x38 | fefates:bytes
    virtual void startNatSessionCore(nn::pia::common::CallContext*); // 0x004131BC slot 0x3C | fefates:bytes
    void ConvertInetAddressToNexInetAddress(const nn::pia::common::InetAddress&, nn::nex::InetAddress*); // 0x00357848 | fefates:bytes [tier B]
    void IsEdmMapping(const nn::pia::transport::StationLocation&); // 0x00413058 | fefates:bytes [tier B]
    void CreateInstance(); // 0x0041307C | fefates:bytes [tier B]
    void CreateProtocols(); // 0x0041314C | fefates:bytes [tier B]
    void DestroyInstance(); // 0x00413168 | fefates:bytes [tier B]
    void CompleteStartNatSession(unsigned short); // 0x004132B8 | fefates:bytes [tier B]
    void ConvertNexStationUrlToStationLocation(const nn::nex::StationURL&, nn::pia::transport::StationLocation*); // 0x004133D4 | fefates:bytes [tier B]
    void RegisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler*); // 0x004134A0 | fefates:bytes [tier B]
    void UnregisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler*); // 0x00413554 | fefates:bytes [tier B]
    void IsGlobal(const nn::pia::transport::StationLocation&); // 0x0041387C | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
