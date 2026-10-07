#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/session/session_ModifyAttributeJob.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
}
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet26NexMatchModifyAttributeJobE @ 0x008CFA28
// vtable 0x00900628 (vptr 0x00900630), offset_to_top 0, 8 entries
//
// Changes an attribute of the matchmake session through NexMatchmakeSession. The step names are from the strings; the member name is ours.
class NexMatchModifyAttributeJob : public ::nn::pia::session::ModifyAttributeJob
{
public:
    NexMatchModifyAttributeJob(); // 0x0040CFC0
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchModifyAttributeJob(); // 0x00439CB8 slot 0x00
    // 0x0040CFE0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F858 slot 0x14
    virtual void Cleanup(); // 0x0040CFA8 slot 0x18
    virtual nn::Result vf_0x1C(); // 0x0040CC50 slot 0x1C

    // the steps
    common::ExecuteResult ModifyAttribute(); // 0x0040CCC0
    common::ExecuteResult WaitModifyAttribute(); // 0x0040CDF8

    session::CommonMatchmakeSession* m_pSession; // 0x64, the current matchmake session of Session
};
ASSERT_OFFSET(NexMatchModifyAttributeJob, m_pSession, 0x64);
ASSERT_SIZE(NexMatchModifyAttributeJob, 0x68);
} // namespace inet
} // namespace pia
} // namespace nn
