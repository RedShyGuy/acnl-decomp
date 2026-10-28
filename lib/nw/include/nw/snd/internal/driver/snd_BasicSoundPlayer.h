#pragma once

#include "decomp.h"
#include "nn/os/os_LightEvent.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The base of the players in the sound thread: the state and the parameters the sounds send with
// commands (the class and function names are from the reference symbols, the members are ours).
// The event is signaled when the player is finalized.
class BasicSoundPlayer
{
public:
    // the biquad filter type of a new player (none)
    static const u8 BIQUAD_FILTER_TYPE_NONE = 255; // (name is ours)

    BasicSoundPlayer(); // 0x004CE550 | fefates:bytes [tier B]
    virtual ~BasicSoundPlayer(); // 0x004CE5FC slot 0x00

    virtual void Initialize(); // 0x004CE4C8 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004CE53C slot 0x0C | fefates:callgraph
    virtual void Start() = 0; // slot 0x10
    virtual void Stop() = 0; // slot 0x14
    virtual void Pause(bool isPause) = 0; // slot 0x18

    void SetBiquadFilter(int type, f32 value); // 0x004CE530 (name after the other players)
    void SetFxSend(AuxBus bus, f32 send); // 0x004CE544 (name after GetFxSend)
    f32 GetFxSend(AuxBus bus) const; // 0x007414CC | nintendogs:bytes [tier A]

    nn::os::LightEvent m_Event; // 0x04
    bool m_IsActive;            // 0x0C
    bool m_IsStarted;           // 0x0D
    bool m_IsPause;             // 0x0E
    bool m_IsFinished;          // 0x0F
    bool m_Unknown0x10;         // 0x10
    f32 m_Volume;               // 0x14
    f32 m_Pitch;                // 0x18
    f32 m_Pan;                  // 0x1C
    f32 m_SurroundPan;          // 0x20
    f32 m_LpfFreq;              // 0x24
    f32 m_BiquadValue;          // 0x28
    u8 m_BiquadType;            // 0x2C
    u8 padding0x2D;             // 0x2D
    bool m_IsFrontBypass;       // 0x2E
    f32 m_MainSend;             // 0x30
    PanMode m_PanMode;          // 0x34
    PanCurve m_PanCurve;        // 0x35
    f32 m_FxSends[2];           // 0x38
    u32 m_Unknown0x40;          // 0x40 (only Initialize sets it)
};
ASSERT_SIZE(BasicSoundPlayer, 0x44);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
