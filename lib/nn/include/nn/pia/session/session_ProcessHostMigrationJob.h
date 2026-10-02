#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session23ProcessHostMigrationJobE @ 0x008D00D4
// vtable 0x00901B90 (vptr 0x00901B98), offset_to_top 0, 20 entries
class ProcessHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ProcessHostMigrationJob(); // ctor candidate(s) 0x00442A04 (unverified)
    virtual ~ProcessHostMigrationJob(); // 0x00442AF0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00442ADC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734154 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void CleanupOldHostInfoCommonProc(); // 0x00442668 slot 0x18 | slot vf_0x18 of nn::pia::session::ProcessHostMigrationJob
    virtual void IsFatalErrorOccur(); // 0x00441430 slot 0x1C | slot vf_0x1C of nn::pia::session::ProcessHostMigrationJob
    virtual void vf_0x20(); // 0x00442664 slot 0x20 | virtual slot, introduced by nn::pia::session::ProcessHostMigrationJob
    virtual void IsValidHostMigrationSetting(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void UpdateHostStationIndexByLocalStationIndex(); // 0x004427F0 slot 0x28 | slot vf_0x28 of nn::pia::session::ProcessHostMigrationJob
    virtual void StartupImpl(bool, nn::pia::StationIndex); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void CleanupImpl(); // 0x00441030 slot 0x30 | slot vf_0x30 of nn::pia::session::ProcessHostMigrationJob
    virtual void CleanupStatus(); // 0x00441320 slot 0x34 | slot vf_0x34 of nn::pia::session::ProcessHostMigrationJob
    virtual void CallUpdateSessionHost(unsigned int); // 0x0044202C slot 0x38 | slot vf_0x38 of nn::pia::session::ProcessHostMigrationJob
    virtual void IsCompletedUpdateSessionHost(); // 0x004426B8 slot 0x3C | slot vf_0x3C of nn::pia::session::ProcessHostMigrationJob
    virtual void vf_0x40(); // 0x004413C0 slot 0x40 | fefates:callseq
    virtual void StartupMultiImpl(bool, unsigned short); // 0x004413B8 slot 0x44 | slot vf_0x44 of nn::pia::session::ProcessHostMigrationJob
    virtual void CheckWhetherReselectNewHost(); // 0x00442638 slot 0x48 | slot vf_0x48 of nn::pia::session::ProcessHostMigrationJob
    virtual void CheckWhetherSendMigrationFinish(); // 0x004426C0 slot 0x4C | slot vf_0x4C of nn::pia::session::ProcessHostMigrationJob
    void WaitUpdateSessionHost(); // 0x00442034 | fefates:bytes [tier B]
    void MakeHostCandidateRanking(nn::pia::StationIndex, unsigned char*, unsigned int*, unsigned int*, bool); // 0x004421DC | fefates:bytes-fuzzy [tier B]
    void PrepareForBecomingHostCommonProc(); // 0x004426C8 | fefates:bytes-fuzzy [tier B]
    void Cleanup(); // 0x0044281C | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
