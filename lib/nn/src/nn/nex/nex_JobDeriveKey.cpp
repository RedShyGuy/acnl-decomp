#include "nn/nex/nex_JobDeriveKey.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_KeyCache.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// 0x0035D13C slot 0x00
// 0x0035D0C8 (deleting dtor)
nn::nex::JobDeriveKey::~JobDeriveKey()
{
    delete m_pAlgorithm;
}

// 0x0035CE64 slot 0x0C | fefates:callseq
void nn::nex::JobDeriveKey::Execute()
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    if (m_Iteration == 0 && m_pKeyCache != nullptr) {
        Key key;
        if (m_pKeyCache->RetrieveKey(m_Iterations, m_Password, &key)) {
            CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallId);
            if (pContext != nullptr) {
                *m_pKey = key;
                pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
            }
            SetState(STATE_COMPLETED);
            return;
        }
    }
    Buffer checksum(m_pAlgorithm->GetChecksumLength());
    while (m_Iteration < m_Iterations) {
        checksum.m_ContentSize = 0;
        checksum.m_HeaderSize = 0;
        m_pAlgorithm->ComputeChecksum(m_Data, &checksum);
        m_Data = checksum;
        m_Iteration++;
        if (Time::ConvertDeadlineToTimeoutInline(m_DispatchDeadline) == 0) {
            break;
        }
        if (m_Iteration % ITERATIONS_PER_RUN == 0) {
            break;
        }
    }
    if (m_Iteration == m_Iterations) {
        Key key(m_Data.m_pData + m_Data.m_HeaderSize, m_Data.m_ContentSize);
        CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallId);
        if (pContext != nullptr) {
            *m_pKey = key;
            pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
        }
        SetState(STATE_COMPLETED);
        if (m_pKeyCache != nullptr) {
            m_pKeyCache->AddKey(m_Iterations, m_Password, key);
        }
    } else {
        // the next run as soon as possible
        SetState(STATE_WAITING);
        m_WaitTime = 0;
    }
}

} // namespace nex
} // namespace nn
