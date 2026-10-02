// Assembly wrappers of the system calls that need more than "svc N".
// Instructions marked with an address are copied
// from ACNL USA 1.5 (orig/USA_1_5/code.elf).

#include "nn/svc/svc_Api.h"

// The functions are naked (no prologue / epilogue): SVC_ASM is on their declarations in
// svc_Api.h, GCC does not allow attributes after the parameters of a definition.

namespace nn {
namespace svc {

// 0x0011DC50 | copied from the binary
nn::Result ControlMemory(u32* outAddr, u32 addr0, u32 addr1, u32 size, u32 operation, u32 permission)
{
    asm volatile(
        "push {r0, r4}\n"
        "ldr r0, [sp, #8]\n"
        "ldr r4, [sp, #12]\n"
        "svc 0x01\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "pop {r4}\n"
        "bx lr\n"
    );
}

// 0x0011F440 | copied from the binary
nn::Result QueryMemory(MemoryInfo* info, PageInfo* page, u32 addr)
{
    asm volatile(
        "push {r0, r1, r4, r5, r6}\n"
        "svc 0x02\n"
        "ldr r6, [sp]\n"
        "str r1, [r6]\n"
        "str r2, [r6, #4]\n"
        "str r3, [r6, #8]\n"
        "str r4, [r6, #12]\n"
        "ldr r6, [sp, #4]\n"
        "str r5, [r6]\n"
        "add sp, sp, #8\n"
        "pop {r4, r5, r6}\n"
        "bx lr\n"
    );
}

void ExitProcess()
{
    asm volatile(
        "svc 0x03\n"
        "bx lr\n"
    );
}

nn::Result GetProcessAffinityMask(u8* affinityMask, nn::Handle process, s32 processorCount)
{
    asm volatile(
        "svc 0x04\n"
        "bx lr\n"
    );
}

nn::Result SetProcessAffinityMask(nn::Handle process, const u8* affinityMask, s32 processorCount)
{
    asm volatile(
        "svc 0x05\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetProcessIdealProcessor(s32* processorId, nn::Handle process)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x06\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result SetProcessIdealProcessor(nn::Handle process, s32 processorId)
{
    asm volatile(
        "svc 0x07\n"
        "bx lr\n"
    );
}

// 0x00122BD4 | copied from the binary
nn::Result CreateThread(nn::Handle* thread, void (*entry)(uptr), uptr arg, uptr stackTop, s32 priority, s32 coreNo)
{
    asm volatile(
        "push {r0, r4}\n"
        "ldr r0, [sp, #8]\n"
        "ldr r4, [sp, #12]\n"
        "svc 0x08\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "pop {r4}\n"
        "bx lr\n"
    );
}

void ExitThread()
{
    asm volatile(
        "svc 0x09\n"
        "bx lr\n"
    );
}

void SleepThread(s64 nanoSeconds)
{
    asm volatile(
        "svc 0x0A\n"
        "bx lr\n"
    );
}

// 0x0013DDB0 | copied from the binary
nn::Result GetThreadPriority(s32* priority, nn::Handle thread)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x0B\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result SetThreadPriority(nn::Handle thread, s32 priority)
{
    asm volatile(
        "svc 0x0C\n"
        "bx lr\n"
    );
}

nn::Result GetThreadAffinityMask(u8* affinityMask, nn::Handle thread, s32 processorCount)
{
    asm volatile(
        "svc 0x0D\n"
        "bx lr\n"
    );
}

nn::Result SetThreadAffinityMask(nn::Handle thread, const u8* affinityMask, s32 processorCount)
{
    asm volatile(
        "svc 0x0E\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetThreadIdealProcessor(s32* processorId, nn::Handle thread)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x0F\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result SetThreadIdealProcessor(nn::Handle thread, s32 processorId)
{
    asm volatile(
        "svc 0x10\n"
        "bx lr\n"
    );
}

s32 GetCurrentProcessorNumber()
{
    asm volatile(
        "svc 0x11\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result Run(nn::Handle process, s32 priority, u32 stackSize, s32 argc, u16* argv, u16* envp)
{
    asm volatile(
        "push {r0-r3}\n"
        "push {r4, r5}\n"
        "ldr r0, [sp, #8]\n"
        "ldr r1, [sp, #12]\n"
        "ldr r2, [sp, #16]\n"
        "ldr r3, [sp, #20]\n"
        "ldr r4, [sp, #24]\n"
        "ldr r5, [sp, #28]\n"
        "svc 0x12\n"
        "pop {r4, r5}\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x0014FB90 | copied from the binary
nn::Result CreateMutex(nn::Handle* mutex, bool initialLocked)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x13\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result ReleaseMutex(nn::Handle mutex)
{
    asm volatile(
        "svc 0x14\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreateSemaphore(nn::Handle* semaphore, s32 initialCount, s32 maxCount)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x15\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result ReleaseSemaphore(s32* count, nn::Handle semaphore, s32 releaseCount)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x16\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x0012840C | copied from the binary
nn::Result CreateEvent(nn::Handle* event, nn::os::ResetType resetType)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x17\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result SignalEvent(nn::Handle event)
{
    asm volatile(
        "svc 0x18\n"
        "bx lr\n"
    );
}

nn::Result ClearEvent(nn::Handle event)
{
    asm volatile(
        "svc 0x19\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreateTimer(nn::Handle* timer, nn::os::ResetType resetType)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x1A\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result SetTimer(nn::Handle timer, s64 initial, s64 interval)
{
    asm volatile(
        "push {r0-r3}\n"
        "push {r4}\n"
        "ldr r0, [sp, #4]\n"
        "ldr r2, [sp, #12]\n"
        "ldr r3, [sp, #16]\n"
        "ldr r1, [sp, #20]\n"
        "ldr r4, [sp, #24]\n"
        "svc 0x1B\n"
        "pop {r4}\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result CancelTimer(nn::Handle timer)
{
    asm volatile(
        "svc 0x1C\n"
        "bx lr\n"
    );
}

nn::Result ClearTimer(nn::Handle timer)
{
    asm volatile(
        "svc 0x1D\n"
        "bx lr\n"
    );
}

// 0x00135C8C | copied from the binary
nn::Result CreateMemoryBlock(nn::Handle* memoryBlock, uptr addr, u32 size, u32 myPermission, u32 otherPermission)
{
    asm volatile(
        "push {r0}\n"
        "ldr r0, [sp, #4]\n"
        "svc 0x1E\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result MapMemoryBlock(nn::Handle memoryBlock, uptr addr, u32 myPermission, u32 otherPermission)
{
    asm volatile(
        "svc 0x1F\n"
        "bx lr\n"
    );
}

nn::Result UnmapMemoryBlock(nn::Handle memoryBlock, uptr addr)
{
    asm volatile(
        "svc 0x20\n"
        "bx lr\n"
    );
}

// 0x0011F428 | copied from the binary
nn::Result CreateAddressArbiter(nn::Handle* arbiter)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x21\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

// 0x00135C5C | copied from the binary
nn::Result ArbitrateAddress(nn::Handle arbiter, uptr addr, nn::os::ArbitrationType type, s32 value, s64 timeout)
{
    asm volatile(
        "push {r4, r5}\n"
        "ldr r4, [sp, #8]\n"
        "ldr r5, [sp, #12]\n"
        "svc 0x22\n"
        "pop {r4, r5}\n"
        "bx lr\n"
    );
}

nn::Result CloseHandle(nn::Handle handle)
{
    asm volatile(
        "svc 0x23\n"
        "bx lr\n"
    );
}

nn::Result WaitSynchronization1(nn::Handle handle, s64 timeout)
{
    asm volatile(
        "svc 0x24\n"
        "bx lr\n"
    );
}

// 0x00128444 | copied from the binary
nn::Result WaitSynchronizationN(s32* index, const nn::Handle* handles, s32 handleCount, bool waitAll, s64 timeout)
{
    asm volatile(
        "push {r0, r4}\n"
        "ldr r0, [sp, #8]\n"
        "ldr r4, [sp, #12]\n"
        "svc 0x25\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "pop {r4}\n"
        "bx lr\n"
    );
}

// 0x0014FBA8 | copied from the binary
nn::Result DuplicateHandle(nn::Handle* out, nn::Handle original)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x27\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

s64 GetSystemTick()
{
    asm volatile(
        "svc 0x28\n"
        "bx lr\n"
    );
}

// 0x0014FBC0 | copied from the binary
nn::Result GetHandleInfo(s64* out, nn::Handle handle, u32 type)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x29\n"
        "ldr r3, [sp]\n"
        "str r1, [r3]\n"
        "str r2, [r3, #4]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetSystemInfo(s64* out, u32 type, s32 param)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x2A\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #0]\n"
        "str r2, [r12, #4]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x00122BF8 | copied from the binary
nn::Result GetProcessInfo(s64* out, nn::Handle process, u32 type)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x2B\n"
        "ldr r3, [sp]\n"
        "str r1, [r3]\n"
        "str r2, [r3, #4]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetThreadInfo(s64* out, nn::Handle thread, u32 type)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x2C\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #0]\n"
        "str r2, [r12, #4]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x001283F4 | copied from the binary
nn::Result ConnectToPort(nn::Handle* session, const char* portName)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x2D\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result SendSyncRequest(nn::Handle session)
{
    asm volatile(
        "svc 0x32\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result OpenProcess(nn::Handle* process, u32 processId)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x33\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result OpenThread(nn::Handle* thread, nn::Handle process, u32 threadId)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x34\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x0012F3D8 | copied from the binary
nn::Result GetProcessId(u32* processId, nn::Handle process)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x35\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetProcessIdOfThread(u32* processId, nn::Handle thread)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x36\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// 0x0013A354 | copied from the binary
nn::Result GetThreadId(u32* threadId, nn::Handle thread)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x37\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

// 0x0011DC74 | copied from the binary
nn::Result GetResourceLimit(nn::Handle* resourceLimit, nn::Handle process)
{
    asm volatile(
        "push {r0}\n"
        "svc 0x38\n"
        "ldr r2, [sp]\n"
        "str r1, [r2]\n"
        "add sp, sp, #4\n"
        "bx lr\n"
    );
}

nn::Result GetResourceLimitLimitValues(s64* values, nn::Handle resourceLimit, const u32* names, s32 nameCount)
{
    asm volatile(
        "svc 0x39\n"
        "bx lr\n"
    );
}

nn::Result GetResourceLimitCurrentValues(s64* values, nn::Handle resourceLimit, const u32* names, s32 nameCount)
{
    asm volatile(
        "svc 0x3A\n"
        "bx lr\n"
    );
}

void Break(u32 reason, const void* data, u32 size)
{
    asm volatile(
        "svc 0x3C\n"
        "bx lr\n"
    );
}

nn::Result OutputDebugString(const char* string, s32 length)
{
    asm volatile(
        "svc 0x3D\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreatePort(nn::Handle* serverPort, nn::Handle* clientPort, const char* name, s32 maxSessions)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r2, [sp, #8]\n"
        "ldr r3, [sp, #12]\n"
        "svc 0x47\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #4]\n"
        "str r2, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreateSessionToPort(nn::Handle* session, nn::Handle clientPort)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x48\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreateSession(nn::Handle* serverSession, nn::Handle* clientSession)
{
    asm volatile(
        "push {r0-r3}\n"
        "svc 0x49\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #4]\n"
        "str r2, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result AcceptSession(nn::Handle* session, nn::Handle serverPort)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x4A\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result ReplyAndReceive(s32* index, const nn::Handle* handles, s32 handleCount, nn::Handle replyTarget)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "ldr r3, [sp, #12]\n"
        "svc 0x4F\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result BindInterrupt(u32 interruptId, nn::Handle event, s32 priority, bool isManualClear)
{
    asm volatile(
        "svc 0x50\n"
        "bx lr\n"
    );
}

nn::Result UnbindInterrupt(u32 interruptId, nn::Handle event)
{
    asm volatile(
        "svc 0x51\n"
        "bx lr\n"
    );
}

nn::Result InvalidateProcessDataCache(nn::Handle process, uptr addr, u32 size)
{
    asm volatile(
        "svc 0x52\n"
        "bx lr\n"
    );
}

nn::Result StoreProcessDataCache(nn::Handle process, uptr addr, u32 size)
{
    asm volatile(
        "svc 0x53\n"
        "bx lr\n"
    );
}

nn::Result FlushProcessDataCache(nn::Handle process, uptr addr, u32 size)
{
    asm volatile(
        "svc 0x54\n"
        "bx lr\n"
    );
}

nn::Result StopDma(nn::Handle dma)
{
    asm volatile(
        "svc 0x56\n"
        "bx lr\n"
    );
}

nn::Result GetDmaState(void* state, nn::Handle dma)
{
    asm volatile(
        "svc 0x57\n"
        "bx lr\n"
    );
}

nn::Result SetGpuProt(bool useApplicationRestriction)
{
    asm volatile(
        "svc 0x59\n"
        "bx lr\n"
    );
}

nn::Result SetWifiEnabled(bool enabled)
{
    asm volatile(
        "svc 0x5A\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result DebugActiveProcess(nn::Handle* debug, u32 processId)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "svc 0x60\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result BreakDebugProcess(nn::Handle debug)
{
    asm volatile(
        "svc 0x61\n"
        "bx lr\n"
    );
}

nn::Result TerminateDebugProcess(nn::Handle debug)
{
    asm volatile(
        "svc 0x62\n"
        "bx lr\n"
    );
}

nn::Result GetProcessDebugEvent(void* info, nn::Handle debug)
{
    asm volatile(
        "svc 0x63\n"
        "bx lr\n"
    );
}

nn::Result ContinueDebugEvent(nn::Handle debug, u32 flags)
{
    asm volatile(
        "svc 0x64\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetProcessList(s32* processCount, u32* processIds, s32 maxCount)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "svc 0x65\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result GetThreadList(s32* threadCount, u32* threadIds, s32 maxCount, nn::Handle process)
{
    asm volatile(
        "push {r0-r3}\n"
        "ldr r1, [sp, #4]\n"
        "ldr r2, [sp, #8]\n"
        "ldr r3, [sp, #12]\n"
        "svc 0x66\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result GetDebugThreadContext(void* context, nn::Handle debug, u32 threadId, u32 controlFlags)
{
    asm volatile(
        "svc 0x67\n"
        "bx lr\n"
    );
}

nn::Result SetDebugThreadContext(nn::Handle debug, u32 threadId, void* context, u32 controlFlags)
{
    asm volatile(
        "svc 0x68\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result QueryDebugProcessMemory(MemoryInfo* info, PageInfo* page, nn::Handle debug, uptr addr)
{
    asm volatile(
        "push {r0-r3}\n"
        "push {r4, r5}\n"
        "ldr r2, [sp, #16]\n"
        "ldr r3, [sp, #20]\n"
        "svc 0x69\n"
        "ldr r12, [sp, #8]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #8]\n"
        "str r2, [r12, #4]\n"
        "ldr r12, [sp, #8]\n"
        "str r3, [r12, #8]\n"
        "ldr r12, [sp, #8]\n"
        "str r4, [r12, #12]\n"
        "ldr r12, [sp, #12]\n"
        "str r5, [r12]\n"
        "pop {r4, r5}\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result ReadProcessMemory(void* buffer, nn::Handle debug, uptr addr, u32 size)
{
    asm volatile(
        "svc 0x6A\n"
        "bx lr\n"
    );
}

nn::Result WriteProcessMemory(nn::Handle debug, const void* buffer, uptr addr, u32 size)
{
    asm volatile(
        "svc 0x6B\n"
        "bx lr\n"
    );
}

nn::Result SetHardwareBreakPoint(s32 registerId, u32 control, u32 value)
{
    asm volatile(
        "svc 0x6C\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result ControlProcessMemory(nn::Handle process, uptr addr0, uptr addr1, u32 size, u32 type, u32 permission)
{
    asm volatile(
        "push {r0-r3}\n"
        "push {r4, r5}\n"
        "ldr r0, [sp, #8]\n"
        "ldr r1, [sp, #12]\n"
        "ldr r2, [sp, #16]\n"
        "ldr r3, [sp, #20]\n"
        "ldr r4, [sp, #24]\n"
        "ldr r5, [sp, #28]\n"
        "svc 0x70\n"
        "pop {r4, r5}\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result MapProcessMemory(nn::Handle process, uptr destAddr, u32 size)
{
    asm volatile(
        "svc 0x71\n"
        "bx lr\n"
    );
}

nn::Result UnmapProcessMemory(nn::Handle process, uptr destAddr, u32 size)
{
    asm volatile(
        "svc 0x72\n"
        "bx lr\n"
    );
}

nn::Result TerminateProcess(nn::Handle process)
{
    asm volatile(
        "svc 0x76\n"
        "bx lr\n"
    );
}

nn::Result SetProcessResourceLimits(nn::Handle process, nn::Handle resourceLimit)
{
    asm volatile(
        "svc 0x77\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result CreateResourceLimit(nn::Handle* resourceLimit)
{
    asm volatile(
        "push {r0-r3}\n"
        "svc 0x78\n"
        "ldr r12, [sp, #0]\n"
        "str r1, [r12]\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

nn::Result SetResourceLimitValues(nn::Handle resourceLimit, const u32* names, const s64* values, s32 nameCount)
{
    asm volatile(
        "svc 0x79\n"
        "bx lr\n"
    );
}

nn::Result Backdoor(s32 (*callback)())
{
    asm volatile(
        "svc 0x7B\n"
        "bx lr\n"
    );
}

nn::Result KernelSetState(u32 type, u32 param0, u32 param1, u32 param2)
{
    asm volatile(
        "svc 0x7C\n"
        "bx lr\n"
    );
}

// not in ACNL - generic wrapper
nn::Result QueryProcessMemory(MemoryInfo* info, PageInfo* page, nn::Handle process, uptr addr)
{
    asm volatile(
        "push {r0-r3}\n"
        "push {r4, r5}\n"
        "ldr r2, [sp, #16]\n"
        "ldr r3, [sp, #20]\n"
        "svc 0x7D\n"
        "ldr r12, [sp, #8]\n"
        "str r1, [r12]\n"
        "ldr r12, [sp, #8]\n"
        "str r2, [r12, #4]\n"
        "ldr r12, [sp, #8]\n"
        "str r3, [r12, #8]\n"
        "ldr r12, [sp, #8]\n"
        "str r4, [r12, #12]\n"
        "ldr r12, [sp, #12]\n"
        "str r5, [r12]\n"
        "pop {r4, r5}\n"
        "add sp, sp, #16\n"
        "bx lr\n"
    );
}

} // namespace svc
} // namespace nn

// 0x0012843C | copied from the binary
extern "C" void svcSleepThread(s64 nanoSeconds)
{
    asm volatile(
        "svc 0x0A\n"
        "bx lr\n"
    );
}
