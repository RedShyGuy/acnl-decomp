#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
class TalkSystem;
class RenderBase;

// The timing of the characters of a talk: the characters written since the last step and the time left (names are
// ours). TalkSeq and TalkRuby advance by the waiting time per character (fast: the second one; the override if it is
// not negative) times the speed.
struct TalkTimer
{
    // (inline) true: the next character is due (else the time goes on)
    bool Step(bool isFast)
    {
        f32 wait = override;
        if (wait < 0.0f) {
            wait = isFast ? waitFast : waitNormal;
        }
        f32 step = speed * wait;
        f32 rest = time - static_cast<f32>(count);
        if (rest >= 0.0f) {
            return true;
        }
        time = rest + step;
        count = 0;
        return false;
    }

    s32 count;       // 0x00 the characters written since the last step
    f32 time;        // 0x04
    f32 speed;       // 0x08 (1.0)
    f32 waitNormal;  // 0x0C (by region)
    f32 waitFast;    // 0x10 (by region)
    f32 override;    // 0x14 (negative: none)
};
ASSERT_SIZE(TalkTimer, 0x18);

// Writes the ruby (or the main text under a ruby) of a talk in step with TalkSeq: a part of the text up to m_pStop
// into a renderer; the ruby text plays the speech sounds. Member names are ours.
// RTTI N6script8TalkRubyE @ 0x008D3DFC
// vtable 0x0090B6F4 (vptr 0x0090B6FC), offset_to_top 0, 8 entries
class TalkRuby : public ::script::MsgEngine
{
public:
    static const u8 MODE_TEXT = 0;  // the main text under a ruby
    static const u8 MODE_RUBY = 1;  // the ruby (with the speech sounds)

    // (inline)
    TalkRuby(TalkSystem* pOwner, RenderBase* pRender)
        : m_pOwner(pOwner), m_pRender(pRender), m_pStop(NULL), m_Mode(2), m_IsDone(1), m_IsFast(0), m_CharNum(0)
    {
        m_Timer.count = 0;
        m_Timer.time = 0.0f;
        m_Timer.speed = 1.0f;
        m_Timer.waitNormal = 0.0f;
        m_Timer.waitFast = 0.0f;
        m_Timer.override = -1.0f;
    }
    // (inline) no part (TalkSeq::Reset)
    void Reset()
    {
        ResetText();
        m_pStop = NULL;
        m_Mode = 2;
        m_IsDone = 1;
        m_IsFast = 0;
        m_CharNum = 0;
        m_Timer.count = 0;
        m_Timer.time = 0.0f;
    }

    virtual ~TalkRuby(); // 0x005FE290 slot 0x00
    // 0x005FE280 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x005FE204 slot 0x08
    virtual void End(); // 0x005FE1F4 slot 0x0C
    virtual bool IsContinue(); // 0x005FE208 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005FE150 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005FE200 slot 0x18

    TalkSystem* m_pOwner;    // 0x60
    RenderBase* m_pRender;   // 0x64
    const char16* m_pStop;   // 0x68 the end of the part
    u8 m_Mode;               // 0x6C
    u8 m_IsDone;             // 0x6D the part is written
    s8 m_IsFast;             // 0x6E
    s32 m_CharNum;           // 0x70 the characters written (0: the timer is taken from TalkSeq)
    TalkTimer m_Timer;       // 0x74
};
ASSERT_SIZE(TalkRuby, 0x8C);
} // namespace script
