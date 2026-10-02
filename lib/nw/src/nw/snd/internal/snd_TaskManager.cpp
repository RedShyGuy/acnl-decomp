#include "nw/snd/internal/snd_TaskManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D9C0 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::Initialize()
{
}

// 0x001429BC | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::CancelWaitTask()
{
}

// 0x00143B6C | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::TaskManager::Finalize()
{
}

// 0x004C688C | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::AppendTask(nw::snd::internal::Task*, nw::snd::internal::TaskManager::TaskPriority)
{
}

// 0x004C68E4 | nintendogs:callgraph [tier A]
void nw::snd::internal::TaskManager::ExecuteTask()
{
}

// 0x004C698C | nintendogs:callgraph [tier A]
void nw::snd::internal::TaskManager::GetInstance()
{
}

// 0x004C6A3C | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::GetNextTask(nw::snd::internal::TaskManager::TaskPriority, bool)
{
}

// 0x004C6AA0 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::RemoveTaskById(unsigned)
{
}

// 0x004C6B2C | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::WaitTask()
{
}

// 0x004C6BB0 | fefates:bytes [tier B]
nw::snd::internal::TaskManager::~TaskManager()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
