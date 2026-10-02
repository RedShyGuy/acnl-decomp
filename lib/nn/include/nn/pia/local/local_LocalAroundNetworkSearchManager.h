#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local31LocalAroundNetworkSearchManagerE @ 0x008CFD9C
// vtable 0x009013B8 (vptr 0x009013C0), offset_to_top 0, 10 entries
class LocalAroundNetworkSearchManager : public ::nn::pia::common::RootObject
{
public:
    class LocalAroundNetworkSearchCommandAckMessage;
    class LocalAroundNetworkSearchCommandMessage;
    class LocalAroundNetworkStatusAckMessage;
    class LocalAroundNetworkStatusMessage;
    struct AroundNetworkStatus { u32 _unknown; }; // TODO: real type unknown (placeholder)
    LocalAroundNetworkSearchManager(); // ctor candidate(s) 0x004248F4 (unverified)
    virtual ~LocalAroundNetworkSearchManager(); // 0x004249C0 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x0042499C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchManager
    virtual void Initialize(); // 0x00423984 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004247EC slot 0x0C | fefates:bytes
    virtual void CreateLocalAroundNetworkInfo(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void CreateLocalAroundNetworkSearchBackgroundJob(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void SerializeAroundNetworkStatus(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage*, const nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus*) const; // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x007317F0 slot 0x24 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchManager
    void EndSendMessage(unsigned char); // 0x00423A40 | fefates:bytes [tier B]
    void EndSendMessage(); // 0x00423AC0 | fefates:bytes [tier B]
    void StartSendCommandMessage(unsigned char); // 0x00423B44 | fefates:bytes [tier B]
    void PrepareNextCommandStatus(); // 0x00423B5C | fefates:bytes [tier B]
    void SendAroundNetworkStatusMessage(); // 0x00423B90 | fefates:bytes-fuzzy [tier B]
    void GetMessageDestBitmap() const; // 0x00731770 | fefates:bytes [tier B]
    void IsSendingStatusMessage() const; // 0x00731788 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
