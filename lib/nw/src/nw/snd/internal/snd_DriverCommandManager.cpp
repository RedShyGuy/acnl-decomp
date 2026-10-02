#include "nw/snd/internal/snd_DriverCommandManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x001415C0 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::GetInstance()
{
}

// 0x00141618 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::FlushCommand(bool)
{
}

// 0x00141764 | fefates:bytes [tier B]
void nw::snd::internal::DriverCommandManager::TryAllocMemory(unsigned int)
{
}

// 0x00141858 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::WaitCommandReply(unsigned)
{
}

// 0x001418BC | nintendogs:bytes-fuzzy [tier A]
nw::snd::internal::DriverCommandManager::DriverCommandManager()
{
}

// 0x00143B88 | nintendogs:bytes [tier A]
nw::snd::internal::DriverCommandManager::~DriverCommandManager()
{
}

// 0x004C8E88 | nintendogs:callseq [tier A]
void nw::snd::internal::DriverCommandManager::Initialize(void*, unsigned)
{
}

// 0x004C8EE4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::DriverCommandManager::AllocMemory(unsigned)
{
}

// 0x004C91AC | nintendogs:callseq-callee [tier A]
void nw::snd::internal::DriverCommandManager::PushCommand(nw::snd::internal::DriverCommand*)
{
}

// 0x004C91D0 | nintendogs:bytes [tier A]
void nw::snd::internal::DriverCommandManager::ProcessCommand()
{
}

// 0x004C921C | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::RecvCommandReply()
{
}

// 0x004C9288 | nintendogs:callgraph [tier A]
void nw::snd::internal::DriverCommandManager::GetInstanceForTaskThread()
{
}

// 0x004C92E4 | nintendogs:callseq [tier A]
void nw::snd::internal::DriverCommandManager::Finalize()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
