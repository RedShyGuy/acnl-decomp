#pragma once

#include "decomp.h"

namespace ut {
// The frame counter of an animation: a frame range, a speed and a play mode (once, loop, ping-pong, forwards or
// backwards). Only the class name is from the binary (RTTI); member and function names are ours. The binary has no
// vtable of its own for the class (it is only built as the base of lyt::AnmObject, whose vtable shares the slots
// that have code: Reset and the setters); the slots 0x0C-0x18 are only known from the derived class.
// RTTI N2ut15FrameControllerE @ 0x008D0C30
class FrameController
{
public:
    // the values of m_PlayMode (names are ours)
    enum PlayMode
    {
        PLAY_MODE_ONCE,          // from the start to the end frame, then it ends
        PLAY_MODE_ONCE_REVERSE,  // from the end to the start frame, then it ends
        PLAY_MODE_LOOP,
        PLAY_MODE_LOOP_REVERSE,
        PLAY_MODE_PING_PONG,     // forwards and backwards in turn
        PLAY_MODE_NUM
    };
    // the values of m_State (names are ours)
    enum State
    {
        STATE_STOPPED,
        STATE_PLAYING,
        STATE_STOPPING,  // stopped this frame (the frame is still applied once; then STATE_STOPPED)
        STATE_3          // also becomes STATE_STOPPED (not set in the binary)
    };

    // (inline)
    FrameController()
        : m_EndFrame(1.0f), m_StartFrame(0.0f), m_Frame(0.0f), m_Speed(1.0f), m_State(STATE_STOPPED), m_PlayMode(PLAY_MODE_ONCE),
          m_IsEnd(false), m_IsReverse(false), m_IsStopRequested(false)
    {
    }
    virtual ~FrameController() {}

    // the frame goes back to where the play mode starts
    virtual void Reset(); // 0x004DB7C0 slot 0x08
    // the frame is applied; true: the animation was applied
    virtual bool Calc() = 0; // slot 0x0C
    // the animation is played again from the start
    virtual void Restart() = 0; // slot 0x10
    virtual void Play() = 0; // slot 0x14
    virtual void Stop() = 0; // slot 0x18
    virtual void SetEndFrame(f32 frame); // 0x004DB600 slot 0x1C
    virtual void SetStartFrame(f32 frame); // 0x004DB608 slot 0x20
    virtual void SetSpeed(f32 speed); // 0x004DB610 slot 0x24
    virtual void SetPlayMode(s32 mode); // 0x004DB5F8 slot 0x28
    virtual void SetFrame(f32 frame); // 0x004DB618 slot 0x2C

    // the frame advances by the speed (playing only); always false
    bool Update(); // 0x004DB620

    f32 m_EndFrame;          // 0x04
    f32 m_StartFrame;        // 0x08
    f32 m_Frame;             // 0x0C
    f32 m_Speed;             // 0x10
    s32 m_State;             // 0x14 State
    s32 m_PlayMode;          // 0x18 PlayMode
    bool m_IsEnd;            // 0x1C the end of a one-time animation was reached
    bool m_IsReverse;        // 0x1D ping-pong: on the way back
    bool m_IsStopRequested;  // 0x1E (not set in the binary)
};
ASSERT_SIZE(FrameController, 0x20);

// the length of a message text in characters: tags count with their parameters (0x0E group index size params,
// 0x0F group index) (name is ours; the copy lies behind FrameController and is only called by lyt::Object)
u32 GetTextLength(const wchar_t* text); // 0x004DB7E0
} // namespace ut
