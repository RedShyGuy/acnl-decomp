#include "nn/nex/nex_StreamBundling.h"

namespace nn {
namespace nex {
// 0x0096C92B (name is ours)
bool g_Unknown0x96C92B;
// 0x0096C92C (name is ours)
bool g_Unknown0x96C92C = true;
// 0x0096C92D (name is ours)
bool g_Unknown0x96C92D = true;
// 0x0096C92E (name is ours)
bool g_Unknown0x96C92E = true;
// 0x00ADDBF8 (name is ours)
BundleStatistics g_BundleStatistics;
// 0x0096C9B4 (name is ours)
s32 g_BundlingHistoryDelay = 182;
// 0x0096C9B8 (name is ours)
s32 g_BundlingRetransmissionDelay = 150;
// 0x0096C9BC (name is ours)
u32 g_MaxBundleSize = 0x554;
// 0x0096C93A (name is ours)
u16 g_MaxBundlingHistoryCount = 32;
// 0x0096C93C (name is ours)
u16 g_MaxBundledHistoryPacketCount = 0x7FFF;
// 0x0096C93E (name is ours)
u16 g_MaxHistoryPacketBundleCount = 2;

// 0x003745A4 slot 0x00
// 0x00374568 (deleting dtor)
nn::nex::StreamBundling::~StreamBundling()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
