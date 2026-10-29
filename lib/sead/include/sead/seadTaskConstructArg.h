#pragma once

#include "decomp.h"

namespace sead {
class HeapArray;
class TaskMgr;
class TaskParameter;

// what a task is constructed with (the class name is from the reference symbols, the member names are
// ours)
struct TaskConstructArg
{
    HeapArray* heapArray;     // 0x0
    TaskMgr* taskMgr;         // 0x4
    TaskParameter* parameter; // 0x8
};
ASSERT_SIZE(TaskConstructArg, 0xC);
} // namespace sead
