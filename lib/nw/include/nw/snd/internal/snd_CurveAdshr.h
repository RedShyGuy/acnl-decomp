#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// An attack-decay-sustain-hold-release envelope; the value is in 1/10 dB (the class and function
// names are from the reference symbols, the rest is ours). Attack multiplies the value towards 0 dB,
// hold keeps it, decay and release lower it linearly.
class CurveAdshr
{
public:
    enum Status : u8
    {
        STATUS_ATTACK = 0,
        STATUS_HOLD = 1,
        STATUS_DECAY = 2,
        STATUS_SUSTAIN = 3,
        STATUS_RELEASE = 4,
    };

    // the volume of a new envelope (dB)
    static const f32 VOLUME_INIT;

    CurveAdshr(); // 0x004C5B78 | nintendogs:bytes [tier A]

    // all parameters at their limits, the attack started at initDb
    void Initialize(f32 initDb); // 0x004C58E0 | nintendogs:bytes [tier A]
    // the attack started again at initDb
    void Reset(f32 initDb); // 0x004C59A0 | nintendogs:bytes [tier A]
    // msec milliseconds on
    void Update(int msec); // 0x004C59BC | nintendogs:callseq-callee [tier A]
    // the volume in dB
    f32 GetValue() const; // 0x0073F30C | nintendogs:bytes [tier A]

    // the parameters 0 to 127
    void SetAttack(int attack); // 0x004C5B64 | nintendogs:bytes [tier C]
    void SetHold(int hold); // 0x004C5ACC | nintendogs:bytes [tier A]
    void SetDecay(int decay); // 0x004C5AEC | nintendogs:bytes [tier A]
    void SetSustain(int sustain) { m_Sustain = sustain; }
    void SetRelease(int release); // 0x004C5928 | nintendogs:bytes [tier A]
    // (inline)
    void SetStatus(Status status) { m_Status = status; }
    Status GetStatus() const { return m_Status; }

    Status m_Status;    // 0x00
    f32 m_Value;        // 0x04 1/10 dB
    f32 m_Decay;        // 0x08 1/10 dB per millisecond
    f32 m_Release;      // 0x0C 1/10 dB per millisecond
    f32 m_Attack;       // 0x10 the factor per millisecond
    u16 m_Hold;         // 0x14 milliseconds
    u16 m_HoldCounter;  // 0x16
    u8 m_Sustain;       // 0x18
};
ASSERT_SIZE(CurveAdshr, 0x1C);
} // namespace internal
} // namespace snd
} // namespace nw
