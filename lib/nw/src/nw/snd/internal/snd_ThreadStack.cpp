#include "nw/snd/internal/snd_ThreadStack.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C6BD0 | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::ThreadStack::Finalize()
{
    m_pBuffer = nullptr;
    m_Size = 0;
}

// 0x004C6BE0 (name is ours)
nw::snd::internal::ThreadStack::~ThreadStack()
{
    Finalize();
}

} // namespace internal
} // namespace snd
} // namespace nw
