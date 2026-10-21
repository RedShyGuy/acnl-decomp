#pragma once

// Bodies of gx functions that the original also has inline in other functions (nngxInitialize
// contains copies of nngxCmdlistStorage, nngxRunCmdlist, nngxWaitCmdlistDone and nngxStopCmdlist).

#include <cstring>
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"

namespace nn {
namespace gx {
namespace detail {
// the run mode in which nngxRunCmdlist starts the execution
const GLint RUN_MODE_AUTO = 0x300;

DECOMP_ALWAYS_INLINE void CmdlistStorageInline(GLsizei bufferSize, GLsizei requestCount)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    if (pCmdlist->m_pBuffer != nullptr && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_CMDLIST, pCmdlist->m_Name, pCmdlist->m_pBuffer);
    }
    u8* pBuffer = s_Allocator.m_pAllocate != nullptr
                      ? static_cast<u8*>(s_Allocator.m_pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_CMDLIST, pCmdlist->m_Name,
                                                                 bufferSize + requestCount * sizeof(CmdlistRequest)))
                      : nullptr;
    pCmdlist->m_BufferSize = bufferSize;
    pCmdlist->m_UsedSize = 0;
    pCmdlist->m_RequestCount = 0;
    pCmdlist->m_ExecutedSize = 0;
    pCmdlist->m_CurrentRequest = 0;
    pCmdlist->m_IssuedRequestCount = 0;
    pCmdlist->m_pSubroutine = nullptr;
    pCmdlist->m_pBuffer = pBuffer;
    pCmdlist->m_SubroutineSize = 0;
    pCmdlist->m_pRequests = reinterpret_cast<CmdlistRequest*>(pBuffer + bufferSize);
    pCmdlist->m_MaxRequestCount = requestCount;
    memset(pCmdlist->m_pRequests, 0, requestCount * sizeof(CmdlistRequest));
    s_pCommandBufferCurrent = reinterpret_cast<u32*>(pCmdlist->m_pBuffer);
    s_pCommandBufferEnd = reinterpret_cast<u32*>(pCmdlist->m_pBuffer + pCmdlist->m_BufferSize);
}

DECOMP_ALWAYS_INLINE void RunCmdlistInline()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr || s_Context.m_IsRunning) {
        return;
    }
    s_Context.m_pRunningCmdlist = pCmdlist;
    s_Context.m_IsRunRequested = true;
    if (pCmdlist->m_CurrentRequest >= pCmdlist->m_RequestCount) {
        return;
    }
    s_Context.m_IsRunning = true;
    if (pCmdlist->m_RunMode != RUN_MODE_AUTO) {
        return;
    }
    nngxlowLock();
    ExecuteRequests();
    nngxlowUnlock();
}

DECOMP_ALWAYS_INLINE void WaitCmdlistDoneInline()
{
    s64 start = 0;
    if (s_Context.m_Timeout != 0) {
        start = nngxlowGetSystemTick();
    }
    while (s_Context.m_IsRunning) {
        if (s_Context.m_Timeout != 0 && s_Context.m_Timeout - (nngxlowGetSystemTick() - start) < 0) {
            if (s_Context.m_pTimeoutCallback != nullptr) {
                s_Context.m_pTimeoutCallback();
            }
            return;
        }
        nngxlowYieldThread();
    }
}

DECOMP_ALWAYS_INLINE void StopCmdlistInline()
{
    Cmdlist* pCmdlist = s_Context.m_pRunningCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    nngxlowLock();
    if (!s_Context.m_IsRunning) {
        s_Context.m_IsRunRequested = false;
    } else {
        // the execution stops after the last request given to the GPU
        pCmdlist->m_pRequests[pCmdlist->m_IssuedRequestCount - 1].m_IsStopPoint = true;
        s_Context.m_IsStopRequested = true;
    }
    nngxlowUnlock();
}
} // namespace detail
} // namespace gx
} // namespace nn
