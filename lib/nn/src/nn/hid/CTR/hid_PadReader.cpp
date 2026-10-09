#include "nn/hid/CTR/hid_PadReader.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/hid/CTR/hid_Devices.h"
#include "nn/hid/CTR/hid_ExtraPad.h"
#include "nn/hidlow/CTR/hidlow_PadLifoRing.h"
#include "nn/hidlow/hidlow_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// a button the readers never report (bit 13; name is ours)
const bit32 BUTTON_HIDDEN = 1 << 13;
} // namespace

// SELECT is reported on its own (otherwise it counts as START; never set in this program; name
// is ours)
// 0x0097E8E0
bool s_IsSelectSeparate;

// 0x003546F0 | nintendogs:bytes [tier A]
bool nn::hid::CTR::PadReader::ReadLatest(PadStatus* pStatus)
{
    int readCount;
    s64 lastTick = -1;
    int lastIndex = -1;
    if (ExtraPad::IsSampling()) {
        return false;
    }
    m_Clamper.ClampValueOfClamp();
    m_pPad->m_pRing->ReadData(pStatus, 1, &readCount, &lastTick, &lastIndex);
    if (readCount <= 0) {
        return false;
    }
    m_Clamper.ClampCore(&pStatus->stickX, &pStatus->stickY, pStatus->stickX, pStatus->stickY);
    if (m_IsFirstRead) {
        m_LastHold = pStatus->hold;
        m_IsFirstRead = false;
    }
    pStatus->hold &= ~BUTTON_HIDDEN;
    pStatus->trigger = (pStatus->hold ^ m_LastHold) & ~m_LastHold;
    pStatus->release = m_LastHold & ~pStatus->hold;
    if (nn::applet::CTR::IsInitialized() && !nn::applet::CTR::detail::IsActive()) {
        pStatus->hold = 0;
        pStatus->release = 0;
        pStatus->trigger = 0;
        pStatus->stickX = 0;
        pStatus->stickY = 0;
    }
    m_LastHold = pStatus->hold;
    if (!s_IsSelectSeparate) {
        nn::hidlow::GatherStartAndSelect(pStatus);
    }
    return true;
}

// 0x00354820 | nintendogs:bytes [tier B]
void nn::hid::CTR::PadReader::NormalizeStickWithScale(f32* pX, f32* pY, s16 x, s16 y)
{
    m_Clamper.NormalizeStickWithScale(pX, pY, x, y);
}

// 0x00353B80 (name is ours)
void nn::hid::CTR::PadReader::SetNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
    m_Clamper.SetNormalizeStickScaleSettings(scale, threshold);
}

// 0x00354838 | nintendogs:bytes [tier A]
nn::hid::CTR::PadReader::PadReader(Pad& pad) : m_pPad(&pad), m_LastIndex(-1)
{
    m_IsFirstRead = true;
    m_LastTick = -1;
}

} // namespace CTR
} // namespace hid
} // namespace nn
