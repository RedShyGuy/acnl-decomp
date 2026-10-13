#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
// The base of the variables with a value per thread: they are in the ThreadVariableList, which
// tells them when everything or the values of a thread go away. The names of the functions and of
// the members are ours.
// RTTI N2nn3nex18ThreadVariableRootE @ 0x008CE760
// vtable 0x008FD588 (vptr 0x008FD590), offset_to_top 0, 5 entries
class ThreadVariableRoot : public ::nn::nex::RootObject
{
public:
    ThreadVariableRoot(); // 0x003906C8 | fefates:callgraph [tier C]
    virtual ~ThreadVariableRoot(); // 0x003907AC slot 0x00
    // 0x00390764 slot 0x04 (deleting dtor)
    // frees the values of all threads
    virtual void ClearAll(); // 0x003906C4 slot 0x08
    // frees the value of the current thread
    virtual void ClearCurrentThread(); // 0x00390698 slot 0x0C
    // frees the value of a thread
    virtual void ClearThread(u32 threadId); // 0x00390694 slot 0x10

    // the id of the current thread (inline in the original, with a copy out of line)
    static u32 GetCurrentThreadId() // 0x0039069C
    {
        u32 threadId;
        nn::Result result = nn::svc::GetThreadId(&threadId, nn::Handle(PSEUDO_HANDLE_CURRENT_THREAD));
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        return threadId;
    }

    ThreadVariableRoot* m_pChainNext; // 0x04
    ThreadVariableRoot* m_pChainPrev; // 0x08
};
ASSERT_SIZE(ThreadVariableRoot, 0xC);
} // namespace nex
} // namespace nn
