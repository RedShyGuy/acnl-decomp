#include "ssys/co/dFaderBase.h"

namespace ssys {
namespace co {
// 0x00566EB8 slot 0x18 (name is ours)
bool ssys::co::FaderBase::Calc()
{
    bool done = false;
    switch (m_Status) {
    case STATUS_FADE_IN:
        if (m_FrameCount++ > m_Duration) {
            m_Status = STATUS_CLEAR;
            m_Flags |= FLAG_FADE_IN_DONE;
            done = m_Flags & FLAG_FADE_IN_DONE;
        }
        break;
    case STATUS_FADE_OUT:
        if (++m_FrameCount > m_Duration + 1) {
            m_Status = STATUS_COVERED;
            m_Flags |= FLAG_FADE_OUT_DONE;
            done = (m_Flags & FLAG_FADE_OUT_DONE) >> 1;
        }
        break;
    }
    return done;
}

// 0x00566F4C slot 0x10 (name is ours)
bool ssys::co::FaderBase::FadeIn()
{
    if (m_Status != STATUS_COVERED) {
        return false;
    }
    m_Status = STATUS_FADE_IN;
    m_FrameCount = 0;
    return true;
}

// 0x00566F80 slot 0x14 (name is ours)
bool ssys::co::FaderBase::FadeOut()
{
    if (m_Status != STATUS_CLEAR) {
        return false;
    }
    m_Status = STATUS_FADE_OUT;
    m_FrameCount = 0;
    return true;
}

// 0x00566FB0 slot 0x08 (name is ours)
void ssys::co::FaderBase::SetStatus(s32 status)
{
    if (status == STATUS_COVERED || status == STATUS_CLEAR) {
        m_Status = status;
    }
}

// 0x00566FC0 (name is ours)
ssys::co::FaderBase::FaderBase(u8 arg, s32 status) : _4(arg), m_Flags(FLAG_FADE_OUT_DONE), m_Duration(20), m_FrameCount(0)
{
    if (status == STATUS_COVERED || status == STATUS_CLEAR) {
        m_Status = status;
    }
}

// 0x0056700C slot 0x00
// 0x00567004 (deleting dtor)
ssys::co::FaderBase::~FaderBase()
{
    // empty (in the original too)
}

} // namespace co
} // namespace ssys
