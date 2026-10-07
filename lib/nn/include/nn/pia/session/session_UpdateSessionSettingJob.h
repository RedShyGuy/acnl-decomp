#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session23UpdateSessionSettingJobE @ 0x008D00EC
// vtable 0x00901C00 (vptr 0x00901C08), offset_to_top 0, 8 entries
//
// The network (inet) changes the setting of the matchmake session in its steps.
class CreateSessionSetting;
class UpdateSessionSettingJob : public ::nn::pia::common::StepSequenceJob
{
public:
    UpdateSessionSettingJob(); // 0x00442B9C
    virtual ~UpdateSessionSettingJob(); // 0x00442BEC slot 0x00
    // 0x00442BC4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073416C slot 0x14
    virtual void Cleanup(); // 0x00442B50 slot 0x18
    // the network takes the new setting (and sets its first step)
    virtual nn::Result vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting) = 0; // slot 0x1C

    // the last step (armlink placed it in front of Cleanup, after SignatureSettingStorage)
    common::ExecuteResult CompleteProcess(); // 0x00442B08

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of the network
    u32 m_SessionId;                     // 0x58
};
ASSERT_OFFSET(UpdateSessionSettingJob, m_SessionId, 0x58);
ASSERT_SIZE(UpdateSessionSettingJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
