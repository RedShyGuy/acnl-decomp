#include "nn/nex/nex_LogDeviceConsole.h"

namespace nn {
namespace nex {
// 0x00382210 (name is ours)
void nn::nex::LogDeviceConsole::Output(const wchar_t* pLine)
{
    // nothing to do in this build
}

// 0x0038222C
// 0x00382214 (deleting dtor)
nn::nex::LogDeviceConsole::~LogDeviceConsole()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
