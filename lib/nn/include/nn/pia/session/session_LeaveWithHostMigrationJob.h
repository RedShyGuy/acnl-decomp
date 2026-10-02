#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session25LeaveWithHostMigrationJobE @ 0x008D0104
// vtable 0x00901C50 (vptr 0x00901C58), offset_to_top 0, 8 entries
class LeaveWithHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~LeaveWithHostMigrationJob(); // 0x004435DC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004435C8 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734174 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void CleanupStatus(); // 0x00442DF4 slot 0x1C | fefates:bytes
    void ReceiveMigrationResponse(nn::pia::StationIndex); // 0x00442EE8 | fefates:bytes [tier B]
    void Cleanup(); // 0x00443420 | fefates:bytes [tier B]
    LeaveWithHostMigrationJob(); // 0x00443570 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
