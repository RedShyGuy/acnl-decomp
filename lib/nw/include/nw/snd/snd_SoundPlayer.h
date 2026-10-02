#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class SoundPlayer
{
public:
    void detail_SortPriorityList(nw::snd::internal::BasicSound*); // 0x00137B6C | fefates:bytes [tier B]
    void InsertPriorityList(nw::snd::internal::BasicSound*); // 0x00137B94 | fefates:bytes [tier B]
    void StopAllSound(int); // 0x004BF6B8 | fefates:bytes [tier B]
    void detail_AppendSound(nw::snd::internal::BasicSound*); // 0x004BF744 | fefates:bytes [tier B]
    void detail_CanPlaySound(int); // 0x004BF804 | fefates:bytes [tier B]
    void SetPlayableSoundCount(int); // 0x004BF86C | fefates:bytes [tier B]
    void detail_FreePlayerHeap(nw::snd::internal::PlayerHeap*); // 0x004BF8B8 | fefates:bytes-fuzzy [tier B]
    void detail_AllocPlayerHeap(); // 0x004BF8EC | fefates:bytes [tier B]
    void detail_AppendPlayerHeap(nw::snd::internal::PlayerHeap*); // 0x004BF918 | fefates:bytes [tier B]
    void Update(); // 0x004BF94C | fefates:bytes [tier B]
    void detail_SortPriorityList(bool); // 0x004BF9D8 | fefates:bytes [tier B]
    void SetVolume(float); // 0x004BFB44 | fefates:bytes [tier B]
    SoundPlayer(); // 0x004BFB60 | fefates:bytes [tier B]
    void detail_RemoveSound(nw::snd::internal::BasicSound*); // 0x004C4E98 | nintendogs:callseq [tier A]
};
} // namespace snd
} // namespace nw
