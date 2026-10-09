#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace ndm {
namespace CTR {
namespace detail {
// The commands of the service ndm:u (3dbrew "NDM Services"); all static, on s_Session. The
// function names are from the symbols.
class Interface
{
public:
    static nn::Result SuspendDaemons(bit32 mask); // 0x00124724 | nintendogs:bytes [tier A]
    static nn::Result OverrideDefaultDaemons(bit32 mask); // 0x00124760 | nintendogs:bytes [tier A]
    static nn::Result ResumeDaemons(bit32 mask); // 0x00144C20 | nintendogs:bytes [tier A]
    static nn::Result QueryExclusiveMode(int* pMode); // 0x00354AF4 | nintendogs:bytes [tier B]
    static nn::Result EnterExclusiveState(int mode); // 0x00354B34 | nintendogs:bytes [tier A]
    static nn::Result LeaveExclusiveState(); // 0x00354B78 | nintendogs:bytes [tier A]

    // Called right in front of the methods above; armlink left only a nop there (a removed tail
    // call). Names are ours.
    static nn::Result OverrideDefaultDaemonsEntry(bit32 mask); // 0x0012475C (name is ours)
    static nn::Result EnterExclusiveStateEntry(s32 mode); // 0x00354B30 (name is ours)
    static nn::Result LeaveExclusiveStateEntry(); // 0x00354B74 (name is ours)
};

// the session of ndm:u (made by nn::ndm::Initialize; name is ours)
extern nn::Handle s_Session; // 0x0097E8E8
} // namespace detail
} // namespace CTR
} // namespace ndm
} // namespace nn
