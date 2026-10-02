#include "nw/snd/internal/snd_TaskThread.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C5EE4 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskThread::ThreadFunc(unsigned)
{
}

// 0x004C5F70 | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::TaskThread::GetInstance()
{
}

// 0x004C5FEC | fefates:bytes [tier B]
void nw::snd::internal::TaskThread::Create(int, nw::snd::internal::ThreadStack&)
{
}

// 0x004C60B8 | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::TaskThread::Destroy()
{
}

// 0x004C6128 | fefates:bytes [tier B]
nw::snd::internal::TaskThread::~TaskThread()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
