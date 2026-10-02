#include "nn/os/os_HandleObject.h"
#include "nn/os/os_WaitObject.h"
#include "nn/svc/svc_Api.h"

#define ASM_FUNCTION __attribute__((naked))

namespace nn {
namespace os {
namespace {

// up to this many handles live in an array on the stack of WaitMultiple; more go through
// WaitMultipleImplWithAlloca (the name is ours)
const s32 MAX_HANDLES_ON_STACK = 10;

// the arguments of WaitMultiple, for WaitMultipleImplWithAlloca (member names are ours)
struct WaitMultipleObjectsArgs {
    s32* index;             // 0x00
    WaitObject** objects;   // 0x04
    s32 count;              // 0x08
    bool waitAll;           // 0x0C (loaded with ldrsb in the binary)
    const s64* timeout;     // 0x10
};

// fills handles (room for count) from the objects and waits (inlined everywhere)
inline nn::Result WaitHandles(s32* index, WaitObject** objects, s32 count, bool waitAll, s64 timeout,
                              nn::Handle* handles)
{
    for (s32 i = 0; i < count; i++) {
        handles[i] = objects[i]->GetHandle();
    }
    if (count == 1) {
        *index = 0;
        return nn::svc::WaitSynchronization1(handles[0], timeout);
    }
    return nn::svc::WaitSynchronizationN(index, handles, count, waitAll, timeout);
}

// 0x0014FBDC | fefates:bytes [tier B]
// handles: room for args->count handles
nn::Result WaitMultipleImpl(WaitMultipleObjectsArgs* args, nn::Handle* handles)
{
    return WaitHandles(args->index, args->objects, args->count, args->waitAll, *args->timeout, handles);
}

// 0x0014E1C0 | copied from the binary
// calls f(args, buffer) with a buffer for count handles on the stack (alloca by hand)
ASM_FUNCTION nn::Result WaitMultipleImplWithAlloca(WaitMultipleObjectsArgs* args, s32 count,
                                                   nn::Result (*f)(WaitMultipleObjectsArgs*, nn::Handle*))
{
    asm volatile(
        "push {lr}\n"
        "bics r3, r1, #1\n"
        "addne r1, r1, #1\n"
        "lsl r3, r1, #2\n"
        "sub sp, sp, r3\n"
        "mov r1, sp\n"
        "push {r3}\n"
        "blx r2\n"
        "pop {r3}\n"
        "add sp, sp, r3\n"
        "pop {pc}\n"
    );
}

} // namespace

// 0x0034BF00 | nintendogs:callseq [tier A]
nn::Result nn::os::WaitObject::WaitMultiple(s32* index, nn::os::WaitObject** objects, s32 count, bool waitAll, s64 timeout)
{
    if (count <= MAX_HANDLES_ON_STACK) {
        nn::Handle handles[MAX_HANDLES_ON_STACK];
        return WaitHandles(index, objects, count, waitAll, timeout, handles);
    }
    WaitMultipleObjectsArgs args = {index, objects, count, waitAll, &timeout};
    return WaitMultipleImplWithAlloca(&args, count, WaitMultipleImpl);
}

} // namespace os
} // namespace nn
