#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm24StageOpusVillageDowntownE @ 0x008D28AC
// vtable 0x00908960 (vptr 0x00908968), offset_to_top 0, 18 entries
// The music of the town and of Main Street: the music of the hour by the weather (STRM_BGM_OUTDOOR*), on Main Street
// STRM_BGM_DWNTWN_DAY/QUIET, or the music of the current event (EventOpusList). Member names are ours.
class StageOpusVillageDowntown : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusVillageDowntown() : m_IsEventChanged(0) {}
    virtual ~StageOpusVillageDowntown();
    virtual void OnStart(); // 0x00589A28 slot 0x0C
    virtual void OnStop(); // 0x00589CD8 slot 0x10
    virtual void Update(); // 0x00589AD8 slot 0x1C
    virtual void EnterKeep(); // 0x005899F4 slot 0x28
    virtual void Leave(stage::Name next); // 0x00589514 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00589310 slot 0x30
    virtual void SetupKeep(stage::Name stage); // 0x0058999C slot 0x34
    virtual void Ready(); // 0x005894EC slot 0x38
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753FF8 slot 0x40
    {
        if (IsGrowUp()) {
            return false;
        }
        if (!isOnline) {
            return true;
        }
        if (GetStageOpusMgr()->m_Flags & StageOpusMgr::FLAG_CEREMONY) {
            return false;
        }
        if (stage::GetCurrent() == next) {
            return true;
        }
        if (next == stage::NAME_DOWNTOWN) {
            return m_pEvent != NULL;
        }
        if (next == stage::NAME_VILLAGE) {
            return m_pEvent != NULL;
        }
        return false;
    }
    virtual u32 GetBgmId(bool isNextHour); // 0x00589DA8 slot 0x44

    // the ambience of Main Street (SE_ENV_DOWNTOWN_1, _2 when Kicks is built); stops it when it plays
    static void UpdateAmbience(); // 0x005897A0
    // the mixes 46 and 47 by the position of the player on Main Street (the side of the ocean decides the direction)
    DECOMP_NOINLINE static void UpdateSideMixes(); // 0x0058983C
    // the mix 45 when the player is below the level of the town (the town only)
    static void UpdateLowMix(); // 0x00589594

    static const u32 s_DowntownBgm[3][2];
    static const u32 s_OutdoorBgm[3][24];

    EvOpusBase* m_pEvent; // 0x20
    u32 m_Value;          // 0x24 (set to 7, not read)
    u8 _28;               // 0x28
    u8 m_IsEventChanged;  // 0x29 (StageOpusMgr::SetTownChanged)
};
ASSERT_SIZE(StageOpusVillageDowntown, 0x2C);
} // namespace bgm
} // namespace audio
