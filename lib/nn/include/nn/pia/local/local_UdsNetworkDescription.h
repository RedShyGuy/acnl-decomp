#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalNetworkDescription.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21UdsNetworkDescriptionE
// vtable 0x00900F58 (vptr 0x00900F60), offset_to_top 0, 8 entries
//
// A uds network (the description a scan returns). The member name is ours.
class UdsNetworkDescription : public ::nn::pia::local::LocalNetworkDescription
{
public:
    UdsNetworkDescription() {}
    virtual u8 GetCurrentParticipants() const; // 0x00731530 slot 0x00 | fefates:bytes
    virtual u8 GetMaxParticipants() const; // 0x0073151C slot 0x04 | fefates:bytes
    virtual bool IsOpened() const; // 0x00731598 slot 0x08 | fefates:bytes
    virtual u32 GetLocalCommunicationId() const; // 0x00731544 slot 0x0C | fefates:bytes
    virtual u8 GetSubId() const; // 0x00731584 slot 0x10 | fefates:bytes
    virtual u16 GetChannel() const; // 0x00731508 slot 0x14 (name is ours)
    virtual void GetBssid(u8* pBssid) const; // 0x0073155C slot 0x18 (name is ours)
    virtual void Copy(const nn::pia::local::LocalNetworkDescription* pDescription); // 0x0041D5B4 slot 0x1C (name is ours)

    nn::uds::CTR::NetworkDescription m_Description; // 0x004
};
ASSERT_SIZE(UdsNetworkDescription, 0x10C);
} // namespace local
} // namespace pia
} // namespace nn
