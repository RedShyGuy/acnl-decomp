#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace boss {
class NsDataIdList;
class Task;
class TaskAction;
class TaskOption;
class TaskPolicy;

// counted by a flag: the first Initialize opens boss:U (and ndm)
nn::Result Initialize(); // 0x0046A764 | nintendogs:bytes [tier A]
nn::Result Finalize(); // 0x0046F178 | nintendogs:bytes [tier A]
nn::Result GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode); // 0x0046AD38 | nintendogs:bytes [tier A]
nn::Result RegisterTask(nn::boss::Task* pTask, nn::boss::TaskPolicy* pPolicy, nn::boss::TaskAction* pAction, nn::boss::TaskOption* pOption, unsigned char option); // 0x0046AE1C | nintendogs:bytes [tier A]
nn::Result GetOptoutFlag(bool* pFlag); // 0x0046B004 | tier X
nn::Result SetOptoutFlag(bool flag); // 0x0046B070 | nintendogs:bytes [tier B]
nn::Result GetStorageInfo(unsigned* pSize); // 0x0046B0C8 | nintendogs:bytes [tier A]
nn::Result UnregisterTask(nn::boss::Task* pTask, unsigned char option); // 0x0046B764 | nintendogs:bytes [tier A]
// lists the ids of the downloaded data into the list (in parts: a full list continues where it
// stopped)
nn::Result GetNsDataIdList(unsigned filter, nn::boss::NsDataIdList* pList); // 0x0046B808 | nintendogs:bytes [tier A]
nn::Result RegisterStorage(unsigned extDataId, unsigned size, nn::boss::StorageType type); // 0x0046B910 | nintendogs:bytes [tier B]
nn::Result UnregisterStorage(); // 0x0046BADC | fefates:bytes [tier B]
// waits (up to the timeout) for the event of a finished task
nn::Result WaitFinishWaitEvent(const nn::fnd::TimeSpan& timeout); // 0x0046BB24 | fefates:bytes [tier B]
// a task that runs once at once (its id must be the one of FgOnlyTask)
nn::Result RegisterImmediateTask(nn::boss::Task* pTask, nn::boss::TaskAction* pAction, nn::boss::TaskPolicy* pPolicy, nn::boss::TaskOption* pOption, unsigned char option); // 0x0046BCB4 | fefates:bytes [tier B]
} // namespace boss
} // namespace nn
