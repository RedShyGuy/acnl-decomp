#pragma once

#include "decomp.h"
#include "nn/nex/nex_LogDevice.h"
#include "nn/nex/nex_OutputFormat.h"
#include "nn/nex/nex_RootObject.h"
#include <cstdarg>

namespace nn {
namespace nex {
// Decides whether a Log writes (no class of the binary sets one; the name is ours).
class LogFilter
{
public:
    virtual ~LogFilter();
    virtual bool IsEnabled();
};

// A log: an OutputFormat makes the line, a LogDevice writes it. The names of the members and of
// the functions other than the constructor are ours.
// RTTI N2nn3nex3LogE @ 0x008CF470
// vtable 0x008FF8A4 (vptr 0x008FF8AC), offset_to_top 0, 5 entries
class Log : public ::nn::nex::RootObject
{
public:
    Log(); // 0x003CCE94 | fefates:bytes [tier B]
    virtual ~Log(); // 0x003CCFD4 slot 0x00 | mk7dlp:bytes
    // 0x003CCF54 slot 0x04 (deleting dtor)
    virtual void vf_0x08(const wchar_t* pFormat, ...); // 0x003CCE74 slot 0x08
    virtual void vf_0x0C(const wchar_t* pFormat, ...); // 0x003CCE50 slot 0x0C
    // appends the name of the level
    virtual void AppendLevel(wchar_t* pBuffer, u32 size, u8 level); // 0x003CCE70 slot 0x10

    void Print(const wchar_t* pFormat, va_list args, u8 level); // 0x003CCD08

    LogDevice* m_pDevice;     // 0x04
    OutputFormat* m_pFormat;  // 0x08
    LogFilter* m_pFilter;     // 0x0C
};
ASSERT_SIZE(Log, 0x10);
} // namespace nex
} // namespace nn
