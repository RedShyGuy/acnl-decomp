#include "nn/hidlow/CTR/hidlow_PadLifoRing.h"
#include "nn/hidlow/CTR/hidlow_LifoRing.h"

namespace nn {
namespace hidlow {
namespace CTR {
// 0x00484414 | nintendogs:bytes [tier A]
void nn::hidlow::CTR::PadLifoRing::ReadData(nn::hid::CTR::PadStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex)
{
    detail::ReadLifoRing<nn::hid::CTR::PadStatus, ENTRY_NUM>(m_Tick, m_PrevTick, m_Index, m_Entries, pBuffer, count, pReadCount,
                                                 pLastTick, pLastIndex);
}

} // namespace CTR
} // namespace hidlow
} // namespace nn
