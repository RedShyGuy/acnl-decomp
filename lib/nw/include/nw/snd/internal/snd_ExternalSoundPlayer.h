#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class ExternalSoundPlayer
{
public:
    void PauseAllSound(bool, int); // 0x004BF6F4 | mk7dlp:bytes-fuzzy [tier B]
    void RemoveSound(nw::snd::internal::BasicSound*); // 0x004C4F64 | fefates:bytes [tier B]
    void AppendSound(nw::snd::internal::BasicSound*); // 0x004C8D18 | fefates:bytes [tier B]
    void detail_CanPlaySound(int); // 0x004C8DE8 | nintendogs:bytes-fuzzy [tier A]
    void GetLowestPrioritySound(); // 0x004C8E3C | nintendogs:bytes-fuzzy [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
