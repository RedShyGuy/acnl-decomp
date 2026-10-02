#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class JobCallContextCallback
{
public:
    void CompleteJob(const nn::nex::qResult&); // 0x0039C2F8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
