#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hid {
namespace CTR {
class TouchPanel;

// Reads the touch panel (member names are ours).
class TouchPanelReader
{
public:
    // inline (in sead::CtrHidDevice::CtrHidDevice)
    TouchPanelReader(TouchPanel& touchPanel) : m_pTouchPanel(&touchPanel), m_LastIndex(-1), m_LastTick(-1) {}
    // the newest state; false without one
    bool ReadLatest(TouchPanelStatus* pStatus); // 0x00353834 | nintendogs:bytes [tier A]

private:
    TouchPanel* m_pTouchPanel; // 0x0
    s32 m_LastIndex;           // 0x4
    s64 m_LastTick;            // 0x8
};
ASSERT_SIZE(TouchPanelReader, 0x10);
} // namespace CTR
} // namespace hid
} // namespace nn
