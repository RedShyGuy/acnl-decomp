#include "nn/pia/common/common_BackgroundScheduler.h"

namespace nn {
namespace pia {
namespace common {
// TODO: default ctor added so derived stubs compile - may not exist
nn::pia::common::BackgroundScheduler::BackgroundScheduler()
{
}

// 0x00428128 | fefates:bytes [tier B]
void nn::pia::common::BackgroundScheduler::Dispatch(pead::Thread*, int)
{
}

// 0x00428250 | fefates:bytes [tier B]
void nn::pia::common::BackgroundScheduler::ResetJob(nn::pia::common::Job*)
{
}

// 0x0042828C | fefates:bytes [tier B]
nn::pia::common::BackgroundScheduler::BackgroundScheduler(int, nn::pia::common::CriticalSection*)
{
}

// 0x00428378 | fefates:bytes [tier B]
nn::pia::common::BackgroundScheduler::~BackgroundScheduler()
{
}

} // namespace common
} // namespace pia
} // namespace nn
