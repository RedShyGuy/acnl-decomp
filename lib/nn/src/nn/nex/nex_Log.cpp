#include "nn/nex/nex_Log.h"
#include "nn/nex/nex_LogDeviceConsole.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x003CCD08 (name is ours)
void nn::nex::Log::Print(const wchar_t* pFormat, va_list args, u8 level)
{
    if (m_pDevice == nullptr) {
        return;
    }
    if (m_pFilter != nullptr && !m_pFilter->IsEnabled()) {
        return;
    }
    if (m_pFormat == nullptr) {
        return;
    }
    wchar_t buffer[256];
    memset(buffer, 0, sizeof(buffer));
    m_pFormat->vf_0x08(buffer, 254);
    m_pFormat->vf_0x0C(buffer, 254);
    AppendLevel(buffer, 254, level);
    m_pFormat->vf_0x10(buffer, 254);
    m_pFormat->vf_0x14(buffer, 254);
    m_pFormat->vf_0x18(buffer, 254);
    m_pFormat->vf_0x1C(buffer, 255, pFormat, args);
    m_pFormat->vf_0x20(buffer, 254);
    m_pDevice->Output(buffer);
}

// 0x003CCE50
void nn::nex::Log::vf_0x0C(const wchar_t* pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    Print(pFormat, args, 0);
    va_end(args);
}

// 0x003CCE70 (name is ours)
void nn::nex::Log::AppendLevel(wchar_t* pBuffer, u32 size, u8 level)
{
    // nothing to do in this build
}

// 0x003CCE74
void nn::nex::Log::vf_0x08(const wchar_t* pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    Print(pFormat, args, 0);
    va_end(args);
}

// 0x003CCE94 | fefates:bytes [tier B]
nn::nex::Log::Log()
{
    m_pDevice = new LogDeviceConsole(__stdout);
    m_pFormat = new OutputFormat();
    m_pFilter = nullptr;
}

// 0x003CCFD4 | mk7dlp:bytes
// 0x003CCF54 (deleting dtor)
nn::nex::Log::~Log()
{
    delete m_pFormat;
    delete m_pDevice;
    delete m_pFilter;
}

} // namespace nex
} // namespace nn
