#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13EvOpusHarvestE @ 0x008D2588
// vtable 0x009074A4 (vptr 0x009074AC), offset_to_top 0, 16 entries
// The music of the Harvest Festival; while the cooking music (m_CookBgmId) plays, the event has no music of its own.
// Member names are ours.
class EvOpusHarvest : public ::audio::bgm::EvOpusBase
{
public:
    // m_State
    enum State
    {
        STATE_COOKING = 0,
        STATE_COOKING_RESUMED = 1,
        STATE_IDLE = 2,
    };

    EvOpusHarvest() : m_CookBgmId(SEQ_BGM_EVT_HARVEST_COOK), m_IsCookAccepted(0) {}
    virtual ~EvOpusHarvest();

    virtual void Reset() // 0x00585FB0 slot 0x00
    {
        m_IsPlaying = 0;
        m_State = STATE_IDLE;
        m_IsCookAccepted = 0;
    }
    virtual void Begin(); // 0x00585F6C slot 0x1C
    virtual void End(bool isReplaced); // 0x00585FC8 slot 0x20
    virtual void Resume(); // 0x00585F78 slot 0x28
    virtual bool HasBgm() const // 0x00753D54 slot 0x2C
    {
        return m_State == STATE_IDLE;
    }
    virtual u32 GetBgmId() const // 0x00753D68 slot 0x30
    {
        return STRM_BGM_EVT_HARVEST;
    }

    u8 m_State;          // 0x07 State
    u32 m_CookBgmId;     // 0x08
    u8 m_IsCookAccepted; // 0x0C
    u32 m_CookDelay;     // 0x10
};
ASSERT_SIZE(EvOpusHarvest, 0x14);
} // namespace bgm
} // namespace audio
