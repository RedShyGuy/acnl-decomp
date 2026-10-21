#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x00394768 | fefates:bytes [tier B]
void nn::nex::ProtocolCallContext::SetCredentials(nn::nex::Credentials* pCredentials)
{
    if (m_pCredentials != nullptr) {
        m_pCredentials->ReleaseRef();
        m_pCredentials = nullptr;
    }
    if (pCredentials != nullptr) {
        m_pCredentials = pCredentials;
        pCredentials->AcquireRef();
    }
}

// 0x003947F4 | fefates:bytes [tier B]
void nn::nex::ProtocolCallContext::BeginTransition(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled)
{
    if (state == STATE_AVAILABLE) {
        m_ReturnValues.clear();
        return;
    }
    if (state == STATE_FAILURE && isSignaled) {
        if (m_pIsSuccess != nullptr) {
            *m_pIsSuccess = false;
            m_pIsSuccess = nullptr;
        }
        if (m_pResult != nullptr) {
            *m_pResult = result;
            m_pResult = nullptr;
        }
    }
}

// 0x00394888 | fefates:bytes [tier B]
void nn::nex::ProtocolCallContext::AddReturnValuePtr(void* pValue)
{
    m_ReturnValues.push_back(pValue);
}

// 0x00394A1C | fefates:callgraph [tier C]
void* nn::nex::ProtocolCallContext::GetReturnValuePtr(unsigned int index)
{
    return m_ReturnValues[index];
}

// 0x00394A28 | mk7dlp:callseq-callee [tier A]
nn::nex::ProtocolCallContext::ProtocolCallContext(nn::nex::Credentials* pCredentials)
    : m_pIsSuccess(nullptr), m_pResult(nullptr), m_pCredentials(nullptr)
{
    if (pCredentials != nullptr) {
        m_pCredentials = pCredentials;
        pCredentials->AcquireRef();
    }
}

// 0x00394A98 | fefates:callgraph [tier C]
// 0x00394A88 (deleting dtor)
nn::nex::ProtocolCallContext::~ProtocolCallContext()
{
    if (m_State == STATE_CALL_IN_PROGRESS) {
        ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
        if (m_State == STATE_CALL_IN_PROGRESS) {
            Cancel(STATE_CANCELLED);
        }
    }
    if (m_pCredentials != nullptr) {
        m_pCredentials->ReleaseRef();
        m_pCredentials = nullptr;
    }
}

} // namespace nex
} // namespace nn
