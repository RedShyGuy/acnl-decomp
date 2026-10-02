#include "nn/os/detail/detail_Api.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/os/os_ThreadLocalStorage.h"
#include "nn/os/os_StackMemoryBlock.h"
#include "nn/svc/svc_Api.h"

#define ASM_FUNCTION __attribute__((naked))

namespace nn {
namespace os {
namespace detail {
namespace {

// thread stacks (except the main thread's) live in [STACK_SPACE_BEGIN, main thread's stack)
const uptr STACK_SPACE_BEGIN = 0x0E000000;
const uptr STACK_SPACE_END = 0x10000000;
const uptr SHARED_MEMORY_SPACE_BEGIN = 0x10000000;
const size_t SHARED_MEMORY_SPACE_SIZE = 0x04000000;
const size_t PAGE_SIZE = 0x1000;

// Library priorities: 0..32 for applications (kernel 32..64), and two ranges for the system
// that start at these values (kernel 24..63 and 0..64). What the bases stand for is unknown.
const u32 SYSTEM_RANGE_A = 0x5109D500;
const u32 SYSTEM_RANGE_B = 0x6C8DA500;

} // namespace

// 0x00AE1F64
AddressSpaceManager s_MemoryBlockSpace;
// 0x00AE1F7C
AddressSpaceManager s_StackSpace;
// 0x00AF6234
AddressSpaceManager s_SharedMemorySpace;
// 0x00975F80
bool s_IsMemoryBlockEnabled;
// 0x00975F88
uptr s_MainThreadLocalRegion;

// 0x0011DFA4 | tier C
void SaveThreadLocalRegionAddress()
{
    s_MainThreadLocalRegion = reinterpret_cast<uptr>(GetThreadLocalRegion());
}

// 0x0011DF90 | tier C
void InitializeSharedMemory()
{
    nnosAddressSpaceManagerInitialize(&s_SharedMemorySpace, SHARED_MEMORY_SPACE_BEGIN, SHARED_MEMORY_SPACE_SIZE);
}

// 0x0011DF28 | fefates:bytes [tier B]
void InitializeStackMemory()
{
    // the stack space ends where the used pages below 0x10000000 (the main thread's stack) begin
    uptr end = STACK_SPACE_END;
    for (;;) {
        end -= PAGE_SIZE;
        nn::svc::MemoryInfo info;
        nn::svc::PageInfo page;
        if (nn::svc::QueryMemory(&info, &page, end).IsFailure() || info.state == 0) {
            break;
        }
        end = info.baseAddress;
        if (end <= STACK_SPACE_BEGIN) {
            break;
        }
    }
    if (end < STACK_SPACE_BEGIN) {
        end = STACK_SPACE_BEGIN;
    }
    nnosAddressSpaceManagerInitialize(&s_StackSpace, STACK_SPACE_BEGIN, end - STACK_SPACE_BEGIN);
}

// 0x00128370 | nintendogs:callseq [tier A]
// first thing every thread does (Thread::ThreadStart)
void InitializeThreadEnvrionment()
{
    ThreadLocalStorage::ClearAllSlots();
    CTR::SetupThreadCppExceptionEnvironment();
    InitializeFpscr();
}

// 0x00128384 | copied from the binary
// FPSCR = flush to zero (bit 24) + default NaN (bit 25), round to nearest
ASM_FUNCTION void InitializeFpscr()
{
    asm volatile(
        "mov r0, #0x3000000\n"
        "vmsr fpscr, r0\n"
        "bx lr\n"
    );
}

// 0x00129AC4 | fefates:bytes [tier B]
// end of a thread: the destructor of every taken slot gets the thread's value
void InvokeAllTlsDestructors()
{
    for (s32 i = 0; i < ThreadLocalStorage::MAX_SLOTS; i++) {
        void (*destructor)(uptr) = ThreadLocalStorage::s_Destructors[i];
        if (static_cast<u32>(i) < ThreadLocalStorage::MAX_SLOTS && ((ThreadLocalStorage::s_UsedSlots >> i) & 1)
            && destructor) {
            destructor(GetThreadLocalRegion()[i]);
        }
    }
}

// 0x0013E888 | nintendogs:bytes [tier A]
s32 ConvertSvcToLibraryPriority(s32 priority)
{
    if (priority >= 32) {
        return priority - 32;
    }
    if (priority >= 24) {
        return priority - 24 + SYSTEM_RANGE_A;
    }
    return priority + SYSTEM_RANGE_B;
}

// 0x00143224 | nintendogs:callgraph [tier A]
void FreeToSharedMemorySpace(nn::os::MemoryBlockBase* block)
{
    s_SharedMemorySpace.Free(block);
}

// 0x0034C904 | mk7dlp:callgraph [tier A]
void FreeToMemoryBlockSpace(nn::os::MemoryBlockBase* block)
{
    s_MemoryBlockSpace.Free(block);
}

// 0x0034C914 | nintendogs:bytes [tier A]
// see SYSTEM_RANGE_A / B; invalid priorities become -1
s32 ConvertLibraryToSvcPriority(s32 priority)
{
    if (static_cast<u32>(priority) <= 32) {
        return priority + 32;
    }
    if (static_cast<u32>(priority) - SYSTEM_RANGE_A <= 39) {
        return priority - SYSTEM_RANGE_A + 24;
    }
    if (static_cast<u32>(priority) - SYSTEM_RANGE_B <= 64) {
        return priority - SYSTEM_RANGE_B;
    }
    return -1;
}

// 0x0034C950 | mk7dlp:bytes [tier A]
uptr AllocateFromMemoryBlockSpace(nn::os::MemoryBlockBase* block, size_t size)
{
    return s_MemoryBlockSpace.Allocate(block, size, 0);
}

// 0x0034C96C | nintendogs:bytes [tier A]
uptr AllocateFromSharedMemorySpace(nn::os::MemoryBlockBase* block, size_t size)
{
    // one page between two shared memory blocks
    return s_SharedMemorySpace.Allocate(block, size, PAGE_SIZE);
}

// 0x0034C8F4 | tier C
bool IsMemoryBlockEnabled()
{
    return s_IsMemoryBlockEnabled;
}

// 0x0034C988 | tier C
void Switch(nn::os::StackMemoryBlock* to, nn::os::StackMemoryBlock* from)
{
    s_MemoryBlockSpace.Switch(to, from);
}

} // namespace detail
} // namespace os
} // namespace nn
