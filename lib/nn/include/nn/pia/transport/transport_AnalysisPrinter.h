#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
// The output of the packet analysis: one formatted line at a time to the callback of the
// application. The members and SetCallback are ours.
class AnalysisPrinter
{
public:
    typedef void (*Callback)(const char* str);

    static void Write(const char* format, ...); // 0x0044FE84 | fefates:bytes [tier B]
    // the callback (not in the binary: armlink removed it as unused; the name is ours)
    static void SetCallback(Callback callback);

    static const u32 BUFFER_SIZE = 512;
    static char s_Buffer[BUFFER_SIZE];
    static Callback s_Callback;
};
} // namespace transport
} // namespace pia
} // namespace nn
