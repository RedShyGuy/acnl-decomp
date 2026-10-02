#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11NatDetecterE @ 0x008CF818
// vtable 0x008FFE6C (vptr 0x008FFE74), offset_to_top 0, 12 entries
class NatDetecter : public ::nn::pia::common::RootObject
{
public:
    class SendNatCheckMessageList;
    struct SendNatCheckMessage { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~NatDetecter(); // 0x003E3A08 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x003E39C0 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
    virtual void Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&); // 0x003E375C slot 0x08 | fefates:callseq
    virtual void Cleanup(); // 0x003E3698 slot 0x0C | fefates:bytes
    virtual void Retry(); // 0x004296DC slot 0x10 | fefates:bytes
    virtual void StartSendingMessage(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void CheckAllMessage(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void HandleResult(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void CheckRetry(); // 0x003E2F28 slot 0x20 | slot vf_0x20 of nn::pia::inet::NatDetecter
    virtual void GetDetectionTimeout() const; // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void StartDetectionJob(); // 0x004288B8 slot 0x28 | slot vf_0x28 of nn::pia::inet::NatDetecter
    virtual void CancelDetectionJob(); // 0x00427438 slot 0x2C | fefates:bytes
    void CloseSocket(); // 0x003E3078 | fefates:bytes [tier B]
    void AddSendMessage(const nn::pia::inet::NatDetecter::SendNatCheckMessage&, unsigned short); // 0x003E3148 | fefates:bytes [tier B]
    void sendDummyMessage(); // 0x003E3300 | fefates:bytes [tier B]
    void SendAllQueuedMessage(); // 0x003E3468 | fefates:bytes [tier B]
    void GetDifferentPortNumber(unsigned short); // 0x003E3568 | fefates:bytes [tier B]
    void InitializeReceiveMessage(); // 0x003E35E4 | fefates:bytes [tier B]
    void TraceReceivedMessageArray(unsigned long long); // 0x003E3618 | fefates:bytes [tier B]
    void GetPrimaryServerPrimaryPortAddress(); // 0x003E362C | fefates:bytes [tier B]
    void GetPrimaryServerSecondaryPortAddress(); // 0x003E365C | fefates:bytes [tier B]
    NatDetecter(); // 0x003E38D0 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
