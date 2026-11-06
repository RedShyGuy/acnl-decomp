#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPrologue4E @ 0x008D27F8
// vtable 0x00908494 (vptr 0x0090849C), offset_to_top 0, 18 entries
// STRM_BGM_PROLOGUE_09/11 in the town at the start of the game, with the fanfare STRM_BGM_PROLOGUE_10 (names are ours)
class StageOpusPrologue4 : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPrologue4() : m_HasFanfare(0), m_IsTown(0), m_KeepsMix21(0), m_IsHouseChosen(0) {}
    virtual ~StageOpusPrologue4();
    virtual void OnStart(); // 0x005889E4 slot 0x0C
    virtual void OnStop(); // 0x00588B14 slot 0x10
    virtual void Update(); // 0x00588A1C slot 0x1C
    virtual void Initialize(); // 0x005887E0 slot 0x20
    virtual void EnterKeep(); // 0x00588934 slot 0x28
    virtual void Leave(stage::Name next); // 0x0058889C slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588860 slot 0x30
    virtual void SetupKeep(stage::Name stage); // 0x005888E4 slot 0x34
    virtual void Ready(); // 0x0058887C slot 0x38
    virtual void ReadyKeep(); // 0x005888E8 slot 0x3C
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753F68 slot 0x40
    {
        return !isOnline || next == stage::NAME_VILLAGE || next == stage::NAME_DOWNTOWN;
    }

    u8 m_HasFanfare;     // 0x1E STRM_BGM_PROLOGUE_10 was requested (StageOpusMgr::RequestPrologue10)
    u8 m_IsFanfareHeard; // 0x1F the jingle of the fanfare is attached
    u8 m_IsTown;         // 0x20
    u8 m_KeepsMix21;     // 0x21
    u8 m_IsHouseChosen;  // 0x22 the player has chosen the place of the house
};
ASSERT_SIZE(StageOpusPrologue4, 0x24);
} // namespace bgm
} // namespace audio
