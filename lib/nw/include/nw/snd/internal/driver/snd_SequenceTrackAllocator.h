#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class SequenceSoundPlayer;
class SequenceTrack;

// The tracks of the sequence sound players (the class and function names are from the reference symbols).
// RTTI N2nw3snd8internal6driver22SequenceTrackAllocatorE @ 0x008D0B38
class SequenceTrackAllocator
{
public:
    // (inline)
    SequenceTrackAllocator() {}
    virtual ~SequenceTrackAllocator() {} // slot 0x00

    // a track for the player (NULL: none free)
    virtual SequenceTrack* AllocTrack(SequenceSoundPlayer* pPlayer) = 0; // slot 0x08
    virtual void FreeTrack(SequenceTrack* pTrack) = 0; // slot 0x0C
    virtual int GetAllocatableTrackCount() const = 0; // slot 0x10
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
