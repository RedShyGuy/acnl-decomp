#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class GroupFile
{
public:
    class FileHeader;
    struct GroupItemInfoEx { u32 _unknown; }; // TODO: real type unknown (placeholder)
};
} // namespace internal
} // namespace snd
} // namespace nw
