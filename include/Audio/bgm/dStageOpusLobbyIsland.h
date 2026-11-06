#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm20StageOpusLobbyIslandE @ 0x008D2870
// vtable 0x009087CC (vptr 0x009087D4), offset_to_top 0, 18 entries
// STRM_BGM_ISL_LOBBY_DAY/NIGHT, silent in the rain (the music of the hour; names are ours)
class StageOpusLobbyIsland : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusLobbyIsland()
    {
        m_Kind = 19;
    }
    virtual ~StageOpusLobbyIsland();
    virtual void OnStart(); // 0x00588E68 slot 0x0C
    virtual void OnStop(); // 0x00589020 slot 0x10
    virtual void Update(); // 0x00588EC0 slot 0x1C
    virtual void Initialize(); // 0x00588DD0 slot 0x20
    virtual void Leave(stage::Name next); // 0x00588E2C slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588DDC slot 0x30
    virtual u32 GetBgmId(bool isNextHour); // 0x005890A8 slot 0x44

    u8 m_IsRaining; // 0x1E
};
ASSERT_SIZE(StageOpusLobbyIsland, 0x20);
} // namespace bgm
} // namespace audio
