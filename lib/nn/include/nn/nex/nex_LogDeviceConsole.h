#pragma once

#include "decomp.h"
#include "nn/nex/nex_LogDevice.h"

// the stdout stream of the C library (the name of its symbol in the ARM C library)
extern "C" u8 __stdout[]; // 0x00AF872C

namespace nn {
namespace nex {
// Writes the lines to a stream of the C library (nothing in this build). The member name is ours.
// RTTI N2nn3nex16LogDeviceConsoleE @ 0x008CE5A8
// vtable 0x008FD168 (vptr 0x008FD170), offset_to_top 0, 3 entries
class LogDeviceConsole : public ::nn::nex::LogDevice
{
public:
    explicit LogDeviceConsole(void* pStream) : m_pStream(pStream) {} // (inline)
    virtual ~LogDeviceConsole(); // 0x0038222C slot 0x00
    // 0x00382214 slot 0x04 (deleting dtor)
    virtual void Output(const wchar_t* pLine); // 0x00382210 slot 0x08 (name is ours)

    void* m_pStream; // 0x04
};
ASSERT_SIZE(LogDeviceConsole, 0x8);
} // namespace nex
} // namespace nn
