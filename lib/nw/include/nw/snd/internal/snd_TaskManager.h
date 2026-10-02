#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class TaskManager
{
public:
    struct TaskPriority { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Initialize(); // 0x0013D9C0 | nintendogs:bytes [tier A]
    void CancelWaitTask(); // 0x001429BC | nintendogs:bytes [tier A]
    void Finalize(); // 0x00143B6C | mk7dlp:callseq-callee [tier A]
    void AppendTask(nw::snd::internal::Task*, nw::snd::internal::TaskManager::TaskPriority); // 0x004C688C | nintendogs:bytes [tier A]
    void ExecuteTask(); // 0x004C68E4 | nintendogs:callgraph [tier A]
    void GetInstance(); // 0x004C698C | nintendogs:callgraph [tier A]
    void GetNextTask(nw::snd::internal::TaskManager::TaskPriority, bool); // 0x004C6A3C | nintendogs:bytes [tier A]
    void RemoveTaskById(unsigned); // 0x004C6AA0 | nintendogs:bytes [tier A]
    void WaitTask(); // 0x004C6B2C | nintendogs:bytes [tier A]
    ~TaskManager(); // 0x004C6BB0 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
