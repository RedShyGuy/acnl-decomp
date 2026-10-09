#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace boss {
class TaskStatus;

// RTTI N2nn4boss4TaskE @ 0x008D038C
// A task of the service, named by its id (at most 7 characters); the member names are ours.
class Task
{
public:
    Task();
    virtual ~Task();

    nn::Result Initialize(const char* pTaskId); // 0x0046C1AC | nintendogs:bytes [tier B]
    // waits until the task has run (state 6 / 7) or cannot run
    nn::Result WaitFinish(const nn::fnd::TimeSpan& timeout); // 0x0046C20C | fefates:bytes-fuzzy [tier B]
    nn::Result UpdateCount(unsigned count); // 0x0046C2FC | nintendogs:bytes [tier A]
    nn::Result GetStateDetail(nn::boss::TaskStatus* pStatus, bool flag, unsigned char* pState, unsigned char option); // 0x0046C394 | fefates:bytes [tier B]
    nn::Result StartImmediate(); // 0x0046C484 | nintendogs:bytes [tier A]
    // 3 when unknown
    nn::boss::TaskServiceStatus GetServiceStatus(); // 0x0046C524 | fefates:bytes [tier B]
    nn::Result Start(); // 0x0046C5C0 | nintendogs:bytes [tier A]
    // the state code (9 when it cannot be read)
    u8 GetState(bool flag, unsigned int* pCount, unsigned char* pDetail); // 0x0046C64C | fefates:bytes-fuzzy [tier B]
    // the result code (3 for a wrong id, 2 when it cannot be read)
    nn::boss::TaskResultCode GetResult(unsigned* pErrorCode, unsigned char* pDetail); // 0x0046C738 | nintendogs:bytes [tier A]

    u32 m_Unknown04; // 0x04
    u32 m_Unknown08; // 0x08
    char m_TaskId[8]; // 0x0C
};
ASSERT_SIZE(Task, 0x14);
} // namespace boss
} // namespace nn
