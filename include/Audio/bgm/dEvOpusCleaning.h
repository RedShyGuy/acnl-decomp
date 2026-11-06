#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14EvOpusCleaningE @ 0x008D260C
// vtable 0x009077FC (vptr 0x00907804), offset_to_top 0, 16 entries
// A cleaning event with the music STRM_BGM_IND_FLORISTS. It is not selected by a date but by a state of the game (Func2932E4);
// which event of the game that is (Weeding Day or Leif in a town with many weeds) is still open. Member names are ours.
class EvOpusCleaning : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusCleaning() : m_IsActive(0), m_IsStageStopped(0) {}
    virtual ~EvOpusCleaning();

    virtual void Reset() // 0x005868F8 slot 0x00
    {
        m_IsPlaying = 0;
        m_IsStageStopped = 0;
        m_IsActive = 0;
    }
    virtual void Begin(); // 0x005868DC slot 0x1C
    virtual void End(bool isReplaced); // 0x005869BC slot 0x20
    virtual void Update(); // 0x00586994 slot 0x24
    virtual bool HasBgm() const // 0x00753E0C slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753E14 slot 0x30
    {
        return STRM_BGM_IND_FLORISTS;
    }
    virtual bool IsChanged() // 0x00753E04 slot 0x34
    {
        return false;
    }
    virtual bool StartRequest(u8 kind, u32 id); // 0x005868F0 slot 0x38
    virtual bool StopRequest(u8 kind, u32 id); // 0x0058690C slot 0x3C

    u8 m_IsActive;       // 0x7
    u8 m_IsStageStopped; // 0x8 the music of the stage was stopped
};
ASSERT_SIZE(EvOpusCleaning, 0xC);
} // namespace bgm
} // namespace audio
