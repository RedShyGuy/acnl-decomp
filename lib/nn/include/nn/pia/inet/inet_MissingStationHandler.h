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
    // (inline in NexNetworkFactory::CreateMissingStationHandler)
    MissingStationHandler() {}
    virtual ~MissingStationHandler(); // 0x004015B4 slot 0x00
    // 0x004015B0 slot 0x04 (deleting dtor)
    virtual void Execute(nn::pia::transport::StationConnectionInfo*); // 0x00401570 slot 0x08 | fefates:bytes
};
} // namespace inet
} // namespace pia
} // namespace nn
