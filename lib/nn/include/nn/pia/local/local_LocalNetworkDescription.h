#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// A network that a scan found (or the next one of a host migration). The slot names are from the
// symbols of UdsNetworkDescription where they have one; the others are ours.
class LocalNetworkDescription : public ::nn::pia::common::RootObject
{
public:
    virtual u8 GetCurrentParticipants() const = 0; // slot 0x00
    virtual u8 GetMaxParticipants() const = 0;     // slot 0x04
    virtual bool IsOpened() const = 0;             // slot 0x08
    virtual u32 GetLocalCommunicationId() const = 0; // slot 0x0C
    virtual u8 GetSubId() const = 0;               // slot 0x10
    virtual u16 GetChannel() const = 0;            // slot 0x14
    // the 6 bytes of the BSSID
    virtual void GetBssid(u8* pBssid) const = 0;   // slot 0x18
    // the description of the other one
    virtual void Copy(const nn::pia::local::LocalNetworkDescription* pDescription) = 0; // slot 0x1C
};
} // namespace local
} // namespace pia
} // namespace nn
