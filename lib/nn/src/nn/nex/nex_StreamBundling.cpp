#include "nn/nex/nex_StreamBundling.h"

namespace nn {
namespace nex {
// 0x0096C92B (name is ours)
bool g_Unknown0x96C92B;
// 0x0096C92C (name is ours)
bool g_Unknown0x96C92C;
// 0x0096C92D (name is ours)
bool g_Unknown0x96C92D;

// 0x003745A4 slot 0x00
// 0x00374568 (deleting dtor)
nn::nex::StreamBundling::~StreamBundling()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
