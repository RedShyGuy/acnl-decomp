#pragma once

#include "decomp.h"
#include "nn/nwm/CTR/nwm_BeaconReader.h"
#include "nn/nwm/nwm_BssDescriptionReaderBase.h"

namespace nn {
namespace nwm {
// Reads the beacon of a BssDescription. Slot 0x04 of the vtable is the deleting destructor
// (0x003E2358); the slots 0x08-0x14 forward to the BeaconReader (in the original they fall
// through into the BssReader functions; their names are ours).
class BeaconDescriptionReader : public ::nn::nwm::BssDescriptionReaderBase
{
public:
    BeaconDescriptionReader(const nn::nwm::BssDescription* bss); // 0x003E22F4 | fefates:bytes [tier B]
    virtual ~BeaconDescriptionReader(); // 0x003E238C slot 0x00 | fefates:bytes
    virtual u8 GetIeNum() const; // 0x0072ECEC slot 0x08 (name is ours)
    virtual Ssid GetSsid() const; // 0x0072EEB0 slot 0x0C (name is ours)
    virtual const u8* GetVendorIe(const u8* oui) const; // 0x0072ED4C slot 0x10 (name is ours)
    // the vendor specific element (tag 0xDD) with this OUI and type; points to its tag (name is ours)
    virtual const u8* GetVendorSpecificElement(const u8* oui, u8 type) const; // 0x0072EDF8 slot 0x14

private:
    nn::nwm::CTR::BeaconReader m_Reader; // 0x08
};
ASSERT_SIZE(BeaconDescriptionReader, 0x14);
} // namespace nwm
} // namespace nn
