#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class SoundHandle;

namespace internal {
class StreamSound;
} // namespace internal

// The handle of a stream sound for the stream functions (the class and function names are from the
// reference symbols, the member is ours).
class StreamSoundHandle
{
public:
    // the stream sound of the handle (the handle becomes its temporary special handle)
    explicit StreamSoundHandle(SoundHandle* pHandle); // 0x004C08F4 | nintendogs:bytes-fuzzy [tier A]
    void DetachSound(); // 0x004C08BC | fefates:bytes [tier B]

    internal::StreamSound* m_pSound; // 0x0 (name is ours)
};
} // namespace snd
} // namespace nw
