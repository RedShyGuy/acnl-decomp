#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet29InetLeaveWithHostMigrationJobE @ 0x008CFA50
// vtable 0x00900700 (vptr 0x00900708), offset_to_top 0, 8 entries
class InetLeaveWithHostMigrationJob : public ::nn::pia::session::LeaveWithHostMigrationJob
{
public:
    InetLeaveWithHostMigrationJob(); // 0x00411128
    virtual ~InetLeaveWithHostMigrationJob(); // 0x004435D8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00411140 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072FA60 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual nn::pia::StationIndex DecideNextHost(); // 0x004110C0 slot 0x18 | slot DecideNextHost of nn::pia::session::LeaveWithHostMigrationJob
};
} // namespace inet
} // namespace pia
} // namespace nn
