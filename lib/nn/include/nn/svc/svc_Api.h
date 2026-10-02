#pragma once

// nn::svc - the 3DS system calls (supervisor calls).
// After the 3dbrew SVC table (register usage as in libctru). Calls whose arguments are
// already in r0-r3 are inline "svc N" in the original (ARMCC's __svc); NN_SVC_INLINE(N)
// marks them.

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/os/os_Types.h"

#define NN_SVC_INLINE(number)

// The system call functions are naked: their body is plain assembly (svc_Api.cpp). The attribute
// is at the end of the declarations because GCC does not allow it after the parameters of a
// definition.
#define SVC_ASM __attribute__((naked))

// C wrappers that the original calls instead of the inline svc (only the ones that are used)
extern "C" void svcSleepThread(s64 nanoSeconds) SVC_ASM; // svc 0x0A | 0x0012843C

namespace nn {
namespace svc {

// QueryMemory output (3dbrew "MemoryInfo")
struct MemoryInfo {
    uptr baseAddress;
    u32 size;
    u32 permission;
    u32 state;
};
ASSERT_SIZE(MemoryInfo, 0x10);

// QueryMemory output (3dbrew "PageInfo")
struct PageInfo {
    u32 flags;
};
ASSERT_SIZE(PageInfo, 0x4);

nn::Result ControlMemory(u32* outAddr, u32 addr0, u32 addr1, u32 size, u32 operation, u32 permission) SVC_ASM; // svc 0x01 | 0x0011DC50 (name from reference symbols)
nn::Result QueryMemory(MemoryInfo* info, PageInfo* page, u32 addr) SVC_ASM; // svc 0x02 | 0x0011F440 (name inferred from the svc number)
NN_SVC_INLINE(0x03) DECOMP_NORETURN void ExitProcess() SVC_ASM; // svc 0x03
NN_SVC_INLINE(0x04) nn::Result GetProcessAffinityMask(u8* affinityMask, nn::Handle process, s32 processorCount) SVC_ASM; // svc 0x04
NN_SVC_INLINE(0x05) nn::Result SetProcessAffinityMask(nn::Handle process, const u8* affinityMask, s32 processorCount) SVC_ASM; // svc 0x05
nn::Result GetProcessIdealProcessor(s32* processorId, nn::Handle process) SVC_ASM; // svc 0x06
NN_SVC_INLINE(0x07) nn::Result SetProcessIdealProcessor(nn::Handle process, s32 processorId) SVC_ASM; // svc 0x07
nn::Result CreateThread(nn::Handle* thread, void (*entry)(uptr), uptr arg, uptr stackTop, s32 priority, s32 coreNo) SVC_ASM; // svc 0x08 | 0x00122BD4 (name from reference symbols)
NN_SVC_INLINE(0x09) DECOMP_NORETURN void ExitThread() SVC_ASM; // svc 0x09
NN_SVC_INLINE(0x0A) void SleepThread(s64 nanoSeconds) SVC_ASM; // svc 0x0A
nn::Result GetThreadPriority(s32* priority, nn::Handle thread) SVC_ASM; // svc 0x0B | 0x0013DDB0 (name from reference symbols)
NN_SVC_INLINE(0x0C) nn::Result SetThreadPriority(nn::Handle thread, s32 priority) SVC_ASM; // svc 0x0C
NN_SVC_INLINE(0x0D) nn::Result GetThreadAffinityMask(u8* affinityMask, nn::Handle thread, s32 processorCount) SVC_ASM; // svc 0x0D
NN_SVC_INLINE(0x0E) nn::Result SetThreadAffinityMask(nn::Handle thread, const u8* affinityMask, s32 processorCount) SVC_ASM; // svc 0x0E
nn::Result GetThreadIdealProcessor(s32* processorId, nn::Handle thread) SVC_ASM; // svc 0x0F
NN_SVC_INLINE(0x10) nn::Result SetThreadIdealProcessor(nn::Handle thread, s32 processorId) SVC_ASM; // svc 0x10
NN_SVC_INLINE(0x11) s32 GetCurrentProcessorNumber() SVC_ASM; // svc 0x11
nn::Result Run(nn::Handle process, s32 priority, u32 stackSize, s32 argc, u16* argv, u16* envp) SVC_ASM; // svc 0x12 | loader only
nn::Result CreateMutex(nn::Handle* mutex, bool initialLocked) SVC_ASM; // svc 0x13 | 0x0014FB90 (name from reference symbols)
NN_SVC_INLINE(0x14) nn::Result ReleaseMutex(nn::Handle mutex) SVC_ASM; // svc 0x14
nn::Result CreateSemaphore(nn::Handle* semaphore, s32 initialCount, s32 maxCount) SVC_ASM; // svc 0x15
nn::Result ReleaseSemaphore(s32* count, nn::Handle semaphore, s32 releaseCount) SVC_ASM; // svc 0x16
nn::Result CreateEvent(nn::Handle* event, nn::os::ResetType resetType) SVC_ASM; // svc 0x17 | 0x0012840C (name from reference symbols)
NN_SVC_INLINE(0x18) nn::Result SignalEvent(nn::Handle event) SVC_ASM; // svc 0x18
NN_SVC_INLINE(0x19) nn::Result ClearEvent(nn::Handle event) SVC_ASM; // svc 0x19
nn::Result CreateTimer(nn::Handle* timer, nn::os::ResetType resetType) SVC_ASM; // svc 0x1A
nn::Result SetTimer(nn::Handle timer, s64 initial, s64 interval) SVC_ASM; // svc 0x1B
NN_SVC_INLINE(0x1C) nn::Result CancelTimer(nn::Handle timer) SVC_ASM; // svc 0x1C
NN_SVC_INLINE(0x1D) nn::Result ClearTimer(nn::Handle timer) SVC_ASM; // svc 0x1D
nn::Result CreateMemoryBlock(nn::Handle* memoryBlock, uptr addr, u32 size, u32 myPermission, u32 otherPermission) SVC_ASM; // svc 0x1E | 0x00135C8C (name from reference symbols)
NN_SVC_INLINE(0x1F) nn::Result MapMemoryBlock(nn::Handle memoryBlock, uptr addr, u32 myPermission, u32 otherPermission) SVC_ASM; // svc 0x1F
NN_SVC_INLINE(0x20) nn::Result UnmapMemoryBlock(nn::Handle memoryBlock, uptr addr) SVC_ASM; // svc 0x20
nn::Result CreateAddressArbiter(nn::Handle* arbiter) SVC_ASM; // svc 0x21 | 0x0011F428 (name from reference symbols)
nn::Result ArbitrateAddress(nn::Handle arbiter, uptr addr, nn::os::ArbitrationType type, s32 value, s64 timeout) SVC_ASM; // svc 0x22 | 0x00135C5C (name from reference symbols)
NN_SVC_INLINE(0x23) nn::Result CloseHandle(nn::Handle handle) SVC_ASM; // svc 0x23
NN_SVC_INLINE(0x24) nn::Result WaitSynchronization1(nn::Handle handle, s64 timeout) SVC_ASM; // svc 0x24 | 3dbrew: WaitSynchronization1
nn::Result WaitSynchronizationN(s32* index, const nn::Handle* handles, s32 handleCount, bool waitAll, s64 timeout) SVC_ASM; // svc 0x25 | 0x00128444 (name from reference symbols)
nn::Result DuplicateHandle(nn::Handle* out, nn::Handle original) SVC_ASM; // svc 0x27 | 0x0014FBA8 (name from reference symbols)
NN_SVC_INLINE(0x28) s64 GetSystemTick() SVC_ASM; // svc 0x28
nn::Result GetHandleInfo(s64* out, nn::Handle handle, u32 type) SVC_ASM; // svc 0x29 | 0x0014FBC0 (name inferred from the svc number)
nn::Result GetSystemInfo(s64* out, u32 type, s32 param) SVC_ASM; // svc 0x2A
nn::Result GetProcessInfo(s64* out, nn::Handle process, u32 type) SVC_ASM; // svc 0x2B | 0x00122BF8 (name inferred from the svc number)
nn::Result GetThreadInfo(s64* out, nn::Handle thread, u32 type) SVC_ASM; // svc 0x2C
nn::Result ConnectToPort(nn::Handle* session, const char* portName) SVC_ASM; // svc 0x2D | 0x001283F4 (name from reference symbols)
NN_SVC_INLINE(0x32) nn::Result SendSyncRequest(nn::Handle session) SVC_ASM; // svc 0x32
nn::Result OpenProcess(nn::Handle* process, u32 processId) SVC_ASM; // svc 0x33
nn::Result OpenThread(nn::Handle* thread, nn::Handle process, u32 threadId) SVC_ASM; // svc 0x34
nn::Result GetProcessId(u32* processId, nn::Handle process) SVC_ASM; // svc 0x35 | 0x0012F3D8 (name from reference symbols)
nn::Result GetProcessIdOfThread(u32* processId, nn::Handle thread) SVC_ASM; // svc 0x36
nn::Result GetThreadId(u32* threadId, nn::Handle thread) SVC_ASM; // svc 0x37 | 0x0013A354 (name from reference symbols)
nn::Result GetResourceLimit(nn::Handle* resourceLimit, nn::Handle process) SVC_ASM; // svc 0x38 | 0x0011DC74 (name from reference symbols)
NN_SVC_INLINE(0x39) nn::Result GetResourceLimitLimitValues(s64* values, nn::Handle resourceLimit, const u32* names, s32 nameCount) SVC_ASM; // svc 0x39
NN_SVC_INLINE(0x3A) nn::Result GetResourceLimitCurrentValues(s64* values, nn::Handle resourceLimit, const u32* names, s32 nameCount) SVC_ASM; // svc 0x3A
NN_SVC_INLINE(0x3C) void Break(u32 reason, const void* data, u32 size) SVC_ASM; // svc 0x3C | data / size only for the CRO notifications
NN_SVC_INLINE(0x3D) nn::Result OutputDebugString(const char* string, s32 length) SVC_ASM; // svc 0x3D
nn::Result CreatePort(nn::Handle* serverPort, nn::Handle* clientPort, const char* name, s32 maxSessions) SVC_ASM; // svc 0x47
nn::Result CreateSessionToPort(nn::Handle* session, nn::Handle clientPort) SVC_ASM; // svc 0x48
nn::Result CreateSession(nn::Handle* serverSession, nn::Handle* clientSession) SVC_ASM; // svc 0x49
nn::Result AcceptSession(nn::Handle* session, nn::Handle serverPort) SVC_ASM; // svc 0x4A
nn::Result ReplyAndReceive(s32* index, const nn::Handle* handles, s32 handleCount, nn::Handle replyTarget) SVC_ASM; // svc 0x4F
NN_SVC_INLINE(0x50) nn::Result BindInterrupt(u32 interruptId, nn::Handle event, s32 priority, bool isManualClear) SVC_ASM; // svc 0x50
NN_SVC_INLINE(0x51) nn::Result UnbindInterrupt(u32 interruptId, nn::Handle event) SVC_ASM; // svc 0x51
NN_SVC_INLINE(0x52) nn::Result InvalidateProcessDataCache(nn::Handle process, uptr addr, u32 size) SVC_ASM; // svc 0x52
NN_SVC_INLINE(0x53) nn::Result StoreProcessDataCache(nn::Handle process, uptr addr, u32 size) SVC_ASM; // svc 0x53
NN_SVC_INLINE(0x54) nn::Result FlushProcessDataCache(nn::Handle process, uptr addr, u32 size) SVC_ASM; // svc 0x54
NN_SVC_INLINE(0x56) nn::Result StopDma(nn::Handle dma) SVC_ASM; // svc 0x56
NN_SVC_INLINE(0x57) nn::Result GetDmaState(void* state, nn::Handle dma) SVC_ASM; // svc 0x57
NN_SVC_INLINE(0x59) nn::Result SetGpuProt(bool useApplicationRestriction) SVC_ASM; // svc 0x59
NN_SVC_INLINE(0x5A) nn::Result SetWifiEnabled(bool enabled) SVC_ASM; // svc 0x5A
nn::Result DebugActiveProcess(nn::Handle* debug, u32 processId) SVC_ASM; // svc 0x60 | debug
NN_SVC_INLINE(0x61) nn::Result BreakDebugProcess(nn::Handle debug) SVC_ASM; // svc 0x61 | debug
NN_SVC_INLINE(0x62) nn::Result TerminateDebugProcess(nn::Handle debug) SVC_ASM; // svc 0x62 | debug
NN_SVC_INLINE(0x63) nn::Result GetProcessDebugEvent(void* info, nn::Handle debug) SVC_ASM; // svc 0x63 | debug
NN_SVC_INLINE(0x64) nn::Result ContinueDebugEvent(nn::Handle debug, u32 flags) SVC_ASM; // svc 0x64 | debug
nn::Result GetProcessList(s32* processCount, u32* processIds, s32 maxCount) SVC_ASM; // svc 0x65
nn::Result GetThreadList(s32* threadCount, u32* threadIds, s32 maxCount, nn::Handle process) SVC_ASM; // svc 0x66
NN_SVC_INLINE(0x67) nn::Result GetDebugThreadContext(void* context, nn::Handle debug, u32 threadId, u32 controlFlags) SVC_ASM; // svc 0x67 | debug
NN_SVC_INLINE(0x68) nn::Result SetDebugThreadContext(nn::Handle debug, u32 threadId, void* context, u32 controlFlags) SVC_ASM; // svc 0x68 | debug
nn::Result QueryDebugProcessMemory(MemoryInfo* info, PageInfo* page, nn::Handle debug, uptr addr) SVC_ASM; // svc 0x69 | debug
NN_SVC_INLINE(0x6A) nn::Result ReadProcessMemory(void* buffer, nn::Handle debug, uptr addr, u32 size) SVC_ASM; // svc 0x6A | debug
NN_SVC_INLINE(0x6B) nn::Result WriteProcessMemory(nn::Handle debug, const void* buffer, uptr addr, u32 size) SVC_ASM; // svc 0x6B | debug
NN_SVC_INLINE(0x6C) nn::Result SetHardwareBreakPoint(s32 registerId, u32 control, u32 value) SVC_ASM; // svc 0x6C | debug
nn::Result ControlProcessMemory(nn::Handle process, uptr addr0, uptr addr1, u32 size, u32 type, u32 permission) SVC_ASM; // svc 0x70
NN_SVC_INLINE(0x71) nn::Result MapProcessMemory(nn::Handle process, uptr destAddr, u32 size) SVC_ASM; // svc 0x71
NN_SVC_INLINE(0x72) nn::Result UnmapProcessMemory(nn::Handle process, uptr destAddr, u32 size) SVC_ASM; // svc 0x72
NN_SVC_INLINE(0x76) nn::Result TerminateProcess(nn::Handle process) SVC_ASM; // svc 0x76
NN_SVC_INLINE(0x77) nn::Result SetProcessResourceLimits(nn::Handle process, nn::Handle resourceLimit) SVC_ASM; // svc 0x77
nn::Result CreateResourceLimit(nn::Handle* resourceLimit) SVC_ASM; // svc 0x78
NN_SVC_INLINE(0x79) nn::Result SetResourceLimitValues(nn::Handle resourceLimit, const u32* names, const s64* values, s32 nameCount) SVC_ASM; // svc 0x79
NN_SVC_INLINE(0x7B) nn::Result Backdoor(s32 (*callback)()) SVC_ASM; // svc 0x7B | removed in later firmware
NN_SVC_INLINE(0x7C) nn::Result KernelSetState(u32 type, u32 param0, u32 param1, u32 param2) SVC_ASM; // svc 0x7C
nn::Result QueryProcessMemory(MemoryInfo* info, PageInfo* page, nn::Handle process, uptr addr) SVC_ASM; // svc 0x7D

} // namespace svc
} // namespace nn
