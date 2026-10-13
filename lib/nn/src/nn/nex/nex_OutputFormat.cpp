#include "nn/nex/nex_OutputFormat.h"
#include <cwchar>

namespace nn {
namespace nex {
// 0x0035D5CC
u32 nn::nex::OutputFormat::vf_0x1C(wchar_t* pBuffer, u32 size, const wchar_t* pFormat, va_list args)
{
    return 0;
}

// 0x0035D5D4
void nn::nex::OutputFormat::vf_0x10(wchar_t* pBuffer, u32 size)
{
    // nothing to do in this build
}

// 0x0035D5D8
void nn::nex::OutputFormat::vf_0x14(wchar_t* pBuffer, u32 size)
{
    // nothing to do in this build
}

// 0x0035D5DC
void nn::nex::OutputFormat::vf_0x08(wchar_t* pBuffer, u32 size)
{
    pBuffer[0] = L'\0';
}

// 0x0035D5E8
void nn::nex::OutputFormat::vf_0x0C(wchar_t* pBuffer, u32 size)
{
    u32 length = wcslen(pBuffer);
    if (size - length == 0) {
        return;
    }
    swprintf(pBuffer + length, size - length, L"%ls", L"(");
}

// 0x0035D628
void nn::nex::OutputFormat::vf_0x18(wchar_t* pBuffer, u32 size)
{
    // nothing to do in this build
}

// 0x0035D62C
void nn::nex::OutputFormat::vf_0x20(wchar_t* pBuffer, u32 size)
{
    // nothing to do in this build
}

// 0x0035D648
// 0x0035D630 (deleting dtor)
nn::nex::OutputFormat::~OutputFormat()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
