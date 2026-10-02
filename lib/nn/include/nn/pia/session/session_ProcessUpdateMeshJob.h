#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session20ProcessUpdateMeshJobE @ 0x008D0074
// vtable 0x00901A34 (vptr 0x00901A3C), offset_to_top 0, 6 entries
class ProcessUpdateMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ProcessUpdateMeshJob(); // ctor candidate(s) 0x0043DE40 (unverified)
    virtual ~ProcessUpdateMeshJob(); // 0x0043E14C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043E13C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007339E8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void CalcTimeLimit(bool); // 0x0043B8E0 | fefates:bytes-fuzzy [tier B]
    void ClearStationIndex(nn::pia::StationIndex); // 0x0043BAA8 | fefates:bytes [tier B]
    void SetStationDataList(const unsigned char*, unsigned int); // 0x0043BF90 | fefates:bytes [tier B]
    void UpdateDataTakeover(unsigned int); // 0x0043C2F8 | fefates:bytes [tier B]
    void UpdateStationDataList(const unsigned char*, unsigned int); // 0x0043D490 | fefates:bytes-fuzzy [tier B]
    void SetConnectionFailureNotice(nn::pia::StationIndex, unsigned char); // 0x0043D9CC | fefates:bytes [tier B]
    void Startup(const unsigned char*, unsigned int, bool); // 0x0043DC68 | fefates:bytes-fuzzy [tier B]
    void CheckEdmByStationIndex(nn::pia::StationIndex) const; // 0x00733930 | fefates:bytes [tier B]
    void GetStationIndexByPrincipalID(unsigned int) const; // 0x00733990 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
