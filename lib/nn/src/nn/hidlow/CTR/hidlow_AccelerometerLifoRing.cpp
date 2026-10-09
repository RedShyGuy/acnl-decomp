#include "nn/hidlow/CTR/hidlow_AccelerometerLifoRing.h"
#include "nn/hidlow/CTR/hidlow_LifoRing.h"

namespace nn {
namespace hidlow {
namespace CTR {
// 0x004848A4 | nintendogs:bytes [tier B]
void nn::hidlow::CTR::AccelerometerLifoRing::ReadData(nn::hid::CTR::AccelerometerStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex)
{
    detail::ReadLifoRing<nn::hid::CTR::AccelerometerStatus, ENTRY_NUM>(m_Tick, m_PrevTick, m_Index, m_Entries, pBuffer, count, pReadCount,
                                                 pLastTick, pLastIndex);
}

} // namespace CTR
} // namespace hidlow
} // namespace nn
