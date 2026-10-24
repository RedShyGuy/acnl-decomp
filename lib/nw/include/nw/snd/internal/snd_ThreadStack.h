#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// The stack of a thread of the sound library (the class name and Finalize are from the reference symbols,
// the members are ours).
class ThreadStack
{
public:
    // (inline)
    ThreadStack() : m_pBuffer(nullptr), m_Size(0) {}
    ~ThreadStack(); // 0x004C6BE0 (name is ours)

    void Finalize(); // 0x004C6BD0 | mk7dlp:callseq-callee [tier A]

    // (inline; name is ours) the end of the stack, where it starts
    uptr GetStackBottom() const { return reinterpret_cast<uptr>(m_pBuffer) + m_Size; }

    void* m_pBuffer; // 0x0
    u32 m_Size;      // 0x4
};
ASSERT_SIZE(ThreadStack, 0x8);
} // namespace internal
} // namespace snd
} // namespace nw
