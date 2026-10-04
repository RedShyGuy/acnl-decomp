#include "nn/pia/common/common_Time.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097E3D8
const Time Time::INFINITE_TIME(-1);
// 0x0097E3E0
const Time Time::ZERO_TIME(0);

// 0x00428DC4 | fefates:bytes [tier B]
void nn::pia::common::Time::SetNow()
{
    m_Tick = nn::svc::GetSystemTick();
}

} // namespace common
} // namespace pia
} // namespace nn
