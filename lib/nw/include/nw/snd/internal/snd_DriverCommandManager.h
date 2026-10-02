#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class DriverCommandManager
{
public:
    void GetInstance(); // 0x001415C0 | nintendogs:callgraph [tier A]
    void FlushCommand(bool); // 0x00141618 | nintendogs:callgraph [tier A]
    void TryAllocMemory(unsigned int); // 0x00141764 | fefates:bytes [tier B]
    void WaitCommandReply(unsigned); // 0x00141858 | nintendogs:callgraph [tier A]
    DriverCommandManager(); // 0x001418BC | nintendogs:bytes-fuzzy [tier A]
    ~DriverCommandManager(); // 0x00143B88 | nintendogs:bytes [tier A]
    void Initialize(void*, unsigned); // 0x004C8E88 | nintendogs:callseq [tier A]
    void AllocMemory(unsigned); // 0x004C8EE4 | nintendogs:callseq-callee [tier A]
    void PushCommand(nw::snd::internal::DriverCommand*); // 0x004C91AC | nintendogs:callseq-callee [tier A]
    void ProcessCommand(); // 0x004C91D0 | nintendogs:bytes [tier A]
    void RecvCommandReply(); // 0x004C921C | nintendogs:callgraph [tier A]
    void GetInstanceForTaskThread(); // 0x004C9288 | nintendogs:callgraph [tier A]
    void Finalize(); // 0x004C92E4 | nintendogs:callseq [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
