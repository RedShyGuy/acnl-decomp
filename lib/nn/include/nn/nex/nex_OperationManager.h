#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class Operation;

// An object called when operations begin (priorities -1024..-1) or end (1..1024); the list is
// sorted by the priority. (The class of the elements is not known; the names are ours.)
class OperationCallback
{
public:
    virtual ~OperationCallback() {}
    virtual void CallMethod(nn::nex::Operation* pOperation) = 0; // slot 0x08

    s32 m_Priority; // 0x04
};

// The operations being processed (a stack: the current one last) and the callbacks around them.
// The names of the members are ours.
// RTTI N2nn3nex16OperationManagerE @ 0x008CE5F0
// vtable 0x008FD204 (vptr 0x008FD20C), offset_to_top 0, 2 entries
class OperationManager : public ::nn::nex::RootObject
{
public:
    // the priorities of the callbacks
    static const s32 PRIORITY_BEGIN_FIRST = -1024;
    static const s32 PRIORITY_BEGIN_LAST = -1;
    static const s32 PRIORITY_END_FIRST = 1;
    static const s32 PRIORITY_END_LAST = 1024;

    OperationManager(); // 0x003830CC | fefates:bytes [tier B]
    virtual ~OperationManager(); // 0x003831E8 slot 0x00 | fefates:bytes [tier B]
    // 0x003831B8 slot 0x04 (deleting dtor)

    void OperationEnds(nn::nex::Operation* pOperation); // 0x00382F20 | mk7dlp:bytes [tier A]
    void OperationBegins(nn::nex::Operation* pOperation); // 0x00382FB4 | mk7dlp:bytes [tier A]
    // the last operation that began (null if there is none; inline in the original, with a copy out
    // of line)
    nn::nex::Operation* GetCurrentOperation() const; // 0x0072B75C | mk7dlp:bytes [tier A]

    qList<OperationCallback*> m_Callbacks; // 0x04
    qList<Operation*> m_Operations;        // 0x1C
};
ASSERT_SIZE(OperationManager, 0x34);
} // namespace nex
} // namespace nn
