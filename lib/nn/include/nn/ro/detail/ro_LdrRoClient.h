#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace ro {
namespace detail {
// The session of the ldr:ro service (name is ours; the rest of nn::ro uses it, too)
extern nn::Handle s_Session;

// The commands of the ldr:ro service (3dbrew "LDR:RO"; the class name is ours). process is the
// handle of the own process.
class LdrRoClient
{
public:
    // crs: the static module (CRS) of the program, mapped at mappedAddress
    static nn::Result Initialize(nn::Handle process, uptr crs, size_t size, uptr mappedAddress); // 0x0013098C (name after 3dbrew)
    static nn::Result LoadCRR(nn::Handle process, uptr crr, size_t size); // 0x00124238 (name after 3dbrew)
    static nn::Result UnloadCRR(nn::Handle process, uptr crr); // 0x0013B300 (name after 3dbrew)
    // LoadCRO_New; *fixedSize gets the size of the module after fixing
    static nn::Result LoadCRO(size_t* fixedSize, nn::Handle process, uptr cro, uptr mappedAddress, size_t croSize, uptr data, u32 zero,
                              size_t dataSize, uptr bss, size_t bssSize, bool isAutoLink, u32 fixLevel, uptr crr); // 0x0034CED4 (name after 3dbrew)
    static nn::Result UnloadCRO(nn::Handle process, uptr mappedAddress, u32 zero, uptr cro); // 0x0013E8B4 (name after 3dbrew)
    static nn::Result Shutdown(nn::Handle process, uptr crsMappedAddress); // 0x0013B344 (name after 3dbrew)
};
} // namespace detail
} // namespace ro
} // namespace nn
