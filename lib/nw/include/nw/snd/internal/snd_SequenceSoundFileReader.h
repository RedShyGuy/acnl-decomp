#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class SequenceSoundFileReader
{
public:
    SequenceSoundFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    SequenceSoundFileReader(const void*); // 0x004C9998 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
