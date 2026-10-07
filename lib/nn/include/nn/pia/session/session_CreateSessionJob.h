#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
class CreateSessionSetting;

// RTTI N2nn3pia7session16CreateSessionJobE @ 0x008CFFCC
// vtable 0x009018C4 (vptr 0x009018CC), offset_to_top 0, 10 entries
//
// Creates a session (Session::CreateSessionAsync): the network creates it on its side (slot
// 0x1C and its steps), then MeshStartup starts the mesh with the matchmake session and
// CreateMesh creates it. The step names are from the strings; the layout is from the
// constructor, the member and slot names are ours.
class CreateSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    CreateSessionJob(); // 0x00438248
    virtual ~CreateSessionJob(); // 0x004382A4 slot 0x00
    // 0x0043827C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338F4 slot 0x14
    virtual void Cleanup(); // 0x00438148 slot 0x18
    // the network takes the setting and sets its first step
    virtual nn::Result vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting) = 0; // slot 0x1C
    // the mesh is created: the next step of the network
    virtual void vf_0x20() = 0; // slot 0x20
    // the creation failed with the result
    virtual nn::pia::common::ExecuteResult vf_0x24(nn::Result result) = 0; // slot 0x24

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::CreateSessionSetting* pSetting); // 0x00438198

    // the steps
    common::ExecuteResult CreateMesh(); // 0x00437CF4
    common::ExecuteResult MeshStartup(); // 0x00437DB0
    common::ExecuteResult WaitCreateMesh(); // 0x00437ED4
    common::ExecuteResult CompleteFailure(); // 0x00438008
    common::ExecuteResult CompleteProcess(); // 0x004380D0

    // the state of Session (for the derived classes; names are ours)
    void SetSessionState(u8 state); // 0x0043811C
    void SetSessionDisconnectState(u8 state); // 0x00438130

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of Mesh::CreateMesh
    nn::Result m_Result;                 // 0x58
};
ASSERT_OFFSET(CreateSessionJob, m_Result, 0x58);
ASSERT_SIZE(CreateSessionJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
