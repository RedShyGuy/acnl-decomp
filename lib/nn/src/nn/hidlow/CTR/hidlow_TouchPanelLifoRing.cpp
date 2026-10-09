#include "nn/hidlow/CTR/hidlow_TouchPanelLifoRing.h"
#include "nn/hidlow/CTR/hidlow_LifoRing.h"

namespace nn {
namespace hidlow {
namespace CTR {
// 0x004845BC | nintendogs:bytes [tier A]
void nn::hidlow::CTR::TouchPanelLifoRing::ReadData(nn::hid::CTR::TouchPanelStatus* pBuffer, int count, int* pReadCount, s64* pLastTick, int* pLastIndex)
{
    detail::ReadLifoRing<nn::hid::CTR::TouchPanelStatus, ENTRY_NUM>(m_Tick, m_PrevTick, m_Index, m_Entries, pBuffer, count, pReadCount,
                                                 pLastTick, pLastIndex);
}

} // namespace CTR
} // namespace hidlow
} // namespace nn
