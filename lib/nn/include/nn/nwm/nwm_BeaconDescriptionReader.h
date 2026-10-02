#pragma once

#include "decomp.h"
#include "nn/nwm/nwm_BssDescriptionReaderBase.h"

namespace nn {
namespace nwm {
// slot 0x04 of the vtable is the deleting destructor (0x003E2358)
class BeaconDescriptionReader : public ::nn::nwm::BssDescriptionReaderBase
{
public:
    BeaconDescriptionReader(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~BeaconDescriptionReader(); // 0x003E238C slot 0x00 | fefates:bytes
    virtual void vf_0x08(); // 0x0072ECEC slot 0x08 | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
    virtual void vf_0x0C(); // 0x0072EEB0 slot 0x0C | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
    virtual void vf_0x10(); // 0x0072ED4C slot 0x10 | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
    // the vendor specific element (tag 0xDD) with this OUI and type; points to its tag (name is ours)
    virtual const u8* GetVendorSpecificElement(const u8* oui, u8 type); // 0x0072EDF8 slot 0x14
    BeaconDescriptionReader(const nn::nwm::BssDescription* bss); // 0x003E22F4 | fefates:bytes [tier B]

private:
    u8 m_Unknown08[0xC];    // 0x08
};
ASSERT_SIZE(BeaconDescriptionReader, 0x14);

} // namespace nwm
} // namespace nn
