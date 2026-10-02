#include "nn/nwm/nwm_BssDescriptionReaderBase.h"
#include "nn/nwm/nwm_BeaconDescriptionReader.h"

namespace nn {
namespace nwm {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nwm::BeaconDescriptionReader::BeaconDescriptionReader()
{
}

// 0x003E238C slot 0x00 | fefates:bytes
nn::nwm::BeaconDescriptionReader::~BeaconDescriptionReader()
{
}

// 0x0072ECEC slot 0x08 | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
void nn::nwm::BeaconDescriptionReader::vf_0x08()
{
}

// 0x0072EEB0 slot 0x0C | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
void nn::nwm::BeaconDescriptionReader::vf_0x0C()
{
}

// 0x0072ED4C slot 0x10 | virtual slot, introduced by nn::nwm::BeaconDescriptionReader
void nn::nwm::BeaconDescriptionReader::vf_0x10()
{
}

// 0x0072EDF8 slot 0x14
const u8* nn::nwm::BeaconDescriptionReader::GetVendorSpecificElement(const u8* oui, u8 type)
{
}

// 0x003E22F4 | fefates:bytes [tier B]
nn::nwm::BeaconDescriptionReader::BeaconDescriptionReader(const nn::nwm::BssDescription* bss)
{
}

} // namespace nwm
} // namespace nn
