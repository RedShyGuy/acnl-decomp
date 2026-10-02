// Classes from the anonymous namespace of the original os_Default.cpp

#include "decomp.h"
#include "nn/os/os_AutoStackManager.h"
#include "nn/os/os_Mutex.h"
#include "nn/os/os_StackMemoryBlock.h"
#include "nn/os/detail/detail_Api.h"

namespace nn {
namespace os {
namespace {

// vtable 0x008B4520 (vptr 0x008B4528), offset_to_top 0, 4 entries
// Stacks from the memory block space. The StackMemoryBlock that describes a stack lies at the
// top of the stack itself. Member names are ours.
class DefaultAutoStackManager : public nn::os::AutoStackManager
{
public:
    // inline in __sti___14_os_Default_cpp (0x00790C98)
    DefaultAutoStackManager() : mIsMutexInitialized(false) {}
    virtual ~DefaultAutoStackManager();
    virtual void* Construct(size_t stackSize);
    virtual void Destruct(void* stackBottom, bool isError);

private:
    Mutex mMutex;                   // 0x004, serializes the use of mFreeStack
    bool mIsMutexInitialized;       // 0x008
    uptr mFreeStack[0x204 / 4];     // 0x00C, the stack an exiting thread frees its own stack on (top 0x210)

    static void CheckLayout()
    {
        ASSERT_OFFSET(DefaultAutoStackManager, mIsMutexInitialized, 0x8);
        ASSERT_OFFSET(DefaultAutoStackManager, mFreeStack, 0xC);
    }
};

// the StackMemoryBlock at the top of a stack, 8 byte aligned
const size_t BLOCK_IN_STACK_SIZE = (sizeof(StackMemoryBlock) + 7) & ~7u;

// 0x0034C6F4 slot 0x00 | fefates:bytes
// 0x0034C690 slot 0x04 (deleting dtor)
DefaultAutoStackManager::~DefaultAutoStackManager()
{
    if (mIsMutexInitialized) {
        mMutex.Finalize();
        mIsMutexInitialized = false;
    }
}

// 0x0034C5EC slot 0x08 | fefates:bytes
void* DefaultAutoStackManager::Construct(size_t stackSize)
{
    if (!mIsMutexInitialized) {
        mMutex.Initialize(false);
        mIsMutexInitialized = true;
    }
    StackMemoryBlock block;
    nnosStackMemoryBlockAllocate(&block, stackSize);
    uptr bottom = nnosStackMemoryBlockGetStackBottom(&block);
    // the block moves onto the stack it describes
    StackMemoryBlock* inStack = reinterpret_cast<StackMemoryBlock*>(bottom - BLOCK_IN_STACK_SIZE);
    nnosStackMemoryBlockInitialize(inStack);
    detail::Switch(inStack, &block);
    return inStack;
}

// 0x0034C594 slot 0x0C | fefates:bytes
void DefaultAutoStackManager::Destruct(void* stackBottom, bool isError)
{
    StackMemoryBlock* block = static_cast<StackMemoryBlock*>(stackBottom);
    if (isError) {
        nnosStackMemoryBlockFree(block);
        return;
    }
    // Called by the exiting thread on the stack to free (Thread::CallDestructorAndExit, with the
    // return address set to ExitThread): free it from mFreeStack and return straight to the
    // caller's return address. The mutex is never unlocked: the kernel releases it when the
    // thread ends.
    mMutex.Lock();
    detail::CallOnStack(reinterpret_cast<uptr>(mFreeStack) + sizeof(mFreeStack), detail::FreeStackMemoryBlock,
                        block, reinterpret_cast<uptr>(__builtin_return_address(0)));
}

// constructed by __sti___14_os_Default_cpp (0x00790C98), which also registers the destructor
// with __aeabi_atexit
// 0x00AEB618 (name is ours)
DefaultAutoStackManager s_DefaultAutoStackManager;

} // namespace
} // namespace os
} // namespace nn
