#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class JobStartNATSession
{
public:
    void StepFinish(); // 0x0038B990 | fefates:bytes [tier B]
    void CompleteJob(const nn::nex::qResult&); // 0x0038B9E4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
