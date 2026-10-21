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

// 1.7.16 float (the float24 of the GPU)
inline u32 Float32ToFloat24(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    const u32 sign = bits.u >> 31;
    s32 exponent = 0;
    if ((bits.u & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits.u << 1) >> 24) - 64;
    }
    if (exponent < 0) {
        return sign << 23;
    }
    return ((bits.u << 9) >> 16) | (exponent << 16) | (sign << 23);
}

// four floats as three words of float24 (the constant value of a vertex attribute)
inline void PackFloat24(u32* pPacked, const f32* pValues)
{
    u32 x = Float32ToFloat24(pValues[0]);
    u32 y = Float32ToFloat24(pValues[1]);
    u32 z = Float32ToFloat24(pValues[2]);
    u32 w = Float32ToFloat24(pValues[3]);
    pPacked[0] = (z >> 16) | (w << 8);
    pPacked[1] = ((y << 8) >> 16) | (z << 16);
    pPacked[2] = x | (y << 24);
}

// a command to the end of the buffer (if it fits; header: register | mask << 16 | extra values << 20
// | consecutive << 31 after 3dbrew)
inline void PutCommand(u32 value, u32 header)
{
    u32* p = s_pCommandBufferCurrent;
    if (p < s_pCommandBufferEnd) {
        p[0] = value;
        p[1] = header;
        s_pCommandBufferCurrent = p + 2;
    }
}

// the request becomes visible to the execution, which starts if the list waits for it
DECOMP_ALWAYS_INLINE void AddRequestInline(Cmdlist* pCmdlist, s32 count = 1)
{
    nngxlowLock();
    pCmdlist->m_RequestCount += count;
    if (s_Context.m_pRunningCmdlist == pCmdlist && s_Context.m_IsRunRequested && !s_Context.m_IsRunning) {
        s_Context.m_IsRunning = true;
        ExecuteRequests();
    }
    nngxlowUnlock();
}

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
