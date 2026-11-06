#pragma once

#include "decomp.h"
#include "Audio/bgm/dOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm10EvOpusBaseE @ 0x008D2564
// The music of an event in the town (EventOpusList::Select; StageOpusVillageDowntown plays it). Slot names are ours.
class EvOpusBase : public ::audio::bgm::OpusBase
{
public:
    EvOpusBase() {}

    // the event becomes the current one
    virtual void Begin(); // 0x00584958 slot 0x1C
    // another event (or none) replaces it (isReplaced: by another event)
    virtual void End(bool isReplaced); // 0x00584974 slot 0x20
    // each frame while it is the current event
    virtual void Update(); // 0x00584970 slot 0x24
    // the stage is entered again with the music kept
    virtual void Resume(); // 0x0058495C slot 0x28
    // the event has its own music (GetBgmId)
    virtual bool HasBgm() const // 0x00753D08 slot 0x2C
    {
        return false;
    }
    virtual u32 GetBgmId() const // 0x00753D10 slot 0x30
    {
        return 0;
    }
    // the music has to be selected again
    virtual bool IsChanged() // 0x00753D00 slot 0x34
    {
        return false;
    }
    // the request of the music of the stage: true when the event made it itself
    virtual bool StartRequest(u8 kind, u32 id); // 0x00584960 slot 0x38
    virtual bool StopRequest(u8 kind, u32 id); // 0x00584968 slot 0x3C
};
ASSERT_SIZE(EvOpusBase, 0x8);
} // namespace bgm
} // namespace audio
