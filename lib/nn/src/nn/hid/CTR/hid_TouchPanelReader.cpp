#include "nn/hid/CTR/hid_TouchPanelReader.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/hid/CTR/hid_Devices.h"
#include "nn/hidlow/CTR/hidlow_TouchPanelLifoRing.h"

namespace nn {
namespace hid {
namespace CTR {
// 0x00353834 | nintendogs:bytes [tier A]
bool nn::hid::CTR::TouchPanelReader::ReadLatest(TouchPanelStatus* pStatus)
{
    int readCount;
    s64 lastTick = -1;
    int lastIndex = -1;
    m_pTouchPanel->m_pRing->ReadData(pStatus, 1, &readCount, &lastTick, &lastIndex);
    if (nn::applet::CTR::IsInitialized() && !nn::applet::CTR::detail::IsActive()) {
        pStatus->x = 0;
        pStatus->y = 0;
        pStatus->touch = 0;
    }
    return readCount > 0;
}

} // namespace CTR
} // namespace hid
} // namespace nn
