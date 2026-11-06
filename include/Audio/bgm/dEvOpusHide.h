#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm10EvOpusHideE @ 0x008D2570
// vtable 0x00907414 (vptr 0x0090741C), offset_to_top 0, 16 entries
// The music of hide-and-seek (STRM_BGM_ISL_GAME_KAKURENBO); member names are ours.
class EvOpusHide : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusHide() : m_IsStarted(0), m_IsWaiting(0), m_IsResumed(0), _B(0) {}
    virtual ~EvOpusHide();

    virtual void Reset() // 0x00584A08 slot 0x00
    {
        m_IsPlaying = 0;
        m_IsStarted = 0;
        m_IsWaiting = 0;
        m_IsResumed = 0;
        _B = 0;
    }
    virtual void Begin(); // 0x00584978 slot 0x1C
    virtual void End(bool isReplaced); // 0x00584B48 slot 0x20
    virtual void Update(); // 0x00584AF8 slot 0x24
    virtual bool HasBgm() const // 0x00753D18 slot 0x2C
    {
        return !m_IsStarted || !m_IsWaiting;
    }
    virtual u32 GetBgmId() const // 0x00753D34 slot 0x30
    {
        return STRM_BGM_ISL_GAME_KAKURENBO;
    }
    virtual bool StartRequest(u8 kind, u32 id); // 0x00584998 slot 0x38
    virtual bool StopRequest(u8 kind, u32 id); // 0x00584A24 slot 0x3C

    u8 m_IsStarted;     // 0x7
    u8 m_IsWaiting;     // 0x8
    u8 m_IsResumed;     // 0x9
    u8 m_StopsAtOnce;   // 0xA the music of the stage stops without a fade
    u8 _B;              // 0xB
};
ASSERT_SIZE(EvOpusHide, 0xC);
} // namespace bgm
} // namespace audio
