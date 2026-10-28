#pragma once

#include "decomp.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
class BasicSound;

// A second limit of playing sounds (e.g. per sound actor) (the class and function names are from the
// reference symbols, the members are ours).
class ExternalSoundPlayer
{
public:
    typedef ut::LinkList<BasicSound, 0x110> SoundList;

    // false if the sound may not play
    bool AppendSound(BasicSound* pSound); // 0x004C8D18 | fefates:bytes [tier B]
    void RemoveSound(BasicSound* pSound); // 0x004C4F64 | fefates:bytes [tier B]
    bool detail_CanPlaySound(int priority); // 0x004C8DE8 | nintendogs:bytes-fuzzy [tier A]
    // the first sound with the lowest priority (NULL: none)
    DECOMP_NOINLINE BasicSound* GetLowestPrioritySound(); // 0x004C8E3C | nintendogs:bytes-fuzzy [tier A]

    SoundList m_SoundList; // 0x0
    int m_PlayableCount;   // 0xC
};
ASSERT_SIZE(ExternalSoundPlayer, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
