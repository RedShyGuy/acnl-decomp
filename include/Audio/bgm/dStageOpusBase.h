#pragma once

#include "decomp.h"
#include "Audio/bgm/dOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusBaseE @ 0x008D25AC
// vtable 0x0090757C (vptr 0x00907584), offset_to_top 0, 18 entries
// The music of a stage (StageOpusHolder creates one per stage through its factory table). Slot and member names are ours.
class StageOpusBase : public ::audio::bgm::OpusBase
{
public:
    // m_State
    enum State
    {
        STATE_PLAYING = 0,
        STATE_STOPPED = 1, // waits to start the music of m_BgmId
        STATE_FADED = 2,   // stopped at the end of the hour
    };

    DECOMP_NOINLINE StageOpusBase(); // 0x00586744

    virtual bool Start(); // 0x005861E8 slot 0x04
    virtual bool Stop(); // 0x0058626C slot 0x08
    // each frame
    virtual void Update(); // 0x005861DC slot 0x1C
    // right after the creation
    virtual void Initialize(); // 0x005861BC slot 0x20
    // the stage is entered (the music of the previous stage was not kept)
    virtual void Enter(); // 0x005861C8 slot 0x24
    virtual void EnterKeep(); // 0x005861D8 slot 0x28
    // the stage is left for the next one (the music is not kept)
    virtual void Leave(stage::Name next); // 0x005861CC slot 0x2C
    // the stage changes to this one
    virtual void Setup(stage::Name stage); // 0x005861C0 slot 0x30
    // the stage changes, the music of the previous stage is kept
    virtual void SetupKeep(stage::Name stage); // 0x005861D0 slot 0x34
    // the stage is ready (the music of the previous stage was not kept)
    virtual void Ready(); // 0x005861C4 slot 0x38
    virtual void ReadyKeep(); // 0x005861D4 slot 0x3C
    // the music stays for the next stage
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753D88 slot 0x40
    {
        return !isOnline;
    }
    // the music of the current (or of the next) hour
    virtual u32 GetBgmId(bool isNextHour); // 0x005861E0 slot 0x44

    // switches the music at the change of the hour: stops it in the last 15 seconds and starts the music of the new hour
    // in the first 15 seconds (isAlways: also when the id stays, isNearHour: near the hour, force: the clock changed)
    void UpdateHourly(bool isAlways, bool isNearHour, bool force); // 0x005862AC

    u32 m_BgmId;           // 0x08
    u32 m_NextBgmId;       // 0x0C
    u8 m_State;            // 0x10 State
    u32 m_Fade;            // 0x14 the frames of the fade out of a stop (set by the game)
    u8 m_Kind;             // 0x18 the kind of the requests (RequestMgr)
    u8 m_IsReady;          // 0x19
    u8 m_IsSuspended;      // 0x1A no hourly change
    u8 m_IsCurrent;        // 0x1B the opus of the current stage (StageOpusHolder)
    u8 m_IsKept;           // 0x1C the music was kept from the previous stage
    u8 m_IsClockChanged;   // 0x1D
};
ASSERT_OFFSET(StageOpusBase, m_Kind, 0x18);
ASSERT_SIZE(StageOpusBase, 0x20);
} // namespace bgm
} // namespace audio
