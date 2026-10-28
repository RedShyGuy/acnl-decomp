#pragma once

#include "decomp.h"
#include "nw/snd/snd_Global.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
class BasicSound;
class PlayerHeap;
} // namespace internal

// The sounds of a player and their limits, the player heaps and the parameters of the player that go
// into all its sounds (the class and function names are from the reference symbols, the members and the
// names of the unnamed functions are ours).
class SoundPlayer
{
public:
    typedef ut::LinkList<internal::BasicSound, 0x100> SoundList;
    typedef ut::LinkList<internal::BasicSound, 0x108> PriorityList;
    typedef ut::LinkList<internal::PlayerHeap, 0x1C> PlayerHeapList;
    // the priorities of the sounds
    static const int PRIORITY_COUNT = 128; // (name is ours)

    SoundPlayer(); // 0x004BFB60 | fefates:bytes [tier B]

    void Update(); // 0x004BF94C | fefates:bytes [tier B]
    void StopAllSound(int fadeFrames); // 0x004BF6B8 | fefates:bytes [tier B]
    // (symbols.json: ExternalSoundPlayer::PauseAllSound from fuzzy bytes; the code walks the sound list of
    // the SoundPlayer)
    void PauseAllSound(bool isPause, int fadeFrames); // 0x004BF6F4 (name after mk7dlp)
    void SetVolume(f32 volume); // 0x004BFB44 | fefates:bytes [tier B]
    void SetBiquadFilter(int type, f32 value); // 0x004BF738 (name is ours)
    void SetFxSend(AuxBus bus, f32 send); // 0x004BFAF8 (name is ours)
    void SetPlayableSoundCount(int count); // 0x004BF86C | fefates:bytes [tier B]

    // false if the sound may not play (all places taken by sounds with a higher priority)
    bool detail_AppendSound(nw::snd::internal::BasicSound* pSound); // 0x004BF744 | fefates:bytes [tier B]
    void detail_RemoveSound(nw::snd::internal::BasicSound* pSound); // 0x004C4E98 | nintendogs:callseq [tier A]
    bool detail_CanPlaySound(int priority); // 0x004BF804 | fefates:bytes [tier B]
    // the sound takes its place for its new priority
    void detail_SortPriorityList(nw::snd::internal::BasicSound* pSound); // 0x00137B6C | fefates:bytes [tier B]
    // all sounds again by priority (reverse: the later sounds of a priority first)
    void detail_SortPriorityList(bool reverse); // 0x004BF9D8 | fefates:bytes [tier B]
    void InsertPriorityList(nw::snd::internal::BasicSound* pSound); // 0x00137B94 | fefates:bytes [tier B]
    // the function called with a temporary handle of each sound (reverse: from the last one; the
    // function may stop the sound); the game instantiates it (name is ours)
    template <typename Function>
    Function ForEachSound(Function function, bool reverse);

    void detail_AppendPlayerHeap(nw::snd::internal::PlayerHeap* pHeap); // 0x004BF918 | fefates:bytes [tier B]
    nw::snd::internal::PlayerHeap* detail_AllocPlayerHeap(); // 0x004BF8EC | fefates:bytes [tier B]
    void detail_FreePlayerHeap(nw::snd::internal::PlayerHeap* pHeap); // 0x004BF8B8 | fefates:bytes-fuzzy [tier B]

    // (inline; name is ours) the sound with the lowest priority (the first of the priority list)
    internal::BasicSound* GetLowestPrioritySound() { return PriorityList::GetPointerFromNode(m_PriorityList.begin().GetNode()); }

    SoundList m_SoundList;                 // 0x00
    PriorityList m_PriorityList;           // 0x0C lowest priority first
    PlayerHeapList m_PlayerHeapFreeList;   // 0x18
    PlayerHeapList m_PlayerHeapUsedList;   // 0x24 their load task still runs
    int m_PlayableCount;                   // 0x30
    int m_PlayableLimit;                   // 0x34
    int m_PlayerHeapCount;                 // 0x38
    f32 m_Volume;                          // 0x3C
    f32 m_LpfFreq;                         // 0x40
    int m_BiquadType;                      // 0x44 (-1: none)
    f32 m_BiquadValue;                     // 0x48
    f32 m_MainSend;                        // 0x4C
    f32 m_FxSend[2];                       // 0x50
    bool m_IsFirstComeBased;               // 0x58 equal priorities: the older sound wins
    u8 padding0x59[3];                     // 0x59
};
ASSERT_SIZE(SoundPlayer, 0x5C);
} // namespace snd
} // namespace nw
