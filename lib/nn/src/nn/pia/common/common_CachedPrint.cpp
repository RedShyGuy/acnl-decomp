#include "nn/pia/common/common_CachedPrint.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadPrintConfig.h"

namespace nn {
namespace pia {
namespace common {
// 0x00975A34
CachedPrint* CachedPrint::s_pInstance;

// 0x004266E0 | fefates:bytes [tier B]
void nn::pia::common::CachedPrint::DestroyInstance()
{
    if (s_pInstance == nullptr) {
        return;
    }
    char* pBuffer = s_pInstance->m_pBuffer;
    if (pBuffer != nullptr) {
        pead::GetMemoryBlockInfo(pBuffer);
        pead::FreeMemory(pBuffer);
    }
    delete s_pInstance;
    s_pInstance = nullptr;
}

// 0x00426734 (name is ours)
void nn::pia::common::CachedPrint::Clear()
{
    m_CriticalSection.Lock();
    m_ReadOffset = 0;
    m_WrapOffset = 0;
    m_WriteOffset = 0;
    m_CriticalSection.Unlock();
}

// 0x00426764 (name is ours)
void nn::pia::common::CachedPrint::Flush()
{
    m_CriticalSection.Lock();
    if (m_WriteOffset != m_ReadOffset) {
        pead::PrintConfig::Print(m_pBuffer + m_ReadOffset, m_WriteOffset - m_ReadOffset);
    }
    if (m_WriteOffset != m_WrapOffset && m_WrapOffset != 0) {
        pead::PrintConfig::Print(m_pBuffer, m_WrapOffset);
    }
    Clear();
    m_CriticalSection.Unlock();
}

} // namespace common
} // namespace pia
} // namespace nn
