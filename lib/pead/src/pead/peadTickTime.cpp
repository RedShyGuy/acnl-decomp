#include "pead/peadTickTime.h"
#include "nn/svc/svc_Api.h"

namespace pead {
// 0x0053D960 | nintendogs:bytes [tier B]
TickTime::TickTime()
{
    mTick = nn::svc::GetSystemTick();
}
} // namespace pead
