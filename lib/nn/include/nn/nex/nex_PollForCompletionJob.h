#pragma once

#include "decomp.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class PollForCompletionJob
{
public:
    void CompleteJob(nn::nex::qResult); // 0x00397A04 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
