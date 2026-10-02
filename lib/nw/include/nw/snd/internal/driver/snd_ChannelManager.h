#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class ChannelManager
{
public:
    void Initialize(void*, unsigned long); // 0x004CCB48 | nintendogs:bytes-fuzzy [tier A]
    void GetInstance(); // 0x004CCB88 | nintendogs:callseq-callee [tier A]
    void UpdateAllChannel(); // 0x004CCBE4 | fefates:bytes [tier B]
    void GetRequiredMemSize(int); // 0x004CCC1C | nintendogs:callseq-callee [tier A]
    void Free(nw::snd::internal::driver::Channel*); // 0x004CCCB4 | nintendogs:callseq-callee [tier A]
    void Alloc(); // 0x004CCCF0 | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004CCD28 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
