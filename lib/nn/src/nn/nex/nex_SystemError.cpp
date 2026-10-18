#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_ErrorDescriptionTable.h"
#include "nn/nex/nex_GlobalVariables.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Platform/Core/SystemError.cpp";
} // namespace

// 0x0035C030 | fefates:callgraph [tier C]
void nn::nex::SystemError::SignalError(wchar_t* file, unsigned int line, unsigned int error, unsigned int detail)
{
    // (the file and the line are not kept in this build)
    GlobalVariables::s_pInstance->m_LastError.Set(error);
    GlobalVariables::s_pInstance->m_LastErrorDetail.Set(detail);
}

// 0x0035C128 (name is ours)
void nn::nex::SystemError::GetErrorDescription(unsigned int error, wchar_t* pBuffer, unsigned int size)
{
    if (error == 0) {
        wcsncpy(pBuffer, L"No Error", size);
        pBuffer[size - 1] = L'\0';
        return;
    }
    u32 module = (error >> 16) & 0xFFF;
    if (module == MODULE_FACADE) {
        if (error == ERROR_FACADE_NOT_INITIALIZED) {
            swprintf(pBuffer, size, L"The facade is not initialized");
        }
        return;
    }
    wcsncpy(pBuffer, ErrorDescriptionTable::s_pTables[module]->m_ppDescriptions[error & 0xFFFF], size);
    pBuffer[size - 1] = L'\0';
}

// 0x0035C1C4 (name is ours)
void nn::nex::SystemError::ClearThread(u32 threadId)
{
    GlobalVariables::s_pInstance->m_LastError.ClearThread(threadId);
    GlobalVariables::s_pInstance->m_LastErrorDetail.ClearThread(threadId);
}

// 0x0035C308 (name is ours)
u32 nn::nex::SystemError::GetLastError()
{
    return GlobalVariables::s_pInstance->m_LastError.GetRef();
}

// 0x0035C328 (name is ours)
void nn::nex::SystemError::ClearLastError()
{
    SignalError(const_cast<wchar_t*>(FILE_NAME), 235, 0, 0);
}

} // namespace nex
} // namespace nn
