#include "cfl/cfl_Api.h"
#include "cfl/cfl_Database.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/fnd/fnd_DateTime.h"

#include <cstring>

// the state of CFL (one group of globals in the original; names are ours)
// 0x00976294
bool s_HDMode = false;
// 0x00976295
bool s_RandomInitialized = false;
// 0x00976298
s32 s_AsyncStatus = 0;
// 0x0097629C
s32 s_LastReason = 0;
// 0x009762A0
u32 s_Flags = 0;
// 0x009762A4
CFLiManager* s_pManager = NULL;

// 0x00AE5E20
u64 s_UserID;
// 0x00AE5E28
CFLiRandom s_Random;

namespace {
// the heap does not need a second buffer (constant in the original; name is ours)
inline bool IsSingleBuffered()
{
    return true;
}

// the resource loader, or NULL while CFL is not initialized (inline; name is ours)
inline CFLiResLoader* GetResLoader()
{
    return CFL_IsAvailable() ? &CFLi_GetManager()->resLoader : NULL;
}

// a finished operation sets the status of the manager (inline; name is ours)
inline s32 GetStatus()
{
    CFLiManager* manager = s_pManager;
    if (manager == NULL) {
        return s_AsyncStatus;
    }
    if (CFLi_GetWorking()) {
        return CFL_RESULT_BUSY;
    }
    if ((s_pManager != NULL ? s_pManager->flags : s_Flags) & 4) {
        return CFL_RESULT_BROKEN;
    }
    if ((s_pManager != NULL ? s_pManager->flags : s_Flags) & 8) {
        return CFL_RESULT_BROKEN;
    }
    return s_pManager->status;
}
} // namespace

extern "C" {
// 0x0013DDCC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFL_IsAvailable()
{
    return s_pManager != NULL;
}

// 0x001555C0 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE bool CFLi_IsHDModeEnabled()
{
    return s_HDMode;
}

// 0x00153468 | nintendogs:bytes [tier B]
DECOMP_NOINLINE u32 CFL_GetWorkSize(bool hd)
{
    u32 iconSize;
    if (s_HDMode) {
        iconSize = hd ? 1024 : 512;
    } else {
        iconSize = hd ? 256 : 128;
    }
    u32 size = iconSize * iconSize * 3;
    if (!IsSingleBuffered()) {
        size *= 2;
    }
    return size + 0x40000 + sizeof(CFLiManager);
}

// 0x0013A4F4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE DECOMP_NOIPA bool CFLi_GetWorking()
{
    if (!CFL_IsAvailable()) {
        return false;
    }
    return CFLi_GetManager()->working;
}

// 0x00156D70 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_SetWorking(bool working)
{
    CFLi_GetManager()->working = working;
}

// ends the asynchronous operation; the first result stays
// 0x00154660 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_EndWorkingReason(s32 status, s32 reason)
{
    CFLi_GetManager()->working = false;
    s32 current = CFLi_GetManager()->status;
    if (current == CFL_RESULT_SUCCESS || current == CFL_RESULT_BUSY) {
        CFLi_GetManager()->status = status;
        CFLi_GetManager()->reason = reason;
    }
}

// 0x0013A4C4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFLi_GetFileBroken(int file)
{
    u32 flags = s_pManager != NULL ? s_pManager->flags : s_Flags;
    return (flags & (1 << file)) != 0;
}

// 0x00156984 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_SetFileBroken(int file)
{
    CFLiManager* manager = s_pManager;
    if (manager != NULL) {
        manager->flags |= 1 << file;
    }
}

// 0x001569B0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFLi_SetIconDrawDoneCallback(CFLiDrawDoneCallback callback)
{
    CFLi_GetManager()->iconDrawDoneCallback = callback != NULL ? callback : CFLi_DrawDoneDefault;
}

// 0x001569D0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFLi_SetModelDrawDoneCallback(CFLiDrawDoneCallback callback)
{
    CFLi_GetManager()->modelDrawDoneCallback = callback;
}

// 0x00135D24 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFL_IsResourceCached()
{
    bool cached = false;
    if (CFL_IsAvailable()) {
        CFLiResLoader* loader = GetResLoader();
        if (loader != NULL) {
            cached = loader->cached;
        }
    }
    return cached;
}

// 0x00135CD0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFL_FreeCachedResource()
{
    bool failed = true;
    if (CFL_IsAvailable() && !CFLi_GetWorking()) {
        CFLiResLoader* loader = GetResLoader();
        if (loader != NULL) {
            loader->cached = false;
            loader->size = 0;
            failed = false;
            loader->data = NULL;
        }
    }
    return failed;
}

// 0x00153658 | nintendogs:callseq [confirmed by mk7dlp] [tier A]
s32 CFL_InitRes(void* work, const void* res, u32 resSize, bool hd)
{
    if ((res == NULL || resSize == 0) && (res != NULL || resSize != 0)) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (work == NULL) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    CFLi_IsHDModeEnabled();
    if (CFL_IsAvailable()) {
        return CFL_RESULT_SUCCESS;
    }
    u32 size;
    if (res != NULL) {
        size = CFL_GetWorkSize(hd);
    } else {
        size = CFLi_GetWorkSizeWithoutRes();
    }
    CFLi_Initialize(work, size);
    CFLi_GetManager()->_C1 = hd;
    if (res != NULL) {
        CFLiResLoader* loader = GetResLoader();
        loader->cached = true;
        loader->data = static_cast<const u8*>(res);
        loader->size = resSize;
    }
    s32 result = CFLi_BootLoadDatabaseAsync(CFLi_BootLoadDatabaseCallback);
    if (result == CFL_RESULT_4 || result == CFL_RESULT_5 || result == CFL_RESULT_FILE_ERROR) {
        result = CFL_RESULT_SUCCESS;
    } else if (result == CFL_RESULT_SUCCESS) {
        return CFL_RESULT_SUCCESS;
    } else if (result != CFL_RESULT_BUSY) {
        if (result == CFL_RESULT_9) {
            return result;
        }
        CFL_Exit();
    }
    if (result == CFL_RESULT_BUSY) {
        return CFL_WaitAsync();
    }
    return result;
}

// 0x0012F730 | nintendogs:bytes-fuzzy [tier B]
DECOMP_NOINLINE void CFL_Exit()
{
    CFLi_Exit(false);
}

// 0x0012F738 (name is ours)
void CFLi_Exit(bool keepDatabase)
{
    if (s_pManager == NULL) {
        return;
    }
    CFL_WaitAsync();
    CFLiManager* manager = s_pManager;
    s_AsyncStatus = CFL_GetAsyncStatus();
    s_LastReason = CFL_GetLastReason();
    s_Flags = manager->flags;
    if (CFL_IsResourceCached()) {
        CFL_FreeCachedResource();
    }
    CFLi_FinalizeResourceDevice();
    CFLi_FinalizeDatabase(keepDatabase);
    CFLi_UnmountNandContent();
    CFLiHeap* heap = manager->heap;
    heap->Invalidate();
    heap->Finalize();
    s_pManager = NULL;
}
} // extern "C"

// 0x0013DEDC | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE CFLiManager* CFLi_GetManager()
{
    return s_pManager;
}

// 0x0013DEEC (name is ours)
DECOMP_NOINLINE void* CFLi_AllocateMemory(u32 size, s32 alignment)
{
    return s_pManager->heap->Allocate(size, alignment);
}

// 0x0013DEBC | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_FreeMemory(void* ptr)
{
    if (ptr != NULL) {
        s_pManager->heap->Free(ptr);
    }
}

// 0x0013AA80 | nintendogs:callseq-callee [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFL_GetAsyncStatus()
{
    return GetStatus();
}

// 0x0013A8E0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFL_WaitAsync()
{
    s32 status;
    do {
        status = GetStatus();
    } while (status == CFL_RESULT_BUSY);
    return status;
}

// 0x0016E244 | nintendogs:bytes [tier B]
DECOMP_NOINLINE s32 CFL_GetLastReason()
{
    CFLiManager* manager = s_pManager;
    return manager != NULL ? manager->reason : s_LastReason;
}

// 0x001726D4 (name is ours)
bool CFLi_Get_C1()
{
    CFLiManager* manager = s_pManager;
    return manager != NULL ? manager->_C1 : false;
}

// 0x00175A9C (name is ours)
DECOMP_NOINLINE u32 CFLi_GetWorkSizeWithoutRes()
{
    u32 iconSize = s_HDMode ? 512 : 128;
    u32 size = iconSize * iconSize * 3;
    if (!IsSingleBuffered()) {
        size *= 2;
    }
    return size + 0x34000 + sizeof(CFLiManager);
}

// 0x0017D234 (name is ours)
void CFLi_Initialize(void* work, u32 size)
{
    if (!s_RandomInitialized) {
        s_RandomInitialized = true;
        s_UserID = 0;
        s_UserID = nn::cfg::CTR::GetTransferableId();
        nn::fnd::DateTimeParameters now = nn::fnd::DateTime::GetNow().GetParameters();
        int days = nn::fnd::DateTime::DateToDays(now.year, now.month, now.day);
        s_Random.increment = 0x269EC3;
        s_Random.value = ((days * 24 + now.hour) * 60 + now.minute) * 60 + now.second;
        s_Random.multiplier = 0x5D588B65;
    }
    s_LastReason = 0;
    s_AsyncStatus = 1;
    s_Flags = 0;

    // the manager at the start of the buffer, aligned to 32
    uptr start = reinterpret_cast<uptr>(work);
    uptr padding = -start & 31;
    CFLiManager* manager = reinterpret_cast<CFLiManager*>(start + padding);
    work = reinterpret_cast<void*>(start + padding + sizeof(CFLiManager));
    size -= padding + sizeof(CFLiManager);
    std::memset(manager, 0, sizeof(CFLiManager));
    std::memset(work, 0, size);
    manager->status = 0;
    manager->reason = 0;
    manager->savedStatus = 0;
    manager->savedReason = 0;
    manager->flags = 0;
    s_pManager = manager;
    CFLi_SetIconDrawDoneCallback(NULL);
    CFLi_SetModelDrawDoneCallback(NULL);
    CFLi_SetWorking(false);
    CFLi_InitializeResLoader();
    CFLi_InitializeResourceDevice(&work, &size);
    CFLi_InitializeDatabase(&work, &size);
    CFLi_InitializeModelAxis();

    // the rest is the heap
    start = reinterpret_cast<uptr>(work);
    padding = -start & 31;
    u32 heapSize = size - 32;
    void* heapStart = reinterpret_cast<void*>(start + padding);
    size -= padding + heapSize;
    work = reinterpret_cast<void*>(start + padding + heapSize);
    std::memset(heapStart, 0, heapSize);
    static CFLiHeap s_Heap; // 0x00AE5DC0 (guard 0x009762A8)
    s_Heap.Initialize(reinterpret_cast<uptr>(heapStart), heapSize, 0x100);
    manager->heap = &s_Heap;
}
