#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local33LocalLeaveWithHostMigrationJobNewE @ 0x008CFDB4
// vtable 0x00901440 (vptr 0x00901448), offset_to_top 0, 8 entries
class LocalLeaveWithHostMigrationJobNew : public ::nn::pia::session::LeaveWithHostMigrationJob
{
public:
    LocalLeaveWithHostMigrationJobNew(); // ctor candidate(s) 0x00425628 (unverified)
    virtual ~LocalLeaveWithHostMigrationJobNew(); // 0x00425650 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00425640 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007317F8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0042554C slot 0x18 | virtual slot, introduced by nn::pia::session::LeaveWithHostMigrationJob
};
} // namespace local
} // namespace pia
} // namespace nn
