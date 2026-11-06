#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14EvOpusCarnivalE @ 0x008D2600
// vtable 0x009077B4 (vptr 0x009077BC), offset_to_top 0, 16 entries
// The music of the carnival event (STRM_BGM_EVT_CARNIVAL); while another song of the event plays (m_BgmId, kind 21, e.g.
// SEQ_BGM_EVT_CARNIVAL_END_L) the event has no music of its own. Member names are ours.
class EvOpusCarnival : public ::audio::bgm::EvOpusBase
{
public:
    // m_State (1 and 3 are the modes of SoundMgr::SPlayCarnival)
    enum State
    {
        STATE_IDLE = 5,
    };

    EvOpusCarnival() {}
    virtual ~EvOpusCarnival();

    virtual void Reset() // 0x00586888 slot 0x00
    {
        m_IsPlaying = 0;
        m_State = STATE_IDLE;
    }
    virtual void Begin(); // 0x0058687C slot 0x1C
    virtual void End(bool isReplaced); // 0x005868A0 slot 0x20
    virtual void Update(); // 0x0058689C slot 0x24
    virtual bool HasBgm() const // 0x00753DE4 slot 0x2C
    {
        return m_State == STATE_IDLE;
    }
    virtual u32 GetBgmId() const // 0x00753DF8 slot 0x30
    {
        return STRM_BGM_EVT_CARNIVAL;
    }

    u8 m_State;  // 0x07
    u32 m_BgmId; // 0x08 the song of the dance (kind 21)
};
ASSERT_SIZE(EvOpusCarnival, 0xC);
} // namespace bgm
} // namespace audio
