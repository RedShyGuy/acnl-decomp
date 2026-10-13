#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_GlobalVariables.h"

namespace nn {
namespace nex {
// 0x0035C030 | fefates:callgraph [tier C]
void nn::nex::SystemError::SignalError(wchar_t* file, unsigned int line, unsigned int error, unsigned int detail)
{
    // (the file and the line are not kept in this build)
    GlobalVariables::s_pInstance->m_LastError.Set(error);
    GlobalVariables::s_pInstance->m_LastErrorDetail.Set(detail);
}

// 0x0035C1C4 (name is ours)
void nn::nex::SystemError::ClearThread(u32 threadId)
{
    GlobalVariables::s_pInstance->m_LastError.ClearThread(threadId);
    GlobalVariables::s_pInstance->m_LastErrorDetail.ClearThread(threadId);
}

} // namespace nex
} // namespace nn
