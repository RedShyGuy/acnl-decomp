#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_HandleObject.h"

namespace nn {
namespace os {
// RTTI N2nn2os10WaitObjectE @ 0x008CDDD0
class WaitObject : public ::nn::os::HandleObject
{
public:
    WaitObject() {}
    // waits for one (waitAll false: *index = which one) or all of the objects
    static nn::Result WaitMultiple(s32* index, nn::os::WaitObject** objects, s32 count, bool waitAll, s64 timeout); // 0x0034BF00 | nintendogs:callseq [tier A]
};
} // namespace os
} // namespace nn
