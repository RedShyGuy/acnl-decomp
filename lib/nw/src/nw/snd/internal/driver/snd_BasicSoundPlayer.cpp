#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CE4C8 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::BasicSoundPlayer::Initialize()
{
    m_IsActive = false;
    m_IsStarted = false;
    m_IsPause = false;
    m_IsFinished = false;
    m_Unknown0x10 = false;
    m_Volume = 1.0f;
    m_Pitch = 1.0f;
    m_Pan = 0.0f;
    m_SurroundPan = 0.0f;
    m_LpfFreq = 0.0f;
    m_IsFrontBypass = false;
    m_BiquadType = BIQUAD_FILTER_TYPE_NONE;
    m_BiquadValue = 0.0f;
    m_MainSend = 0.0f;
    m_PanMode = PAN_MODE_DUAL;
    m_PanCurve = PAN_CURVE_SQRT;
    m_FxSends[0] = 0.0f;
    m_FxSends[1] = 0.0f;
    m_Unknown0x40 = 0;
}

// 0x004CE530 (name after the other players)
void nw::snd::internal::driver::BasicSoundPlayer::SetBiquadFilter(int type, f32 value)
{
    m_BiquadValue = value;
    m_BiquadType = type;
}

// 0x004CE53C slot 0x0C | fefates:callgraph
void nw::snd::internal::driver::BasicSoundPlayer::Finalize()
{
    m_Event.Signal();
}

// 0x004CE544 (name after GetFxSend)
void nw::snd::internal::driver::BasicSoundPlayer::SetFxSend(AuxBus bus, f32 send)
{
    m_FxSends[bus] = send;
}

// 0x004CE550 | fefates:bytes [tier B]
nw::snd::internal::driver::BasicSoundPlayer::BasicSoundPlayer()
    : m_IsActive(false), m_IsStarted(false), m_IsPause(false), m_IsFinished(false), m_Unknown0x10(false), m_Volume(1.0f), m_Pitch(1.0f), m_Pan(0.0f),
      m_SurroundPan(0.0f), m_LpfFreq(0.0f), m_BiquadValue(0.0f), m_BiquadType(BIQUAD_FILTER_TYPE_NONE), m_IsFrontBypass(false), m_MainSend(0.0f),
      m_PanMode(PAN_MODE_DUAL), m_PanCurve(PAN_CURVE_SQRT)
{
    m_Event.Initialize(true);
    m_FxSends[0] = 0.0f;
    m_FxSends[1] = 0.0f;
    m_Event.Signal();
}

// 0x004CE5FC slot 0x00
// 0x004CE5EC (deleting dtor)
nw::snd::internal::driver::BasicSoundPlayer::~BasicSoundPlayer()
{
    // empty (in the original too)
}

// 0x007414CC | nintendogs:bytes [tier A]
f32 nw::snd::internal::driver::BasicSoundPlayer::GetFxSend(AuxBus bus) const
{
    return m_FxSends[bus];
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
