#include "nn/hidlow/CTR/hidlow_GyroscopeLowLifoRing.h"
#include "nn/hidlow/CTR/hidlow_LifoRing.h"

namespace nn {
namespace hidlow {
namespace CTR {
// 0x00484730 | nintendogs:bytes [tier B]
void nn::hidlow::CTR::GyroscopeLowLifoRing::ReadData(nn::hid::CTR::GyroscopeLowStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex)
{
    detail::ReadLifoRing<nn::hid::CTR::GyroscopeLowStatus, ENTRY_NUM>(m_Tick, m_PrevTick, m_Index, m_Entries, pBuffer, count, pReadCount,
                                                 pLastTick, pLastIndex);
}

} // namespace CTR
} // namespace hidlow
} // namespace nn
