#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The key of the HPP server of DataStore: derived from the password and the principal ID (Kerberos),
// cached for the last principal ID. The names of the members are ours.
// RTTI N2nn3nex24JobRetrieveGameAuthTokenE @ 0x008CEDD0
// vtable 0x008FE82C (vptr 0x008FE834), offset_to_top 0, 13 entries
class JobRetrieveGameAuthToken : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // (inline in RetrieveGameAuthTokenClient::RetrieveGameAuthToken)
    JobRetrieveGameAuthToken() : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_PrincipalId(0), m_pKey(nullptr) {}
    virtual ~JobRetrieveGameAuthToken(); // 0x003B47DC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B47B8 slot 0x04 | fefates:callseq (deleting dtor)
    nn::nex::qResult Initialize(unsigned int callContextId, unsigned int principalId, const nn::nex::String& password, nn::nex::String* pToken,
                                nn::nex::String* pKey); // 0x003B4458 | fefates:bytes [tier B]
    void CompleteJob(const nn::nex::qResult& result); // 0x003B45B8 | fefates:bytes [tier B]
    void StepDeriveKey(); // 0x003B465C | fefates:bytes [tier B]
    void StepDummyToComplete(); // 0x003B473C | fefates:bytes [tier B]

    u32 m_CallContextId; // 0x68
    u32 m_PrincipalId;   // 0x6C
    String m_Password;   // 0x70
    String* m_pKey;      // 0x78 (of the caller)
};
ASSERT_OFFSET(JobRetrieveGameAuthToken, m_CallContextId, 0x68);
ASSERT_SIZE(JobRetrieveGameAuthToken, 0x80);
} // namespace nex
} // namespace nn
