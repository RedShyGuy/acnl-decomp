#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local33LocalLeaveWithHostMigrationJobNewE @ 0x008CFDB4
// vtable 0x00901440 (vptr 0x00901448), offset_to_top 0, 8 entries
//
// The leaving host of the local network hands the network over to the next candidate of the
// LocalMigrationManager.
class LocalLeaveWithHostMigrationJobNew : public ::nn::pia::session::LeaveWithHostMigrationJob
{
public:
    LocalLeaveWithHostMigrationJobNew(); // 0x00425628
    virtual ~LocalLeaveWithHostMigrationJobNew(); // 0x00425650 slot 0x00
    // 0x00425640 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007317F8 slot 0x14
    virtual nn::pia::StationIndex DecideNextHost(); // 0x0042554C slot 0x18
};
} // namespace local
} // namespace pia
} // namespace nn
