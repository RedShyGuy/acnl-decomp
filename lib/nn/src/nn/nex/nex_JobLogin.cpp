#include "nn/nex/nex_JobLogin.h"
#include "nn/nex/nex_NgsFacade.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
// 0x0096CE3C (name is ours)
AnyObjectHolder<Data, String> g_LoginCustomData;

// 0x003D5104 slot 0x00 | fefates:bytes
// 0x003D50F4 (deleting dtor)
nn::nex::JobLogin::~JobLogin()
{
    g_LoginCustomData.Reset(nullptr);
}

// 0x003D5064 | mk7dlp:callseq [tier A]
void nn::nex::JobLogin::Complete(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    m_pBackEndServices->m_LoginResult = result;
    if (pContext != nullptr) {
        if (result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    }
    if (m_State != STATE_COMPLETED) {
        SetToComplete();
    }
}

} // namespace nex
} // namespace nn
