#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport21MissingStationHandlerE @ 0x008D0230
class StationConnectionInfo;

// Handles a station of the station data list that is gone before it was connected (session::
// ProcessUpdateMeshJob). An interface (no vtable of its own in the binary); the slot name is from
// the fefates symbols of inet::MissingStationHandler.
class MissingStationHandler : public ::nn::pia::common::RootObject
{
public:
    MissingStationHandler() {} // (inline)
    virtual ~MissingStationHandler() {}
    virtual void Execute(nn::pia::transport::StationConnectionInfo* pInfo) = 0;
};
} // namespace transport
} // namespace pia
} // namespace nn
