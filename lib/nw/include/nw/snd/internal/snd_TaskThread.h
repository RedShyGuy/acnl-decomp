#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class TaskThread
{
public:
    void ThreadFunc(unsigned); // 0x004C5EE4 | nintendogs:bytes [tier A]
    void GetInstance(); // 0x004C5F70 | mk7dlp:callseq-callee [tier A]
    void Create(int, nw::snd::internal::ThreadStack&); // 0x004C5FEC | fefates:bytes [tier B]
    void Destroy(); // 0x004C60B8 | mk7dlp:callseq-callee [tier A]
    ~TaskThread(); // 0x004C6128 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
