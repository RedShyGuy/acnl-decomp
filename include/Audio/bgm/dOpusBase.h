#pragma once

#include "decomp.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm8OpusBaseE @ 0x008D28B8
// The base of the music of a stage (StageOpusBase) or of an event (EvOpusBase); slot and member names are ours.
class OpusBase
{
public:
    // (m_IsSwitching is not initialized)
    OpusBase() : m_IsPlaying(0), m_WaitsForChime(0) {}

    // the music is not playing
    virtual void Reset() // 0x0058CC90 slot 0x00
    {
        m_IsPlaying = 0;
    }
    // starts the music (not while the time chime plays, except at midnight of New Year's Day)
    virtual bool Start(); // 0x0058CCA4 slot 0x04
    virtual bool Stop(); // 0x0058CD20 slot 0x08
    virtual void OnStart(); // 0x0058CC9C slot 0x0C
    virtual void OnStop(); // 0x0058CCA0 slot 0x10
    virtual ~OpusBase() {}

    u8 m_IsSwitching;   // 0x4 the music stops to switch to the music of another hour
    u8 m_IsPlaying;     // 0x5
    u8 m_WaitsForChime; // 0x6 Start waits for the end of the time chime
};
ASSERT_SIZE(OpusBase, 0x8);
} // namespace bgm
} // namespace audio
