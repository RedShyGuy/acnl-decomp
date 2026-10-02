#pragma once

#include "decomp.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local31LocalProcessHostMigrationJobNewE @ 0x008CFDA8
// vtable 0x009013E8 (vptr 0x009013F0), offset_to_top 0, 20 entries
class LocalProcessHostMigrationJobNew : public ::nn::pia::session::ProcessHostMigrationJob
{
public:
    LocalProcessHostMigrationJobNew(); // ctor candidate(s) 0x00425518 (unverified)
    virtual ~LocalProcessHostMigrationJobNew(); // 0x00442AEC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0042553C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007317F4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void IsValidHostMigrationSetting(); // 0x004250A0 slot 0x24 | slot vf_0x24 of nn::pia::session::ProcessHostMigrationJob
    virtual void StartupImpl(bool, nn::pia::StationIndex); // 0x004249DC slot 0x2C | slot vf_0x2C of nn::pia::session::ProcessHostMigrationJob
    virtual void CleanupStatus(); // 0x00424AB4 slot 0x34 | slot vf_0x34 of nn::pia::session::ProcessHostMigrationJob
};
} // namespace local
} // namespace pia
} // namespace nn
