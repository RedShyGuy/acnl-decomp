#pragma once

#include "decomp.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet26NexProcessHostMigrationJobE @ 0x008CFA2C
// vtable 0x00900650 (vptr 0x00900658), offset_to_top 0, 20 entries
class NexProcessHostMigrationJob : public ::nn::pia::session::ProcessHostMigrationJob
{
public:
    NexProcessHostMigrationJob(); // ctor address unknown
    virtual ~NexProcessHostMigrationJob(); // 0x0040F9FC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040F9EC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F85C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void IsFatalErrorOccur(); // 0x0040D240 slot 0x1C | slot vf_0x1C of nn::pia::session::ProcessHostMigrationJob
    virtual void vf_0x20(); // 0x0040DEF8 slot 0x20 | virtual slot, introduced by nn::pia::session::ProcessHostMigrationJob
    virtual void IsValidHostMigrationSetting(); // 0x0040DE98 slot 0x24 | slot vf_0x24 of nn::pia::session::ProcessHostMigrationJob
    virtual void StartupImpl(bool, nn::pia::StationIndex); // 0x0040D024 slot 0x2C | slot vf_0x2C of nn::pia::session::ProcessHostMigrationJob
    virtual void CleanupImpl(); // 0x0040CFF0 slot 0x30 | slot vf_0x30 of nn::pia::session::ProcessHostMigrationJob
    virtual void CleanupStatus(); // 0x0040D0EC slot 0x34 | slot vf_0x34 of nn::pia::session::ProcessHostMigrationJob
    virtual void CallUpdateSessionHost(unsigned int); // 0x0040DA4C slot 0x38 | fefates:bytes-fuzzy
    virtual void IsCompletedUpdateSessionHost(); // 0x0040E1B4 slot 0x3C | slot vf_0x3C of nn::pia::session::ProcessHostMigrationJob
    virtual void StartupMultiImpl(bool, unsigned short); // 0x0040D138 slot 0x44 | fefates:bytes-fuzzy
    virtual void CheckWhetherReselectNewHost(); // 0x0040DD88 slot 0x48 | slot vf_0x48 of nn::pia::session::ProcessHostMigrationJob
    virtual void CheckWhetherSendMigrationFinish(); // 0x0040EB9C slot 0x4C | slot vf_0x4C of nn::pia::session::ProcessHostMigrationJob
    void InetCleanupOldHostInfo(); // 0x0040DABC | fefates:bytes [tier B]
    void WaitAfterPrepareForBecomingHost(); // 0x0040ED0C | fefates:bytes [tier B]
    void InetCleanupOldHostInfoOnMultiCandidate(); // 0x0040F1D4 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
