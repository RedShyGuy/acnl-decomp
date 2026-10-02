#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_MissingStationHandler.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet21MissingStationHandlerE @ 0x008CF978
// vtable 0x009003CC (vptr 0x009003D4), offset_to_top 0, 3 entries
class MissingStationHandler : public ::nn::pia::transport::MissingStationHandler
{
public:
    MissingStationHandler(); // ctor address unknown
    virtual void vf_0x00(); // 0x004015B4 slot 0x00 | virtual slot, introduced by nn::pia::inet::MissingStationHandler
    virtual void vf_0x04(); // 0x004015B0 slot 0x04 | virtual slot, introduced by nn::pia::inet::MissingStationHandler
    virtual void Execute(nn::pia::transport::StationConnectionInfo*); // 0x00401570 slot 0x08 | fefates:bytes
};
} // namespace inet
} // namespace pia
} // namespace nn
