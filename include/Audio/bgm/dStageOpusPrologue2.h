#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPrologue2E @ 0x008D27E0
// vtable 0x009083F4 (vptr 0x009083FC), offset_to_top 0, 18 entries
// STRM_BGM_PROLOGUE_07/08 in the town at the start of the game (names are ours)
class StageOpusPrologue2 : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPrologue2() {}
    virtual ~StageOpusPrologue2();
    virtual void OnStart(); // 0x00588548 slot 0x0C
    virtual void OnStop(); // 0x00588678 slot 0x10
    virtual void Update(); // 0x00588580 slot 0x1C
    virtual void Initialize(); // 0x005883B0 slot 0x20
    virtual void EnterKeep(); // 0x00588534 slot 0x28
    virtual void Leave(stage::Name next); // 0x005884C8 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058846C slot 0x30
    virtual void SetupKeep(stage::Name stage); // 0x0058979C slot 0x34
    virtual void Ready(); // 0x005884A8 slot 0x38
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753F48 slot 0x40
    {
        return !isOnline || next == stage::NAME_VILLAGE || next == stage::NAME_DOWNTOWN;
    }

    u8 m_IsNamedArrival; // 0x1E m_IsArrival and the name of the player and of the town are set (mix 21)
    u8 m_IsArrival;    // 0x1F (Func5C58C0() == 1)
    u8 m_StartsLater;  // 0x20 the music starts when the player is ready
};
ASSERT_SIZE(StageOpusPrologue2, 0x24);
} // namespace bgm
} // namespace audio
