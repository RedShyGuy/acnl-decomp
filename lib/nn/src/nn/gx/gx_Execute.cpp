#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"

// The execution of the command lists: the requests of the running list go to the GPU (at most
// m_MaxSpeculativeRequests ahead of the finished ones), the interrupt handlers finish them and
// count the VSyncs.

using nn::gx::detail::Cmdlist;
using nn::gx::detail::CmdlistRequest;
using nn::gx::detail::s_Context;

namespace {
// the request types (3dbrew GX commands)
const u8 REQUEST_DMA = 0;
const u8 REQUEST_3D_COMMANDS = 1;
const u8 REQUEST_MEMORY_FILL = 2;
const u8 REQUEST_DISPLAY_TRANSFER = 3;
const u8 REQUEST_TEXTURE_COPY = 4;
const u32 REQUEST_FLAG_SET = 1 << 0;            // 3D commands
const u32 REQUEST_FLAG_EXTERNAL = 1 << 1;       // 3D commands: outside the command buffer
const u32 REQUEST_FLAG_FLUSHED = 1 << 2;        // 3D commands: the cache is flushed already
const u32 REQUEST_FLAG_NO_CACHE_FLUSH = 1 << 0; // DMA
// the start bit of the control word of a memory fill (the fill width is in bits 8-9)
const u32 FILL_START = 1;
// the texture copy of the GPU (the flags of SetTextureCopy; names are ours)
const u32 COPY_WITH_GAPS = 4;
const u32 COPY_TEXTURE = 8;
// the displays of the VSync callbacks and the counters (up to 0x7FFFFFFE)
const GLenum DISPLAY_0 = 0x400;
const GLenum DISPLAY_1 = 0x401;
const s32 VSYNC_COUNT_LIMIT = 0x7FFFFFFF;

inline u16 Low(u32 value)
{
    return value & 0xFFFF;
}
inline u16 High(u32 value)
{
    return value >> 16;
}

// the flags of SetDisplayTransfer from the mode of the request (bit 2: the destination is smaller
// than the source)
inline u32 MakeTransferFlags(const CmdlistRequest* pRequest)
{
    u32 flags = 0;
    flags |= pRequest->m_Transfer.m_IsFlipped;
    if (pRequest->m_Transfer.m_DestinationWidth < pRequest->m_Transfer.m_SourceWidth ||
        pRequest->m_Transfer.m_DestinationHeight < pRequest->m_Transfer.m_SourceHeight) {
        flags |= 4;
    } else {
        flags &= ~4;
    }
    flags = (flags & ~0x700) | (pRequest->m_Transfer.m_InputFormat << 8);
    flags = (flags & ~0x7000) | (pRequest->m_Transfer.m_OutputFormat << 12);
    flags = (flags & ~0x3000000) | (pRequest->m_Transfer.m_Scaling << 24);
    flags = (flags & ~0x10000) | (pRequest->m_Transfer.m_IsBlock32 << 16);
    flags = (flags & ~2) | (pRequest->m_Transfer.m_IsLinearOutput << 1);
    flags = (flags & ~0x20) | (pRequest->m_Transfer.m_IsTiledOutput << 5);
    return flags;
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x0012E7EC (name is ours)
void ExecuteRequests()
{
    if (s_Context.m_IsStopRequested == 1) {
        return;
    }
    Cmdlist* pCmdlist = s_Context.m_pRunningCmdlist;
    s32 issued = pCmdlist->m_IssuedRequestCount;
    if (pCmdlist->m_RequestCount <= issued) {
        return;
    }
    do {
        if (s_Context.m_MaxSpeculativeRequests + pCmdlist->m_CurrentRequest <= issued) {
            return;
        }
        CmdlistRequest* pRequest = &pCmdlist->m_pRequests[issued];
        switch (pRequest->m_Type) {
        case REQUEST_DMA:
            nngxlowRequestDma(reinterpret_cast<void*>(pRequest->m_Dma.m_Destination), reinterpret_cast<const void*>(pRequest->m_Dma.m_Source),
                              pRequest->m_Dma.m_Size, (pRequest->m_Dma.m_Flags & REQUEST_FLAG_NO_CACHE_FLUSH) == 0);
            break;
        case REQUEST_3D_COMMANDS:
            nngxlowSetCommandlist(reinterpret_cast<void*>(pRequest->m_Commands.m_Address), pRequest->m_Commands.m_Size,
                                  (pRequest->m_Commands.m_Flags & REQUEST_FLAG_FLUSHED) == 0, (pRequest->m_Commands.m_Flags & REQUEST_FLAG_SET) != 0);
            break;
        case REQUEST_MEMORY_FILL:
            nngxlowSetMemoryFill(reinterpret_cast<void*>(pRequest->m_Fill.m_Start0), reinterpret_cast<void*>(pRequest->m_Fill.m_Start0 + pRequest->m_Fill.m_Size0),
                                 pRequest->m_Fill.m_Value0, (pRequest->m_Fill.m_Width0 << 8) | FILL_START, reinterpret_cast<void*>(pRequest->m_Fill.m_Start1),
                                 reinterpret_cast<void*>(pRequest->m_Fill.m_Start1 + pRequest->m_Fill.m_Size1), pRequest->m_Fill.m_Value1,
                                 (pRequest->m_Fill.m_Width1 << 8) | FILL_START);
            break;
        case REQUEST_DISPLAY_TRANSFER:
            nngxlowSetDisplayTransfer(reinterpret_cast<void*>(pRequest->m_Transfer.m_Source), pRequest->m_Transfer.m_SourceWidth,
                                      pRequest->m_Transfer.m_SourceHeight, reinterpret_cast<void*>(pRequest->m_Transfer.m_Destination),
                                      pRequest->m_Transfer.m_DestinationWidth, pRequest->m_Transfer.m_DestinationHeight, MakeTransferFlags(pRequest));
            break;
        case REQUEST_TEXTURE_COPY: {
            u32 flags = Low(pRequest->m_Copy.m_SourceLine) != 0 || Low(pRequest->m_Copy.m_DestinationLine) != 0 ? COPY_WITH_GAPS : 0;
            nngxlowSetTextureCopy(reinterpret_cast<void*>(pRequest->m_Copy.m_Source), reinterpret_cast<void*>(pRequest->m_Copy.m_Destination),
                                  pRequest->m_Copy.m_Size, High(pRequest->m_Copy.m_SourceLine), Low(pRequest->m_Copy.m_SourceLine),
                                  High(pRequest->m_Copy.m_DestinationLine), Low(pRequest->m_Copy.m_DestinationLine), flags | COPY_TEXTURE);
            break;
        }
        default:
            break;
        }
        issued = ++pCmdlist->m_IssuedRequestCount;
        if (pRequest->m_IsStopPoint == 1) {
            s_Context.m_IsStopRequested = true;
            return;
        }
    } while (pCmdlist->m_RequestCount > issued);
}

// 0x0012E794 (name is ours)
void FinishSubroutine(Cmdlist* pCmdlist, CmdlistRequest* pRequest)
{
    FinishSubroutineInline(pCmdlist, pRequest);
}

// a request of the running list is done: the callback of the list, then the next requests (or
// the end / the stop of the execution)
// 0x0012E6C8 (name is ours)
DECOMP_NOINLINE void FinishRequest()
{
    Cmdlist* pCmdlist = s_Context.m_pRunningCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest];
    void (*pCallback)(GLint) = nullptr;
    GLint finished;
    nngxlowLock();
    s32 current = ++pCmdlist->m_CurrentRequest;
    if (pCmdlist->m_pCallback != nullptr &&
        (pRequest->m_IsCallbackEnabled || (current == pCmdlist->m_RequestCount && pCmdlist->m_IsCallbackEnabledAll))) {
        pCallback = pCmdlist->m_pCallback;
        finished = current;
    }
    if (pRequest->m_IsStopPoint) {
        s_Context.m_IsRunRequested = false;
        s_Context.m_IsStopRequested = false;
        s_Context.m_IsRunning = false;
    } else if (current >= pCmdlist->m_RequestCount) {
        s_Context.m_IsRunning = false;
    } else {
        ExecuteRequests();
    }
    nngxlowUnlock();
    if (pCallback != nullptr) {
        pCallback(finished);
    }
}

// the interrupt handlers (registered by nngxInitialize; names are ours)
// 0x00128348 (name is ours)
void OnPsc0Interrupt()
{
    FinishRequest();
}

// 0x0012834C (name is ours)
void OnPsc1Interrupt()
{
    FinishRequest();
}

// 0x00128344 (name is ours)
void OnPpfInterrupt()
{
    FinishRequest();
}

// (a nop that falls into FinishRequest)
// 0x0012E6C4 (name is ours)
void OnDmaInterrupt()
{
    FinishRequest();
}

// the 3D commands of the request are done
// 0x00128278 (name is ours)
void OnP3dInterrupt()
{
    Cmdlist* pCmdlist = s_Context.m_pRunningCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest];
    if ((pRequest->m_Commands.m_Flags & REQUEST_FLAG_EXTERNAL) == 0) {
        pCmdlist->m_ExecutedSize += pRequest->m_Commands.m_Size;
    }
    FinishRequest();
}

// 0x001282B8 (name is ours)
void OnVBlankTopInterrupt()
{
    if (s_Context.m_pVSyncCallbacks[0] != nullptr) {
        s_Context.m_pVSyncCallbacks[0](DISPLAY_0);
    }
    s_Context.m_VSyncCounts[0] = (s_Context.m_VSyncCounts[0] + 1) % VSYNC_COUNT_LIMIT;
}

// 0x001282FC (name is ours)
void OnVBlankBottomInterrupt()
{
    if (s_Context.m_pVSyncCallbacks[1] != nullptr) {
        s_Context.m_pVSyncCallbacks[1](DISPLAY_1);
    }
    s_Context.m_VSyncCounts[1] = (s_Context.m_VSyncCounts[1] + 1) % VSYNC_COUNT_LIMIT;
}
} // namespace detail
} // namespace gx
} // namespace nn
