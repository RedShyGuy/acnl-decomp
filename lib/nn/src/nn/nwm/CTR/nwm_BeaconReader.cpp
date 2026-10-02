#include "nn/nwm/CTR/nwm_BssReader.h"
#include "nn/nwm/CTR/nwm_BeaconReader.h"

namespace nn {
namespace nwm {
namespace CTR {
// ctor address unknown
nn::nwm::CTR::BeaconReader::BeaconReader()
{
}

// 0x003E23BC slot 0x00 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
void nn::nwm::CTR::BeaconReader::vf_0x00()
{
}

// 0x003E23B8 slot 0x04 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
void nn::nwm::CTR::BeaconReader::vf_0x04()
{
}

// 0x0072ECCC slot 0x08 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
void nn::nwm::CTR::BeaconReader::vf_0x08()
{
}

// 0x0072ECDC slot 0x0C | virtual slot, introduced by nn::nwm::CTR::BeaconReader
void nn::nwm::CTR::BeaconReader::vf_0x0C()
{
}

} // namespace CTR
} // namespace nwm
} // namespace nn
