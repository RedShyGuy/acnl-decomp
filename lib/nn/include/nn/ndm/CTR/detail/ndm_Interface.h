#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ndm {
namespace CTR {
namespace detail {
class Interface
{
public:
    void OverrideDefaultDaemons(unsigned); // 0x00124760 | nintendogs:bytes [tier A]
    void QueryExclusiveMode(int*); // 0x00354AF4 | nintendogs:bytes [tier B]
    void EnterExclusiveState(int); // 0x00354B34 | nintendogs:bytes [tier A]
    void LeaveExclusiveState(); // 0x00354B78 | nintendogs:bytes [tier A]

    // Called without an object right in front of the methods above; armlink left only a nop
    // there (a removed tail call). Names are ours.
    static nn::Result EnterExclusiveStateEntry(s32 mode); // 0x00354B30 (name is ours)
    static nn::Result LeaveExclusiveStateEntry(); // 0x00354B74 (name is ours)
};
} // namespace detail
} // namespace CTR
} // namespace ndm
} // namespace nn
