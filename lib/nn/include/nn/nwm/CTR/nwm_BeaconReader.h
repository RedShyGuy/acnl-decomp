#pragma once

#include "decomp.h"
#include "nn/nwm/CTR/nwm_BssReader.h"

namespace nn {
namespace nwm {
namespace CTR {
// RTTI N2nn3nwm3CTR12BeaconReaderE @ 0x008CF7E0
// vtable 0x008FFE14 (vptr 0x008FFE1C), offset_to_top 0, 4 entries
class BeaconReader : public ::nn::nwm::CTR::BssReader
{
public:
    BeaconReader(); // ctor address unknown
    virtual void vf_0x00(); // 0x003E23BC slot 0x00 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
    virtual void vf_0x04(); // 0x003E23B8 slot 0x04 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
    virtual void vf_0x08(); // 0x0072ECCC slot 0x08 | virtual slot, introduced by nn::nwm::CTR::BeaconReader
    virtual void vf_0x0C(); // 0x0072ECDC slot 0x0C | virtual slot, introduced by nn::nwm::CTR::BeaconReader
};
} // namespace CTR
} // namespace nwm
} // namespace nn
