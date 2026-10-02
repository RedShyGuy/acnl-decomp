#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class BackgroundScheduler
{
public:
    BackgroundScheduler(); // TODO: default ctor added so derived stubs compile - may not exist
    void Dispatch(pead::Thread*, int); // 0x00428128 | fefates:bytes [tier B]
    void ResetJob(nn::pia::common::Job*); // 0x00428250 | fefates:bytes [tier B]
    BackgroundScheduler(int, nn::pia::common::CriticalSection*); // 0x0042828C | fefates:bytes [tier B]
    ~BackgroundScheduler(); // 0x00428378 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
