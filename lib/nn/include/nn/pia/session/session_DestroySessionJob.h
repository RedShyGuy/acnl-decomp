#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session17DestroySessionJobE @ 0x008CFFF0
// vtable 0x0090191C (vptr 0x00901924), offset_to_top 0, 10 entries
//
// The host destroys the session (Session::LeaveSessionAsync without host migration): it waits
// for the joint session job, destroys the mesh, then the network removes the session (slots
// 0x20 / 0x24) and MeshCleanup ends it. The step names are from the strings; the layout is
// from the constructor, the member and slot names are ours.
class DestroySessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    DestroySessionJob(); // 0x00438EC8
    virtual ~DestroySessionJob(); // 0x00438F34 slot 0x00
    // 0x00438F0C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338FC slot 0x14
    virtual void Cleanup(); // 0x00438D3C slot 0x18
    // whether the network can destroy the session now
    virtual nn::Result vf_0x1C() = 0; // slot 0x1C
    // the work of the network after the mesh (its next step)
    virtual void vf_0x20() = 0; // slot 0x20
    // the last step of the network
    virtual void vf_0x24() = 0; // slot 0x24

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x00438D8C

    // the steps
    common::ExecuteResult WaitForcedTerminatingOfJointSessionJob(); // 0x00438C6C
    common::ExecuteResult DestroyMesh(); // 0x004389D4
    common::ExecuteResult WaitDestroyMesh(); // 0x00438BD4
    common::ExecuteResult MeshCleanup(); // 0x00438B18

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of Mesh::DestroyMesh
    nn::Result m_Result;                 // 0x58
    common::Time m_StartTime;            // 0x60 (the monitoring data)
    bool m_IsJointSessionJobRunning;     // 0x68, it was when the job started
};
ASSERT_OFFSET(DestroySessionJob, m_Result, 0x58);
ASSERT_OFFSET(DestroySessionJob, m_StartTime, 0x60);
ASSERT_SIZE(DestroySessionJob, 0x70);
} // namespace session
} // namespace pia
} // namespace nn
