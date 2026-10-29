#pragma once

#include "decomp.h"
#include "sead/seadAudioFxCtr.h"
#include "sead/seadAudioGlobal.h"
#include "sead/seadCheckedArray.h"

namespace nw {
namespace snd {
class SoundHeap;
} // namespace snd
} // namespace nw

namespace sead {
class AudioFxMgr;
class AudioFxMemoryMgrCtr;

// The effects of the two aux buses (up to three each) with their own effect and memory managers: the
// game collects the effects with append and gives them memory and their buses with setup (the object
// lies in the game's SoundFxMgr at 0x14; there is no RTTI, the class and all names are ours).
class AudioFxSet
{
public:
    static const s32 FX_NUM_PER_BUS = 3;

    // takes the effects off their buses and deletes the managers
    ~AudioFxSet(); // 0x00141BD8 (name is ours)

    // the number of effects of bus
    DECOMP_EMIT_COPY s32 getNum(s32 bus) const { return mNum[bus]; } // 0x0074CAC0 (name is ours)
    // whether fx may be added to bus: an empty bus takes any effect, otherwise all effects of the bus and fx
    // must be ones that are no effects of nn::snd
    DECOMP_EMIT_COPY bool canAppend(s32 bus, AudioFx* fx) const // 0x0074CAD4 (name is ours)
    {
        s32 num = mNum[bus];
        if (num == 0) {
            return true;
        }
        for (s32 i = 0; i < num; i++) {
            if (!DynamicCast<AudioFxCtr>(mFx[bus][i])->isNw()) {
                return false;
            }
        }
        if (!DynamicCast<AudioFxCtr>(fx)->isNw()) {
            return false;
        }
        return true;
    }

    // clears the buses that have effects (fading out over fadeFrames) and forgets the effects
    void reset(s32 fadeFrames); // 0x00549328 (name is ours)
    // the memory of all effects from soundHeap (NULL: the one of the AudioPlayerCtr), then each effect on
    // its bus (false if one could not be appended)
    bool setup(nw::snd::SoundHeap* soundHeap); // 0x005493CC (name is ours)
    // fx for bus (false if the bus is full or canAppend says no)
    bool append(s32 bus, AudioFx* fx); // 0x00549604 (name is ours)

    AudioFxMgr* mFxMgr;                                         // 0x00
    AudioFxMemoryMgrCtr* mMemoryMgr;                            // 0x04
    CheckedArray<CheckedArray<AudioFx*, FX_NUM_PER_BUS>, 2> mFx; // 0x08
    CheckedArray<s32, 2> mNum;                                  // 0x20
    bool field_0x28;                                            // 0x28, cleared by reset and setup
};
ASSERT_SIZE(AudioFxSet, 0x2C);
} // namespace sead
