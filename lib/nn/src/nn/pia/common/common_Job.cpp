#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Job.h"

namespace nn {
namespace pia {
namespace common {
// 0x00428BA8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::common::Job::~Job()
{
}

// 0x00428934 slot 0x08 | fefates:bytes-fuzzy
void nn::pia::common::Job::Reset(bool)
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::pia::common::Job::ExecuteCore()
{
}

// 0x004288C4 | fefates:bytes [tier B]
void nn::pia::common::Job::Ready(bool)
{
}

// 0x004289A4 | fefates:bytes [tier B]
void nn::pia::common::Job::Resume(bool)
{
}

// 0x00428A14 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::Job::Execute(bool)
{
}

// 0x00428B74 | fefates:bytes [tier B]
nn::pia::common::Job::Job()
{
}

// 0x007331F0 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::Job::IsForeground() const
{
}

} // namespace common
} // namespace pia
} // namespace nn
