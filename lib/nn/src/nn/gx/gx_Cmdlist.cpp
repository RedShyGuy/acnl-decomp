#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include <cstring>

// The command lists of the gx library: named objects in a hash table, a command buffer with the
// requests behind it, and the functions that build and run them.

using nn::gx::detail::AddRequestInline;
using nn::gx::detail::Cmdlist;
using nn::gx::detail::PutCommand;
using nn::gx::detail::CmdlistRequest;
using nn::gx::detail::s_Allocator;
using nn::gx::detail::s_Context;
using nn::gx::detail::s_pCommandBufferCurrent;
using nn::gx::detail::s_pCommandBufferEnd;

namespace {
// the parameters of nngxGetCmdlistParameteri / nngxSetCmdlistParameteri (names are ours)
const GLenum CMDLIST_RUN_MODE = 0x200;
const GLenum CMDLIST_IS_RUNNING = 0x201;
const GLenum CMDLIST_USED_SIZE = 0x202;
const GLenum CMDLIST_REQUEST_COUNT = 0x203;
const GLenum CMDLIST_BUFFER_SIZE = 0x204;
const GLenum CMDLIST_MAX_REQUEST_COUNT = 0x205;
const GLenum CMDLIST_BUFFER_ADDRESS = 0x206;
const GLenum CMDLIST_NAME = 0x207;
const GLenum CMDLIST_CURRENT_ADDRESS = 0x208;
const GLenum CMDLIST_EXECUTED_SIZE = 0x209;
const GLenum CMDLIST_CURRENT_REQUEST = 0x20A;
const GLenum CMDLIST_REQUEST_ADDRESS = 0x20B;
const GLenum CMDLIST_CURRENT_REQUEST_TYPE = 0x20C;
const GLenum CMDLIST_CURRENT_3D_COMMANDS = 0x20D;
const GLenum CMDLIST_GPU_STATUS = 0x20E;
const GLenum CMDLIST_REQUEST_FLAG = 0x20F;

// the types of the requests as nngxGetCmdlistParameteri returns them (the request types 0-4 in the
// order of the gxlow commands: DMA, 3D commands, memory fill, display transfer, texture copy)
const GLint REQUEST_TYPE_DMA = 0x310;
const GLint REQUEST_TYPE_3D_COMMANDS = 0x311;
const GLint REQUEST_TYPE_MEMORY_FILL = 0x312;
const GLint REQUEST_TYPE_DISPLAY_TRANSFER = 0x313;
const GLint REQUEST_TYPE_TEXTURE_COPY = 0x314;
// the request types (3dbrew GX commands) and flags
const u8 REQUEST_DMA = 0;
const u8 REQUEST_3D_COMMANDS = 1;
const u32 REQUEST_FLAG_SET = 1 << 0;            // 3D commands: nngxSetCmdlistParameteri 0x20F
const u32 REQUEST_FLAG_EXTERNAL = 1 << 1;       // 3D commands: outside the command buffer
const u32 REQUEST_FLAG_NO_CACHE_FLUSH = 1 << 0; // DMA
// the displays of nngxCheckVSync and the largest count (names are ours)
const GLenum VSYNC_DISPLAY_0 = 0x400;
const GLenum VSYNC_DISPLAY_1 = 0x401;
const GLenum VSYNC_DISPLAY_BOTH = 0x402;
const s32 VSYNC_COUNT_MAX = 0x7FFFFFFE;
// a command that does nothing (it aligns the buffer to 16 bytes); the end of the 3D commands
const u32 DUMMY_COMMAND_HEADER = 0x8025E;
const u32 SPLIT_COMMAND_HEADER_0 = 0xF0111;
const u32 SPLIT_COMMAND_HEADER_1 = 0xF0110;
const u32 SPLIT_COMMAND_HEADER_2 = 0xF0010;
const u32 SPLIT_COMMAND_VALUE = 0x12345678;
// the register of the GPU status (nngxGetCmdlistParameteri 0x20E)
const u32 GPU_STATUS_REGISTER = 0x400058;

// the list of the hash table with the name
inline Cmdlist* FindCmdlist(GLuint name)
{
    Cmdlist* p = s_Context.m_pCmdlists[name % nn::gx::detail::BUCKET_COUNT];
    while (p != nullptr && p->m_Name != name) {
        p = p->m_pNext;
    }
    return p;
}

// a new list with the name in the hash table (sorted); false if the name is taken
inline bool InsertCmdlist(Cmdlist* pCmdlist, GLuint name)
{
    Cmdlist** ppBucket = &s_Context.m_pCmdlists[name % nn::gx::detail::BUCKET_COUNT];
    Cmdlist* pHead = *ppBucket;
    if (pHead == nullptr) {
        pCmdlist->m_Name = name;
        pCmdlist->m_pNext = nullptr;
        *ppBucket = pCmdlist;
        return true;
    }
    if (pHead->m_Name == name) {
        return false;
    }
    if (pHead->m_Name > name) {
        pCmdlist->m_Name = name;
        pCmdlist->m_pNext = pHead;
        *ppBucket = pCmdlist;
        return true;
    }
    Cmdlist* pPrev = pHead;
    for (Cmdlist* p = pHead->m_pNext; p != nullptr; p = p->m_pNext) {
        if (p->m_Name == name) {
            return false;
        }
        if (p->m_Name > name) {
            pCmdlist->m_Name = name;
            pPrev->m_pNext = pCmdlist;
            pCmdlist->m_pNext = p;
            return true;
        }
        pPrev = p;
    }
    pPrev->m_pNext = pCmdlist;
    pCmdlist->m_Name = name;
    pCmdlist->m_pNext = nullptr;
    return true;
}

// a new list object (not in the hash table yet)
inline Cmdlist* CreateCmdlist()
{
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    Cmdlist* p = pAllocate != nullptr ? static_cast<Cmdlist*>(pAllocate(nn::gx::detail::MEMORY_AREA_MAIN, nn::gx::detail::MEMORY_PURPOSE_OBJECT, 0, sizeof(Cmdlist)))
                                      : nullptr;
    memset(p, 0, sizeof(Cmdlist));
    p->m_RunMode = nn::gx::detail::RUN_MODE_AUTO;
    return p;
}

// writes the used size of the bound list and pads its buffer to 16 bytes
inline void AlignCommandBuffer()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    u32* pCurrent = s_pCommandBufferCurrent;
    u32 used = reinterpret_cast<u8*>(pCurrent) - pCmdlist->m_pBuffer;
    pCmdlist->m_UsedSize = used;
    if ((used & 8) != 0 && pCurrent < s_pCommandBufferEnd) {
        pCurrent[0] = 0;
        pCurrent[1] = DUMMY_COMMAND_HEADER;
        s_pCommandBufferCurrent = pCurrent + 2;
    }
}

// the end of the 3D commands of a request: three commands, before them a dummy command that
// keeps the size a multiple of 16 (m_UsedSize is the size without them)
DECOMP_ALWAYS_INLINE void FinishCommandsInline(Cmdlist* pCmdlist)
{
    u32* pCurrent = s_pCommandBufferCurrent;
    u32 used = pCmdlist->m_UsedSize;
    if ((used & 8) != 0) {
        pCmdlist->m_UsedSize = used + 3 * 8;
    } else {
        pCmdlist->m_UsedSize = used + 4 * 8;
        if (pCurrent < s_pCommandBufferEnd) {
            pCurrent[0] = 0;
            pCurrent[1] = DUMMY_COMMAND_HEADER;
            s_pCommandBufferCurrent = pCurrent + 2;
        }
    }
    PutCommand(1, SPLIT_COMMAND_HEADER_0);
    PutCommand(1, SPLIT_COMMAND_HEADER_1);
    PutCommand(SPLIT_COMMAND_VALUE, SPLIT_COMMAND_HEADER_2);
}

// the commands since the last request as a request
DECOMP_ALWAYS_INLINE void FlushCommandsInline(Cmdlist* pCmdlist)
{
    FinishCommandsInline(pCmdlist);
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_3D_COMMANDS;
    pRequest->m_Commands.m_Address = reinterpret_cast<u32>(pCmdlist->m_pBuffer + pCmdlist->m_FlushedSize);
    pRequest->m_Commands.m_Size = pCmdlist->m_UsedSize - pCmdlist->m_FlushedSize;
    if (pCmdlist->m_pSubroutine != nullptr) {
        nn::gx::detail::FinishSubroutine(pCmdlist, pRequest);
    }
    pRequest->m_Commands.m_Flags &= ~REQUEST_FLAG_EXTERNAL;
    if (pCmdlist->m_IsRequestFlagSet) {
        pRequest->m_Commands.m_Flags |= REQUEST_FLAG_SET;
    }
    pCmdlist->m_FlushedSize = pCmdlist->m_UsedSize;
    AddRequestInline(pCmdlist);
}
} // namespace

// 0x00121B48 | nintendogs:bytes [tier A]
extern "C" void nngxGenCmdlists(GLsizei n, GLuint* pNames)
{
    for (GLsizei i = 0; i < n; i++) {
        Cmdlist* pCmdlist = CreateCmdlist();
        GLuint name = s_Allocator.m_NextCmdlistName;
        while (!InsertCmdlist(pCmdlist, name)) {
            name++;
        }
        pNames[i] = name;
        s_Allocator.m_NextCmdlistName = name + 1;
    }
}

// 0x001216D0 | nintendogs:bytes [tier A]
extern "C" void nngxBindCmdlist(GLuint name)
{
    Cmdlist* pCmdlist = FindCmdlist(name);
    if (pCmdlist == nullptr && name != 0) {
        pCmdlist = CreateCmdlist();
        InsertCmdlist(pCmdlist, name);
    }
    Cmdlist* pOld = s_Context.m_pBoundCmdlist;
    if (pOld != nullptr) {
        pOld->m_UsedSize = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pOld->m_pBuffer;
    }
    s_Context.m_pBoundCmdlist = pCmdlist;
    if (pCmdlist != nullptr && pCmdlist->m_pBuffer != nullptr) {
        s_pCommandBufferCurrent = reinterpret_cast<u32*>(pCmdlist->m_pBuffer + pCmdlist->m_UsedSize);
        s_pCommandBufferEnd = reinterpret_cast<u32*>(pCmdlist->m_pBuffer + pCmdlist->m_BufferSize);
    } else {
        s_pCommandBufferEnd = nullptr;
        s_pCommandBufferCurrent = nullptr;
    }
}

// 0x0012199C | nintendogs:callgraph [tier A]
extern "C" void nngxCmdlistStorage(GLsizei bufferSize, GLsizei requestCount)
{
    nn::gx::detail::CmdlistStorageInline(bufferSize, requestCount);
}

// 0x00127DAC | nintendogs:bytes [tier A]
extern "C" void nngxDeleteCmdlists(GLsizei n, const GLuint* pNames)
{
    for (GLsizei i = 0; i < n; i++) {
        GLuint name = pNames[i];
        if (name == 0) {
            continue;
        }
        Cmdlist* pPrev = nullptr;
        Cmdlist* pCmdlist = s_Context.m_pCmdlists[name % nn::gx::detail::BUCKET_COUNT];
        while (pCmdlist != nullptr && name > pCmdlist->m_Name) {
            pPrev = pCmdlist;
            pCmdlist = pCmdlist->m_pNext;
        }
        if (pCmdlist == nullptr || pCmdlist->m_Name != name) {
            continue;
        }
        if (s_Context.m_pRunningCmdlist == pCmdlist) {
            s_Context.m_pRunningCmdlist = nullptr;
            s_Context.m_IsRunRequested = false;
        }
        if (s_Context.m_pBoundCmdlist == pCmdlist) {
            s_pCommandBufferEnd = nullptr;
            s_pCommandBufferCurrent = nullptr;
            s_Context.m_pBoundCmdlist = nullptr;
        }
        if (pCmdlist->m_pBuffer != nullptr && s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(nn::gx::detail::MEMORY_AREA_MAIN, nn::gx::detail::MEMORY_PURPOSE_CMDLIST, pCmdlist->m_Name, pCmdlist->m_pBuffer);
        }
        // the name can be given again
        if (pNames[i] < s_Allocator.m_NextCmdlistName) {
            s_Allocator.m_NextCmdlistName = pNames[i];
        }
        if (pPrev != nullptr) {
            pPrev->m_pNext = pCmdlist->m_pNext;
        } else {
            s_Context.m_pCmdlists[pNames[i] % nn::gx::detail::BUCKET_COUNT] = s_Context.m_pCmdlists[pNames[i] % nn::gx::detail::BUCKET_COUNT]->m_pNext;
        }
        if (s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(nn::gx::detail::MEMORY_AREA_MAIN, nn::gx::detail::MEMORY_PURPOSE_OBJECT, 0, pCmdlist);
        }
    }
}

// 0x00127EF0 | libgarden [tier A]
extern "C" void nngxGetCmdlistParameteri(GLenum name, GLint* pParameter)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    switch (name) {
    case CMDLIST_RUN_MODE:
        *pParameter = pCmdlist->m_RunMode;
        break;
    case CMDLIST_IS_RUNNING:
        *pParameter = s_Context.m_pRunningCmdlist == pCmdlist && s_Context.m_IsRunning ? 1 : 0;
        break;
    case CMDLIST_USED_SIZE: {
        u32 used = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
        pCmdlist->m_UsedSize = used;
        *pParameter = used;
        break;
    }
    case CMDLIST_REQUEST_COUNT:
        *pParameter = pCmdlist->m_RequestCount;
        break;
    case CMDLIST_BUFFER_SIZE:
        *pParameter = pCmdlist->m_BufferSize;
        break;
    case CMDLIST_MAX_REQUEST_COUNT:
        *pParameter = pCmdlist->m_MaxRequestCount;
        break;
    case CMDLIST_BUFFER_ADDRESS:
        *pParameter = reinterpret_cast<GLint>(pCmdlist->m_pBuffer);
        break;
    case CMDLIST_NAME:
        *pParameter = pCmdlist != nullptr ? pCmdlist->m_Name : 0;
        break;
    case CMDLIST_CURRENT_ADDRESS:
        *pParameter = reinterpret_cast<GLint>(s_pCommandBufferCurrent);
        break;
    case CMDLIST_EXECUTED_SIZE:
        *pParameter = pCmdlist->m_ExecutedSize;
        break;
    case CMDLIST_CURRENT_REQUEST:
        *pParameter = pCmdlist->m_CurrentRequest;
        break;
    case CMDLIST_REQUEST_ADDRESS:
        *pParameter = reinterpret_cast<GLint>(pCmdlist->m_pRequests);
        break;
    case CMDLIST_CURRENT_REQUEST_TYPE:
        if (pCmdlist->m_RequestCount > pCmdlist->m_CurrentRequest) {
            switch (pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest].m_Type) {
            case 0:
                *pParameter = REQUEST_TYPE_DMA;
                break;
            case 1:
                *pParameter = REQUEST_TYPE_3D_COMMANDS;
                break;
            case 2:
                *pParameter = REQUEST_TYPE_MEMORY_FILL;
                break;
            case 3:
                *pParameter = REQUEST_TYPE_DISPLAY_TRANSFER;
                break;
            case 4:
                *pParameter = REQUEST_TYPE_TEXTURE_COPY;
                break;
            default:
                break;
            }
        }
        break;
    case CMDLIST_CURRENT_3D_COMMANDS:
        if (pCmdlist->m_RequestCount > pCmdlist->m_CurrentRequest && pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest].m_Type == 1) {
            pParameter[0] = pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest].m_Commands.m_Address;
            pParameter[1] = pCmdlist->m_pRequests[pCmdlist->m_CurrentRequest].m_Commands.m_Size;
        }
        break;
    case CMDLIST_GPU_STATUS:
        GSPGPU_ReadHWRegs(GPU_STATUS_REGISTER, pParameter, sizeof(GLint));
        break;
    default:
        break;
    }
}

// 0x007B1DA0 | fefates:bytes [tier B]
extern "C" void nngxSetCmdlistParameteri(GLenum name, GLint parameter)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    if (name == CMDLIST_RUN_MODE) {
        pCmdlist->m_RunMode = parameter;
    } else if (name == CMDLIST_REQUEST_FLAG) {
        pCmdlist->m_IsRequestFlagSet = parameter != 0;
    }
}

// 0x001280C8 | nintendogs:bytes [tier A]
extern "C" void nngxMoveCommandbufferPointer(GLint offset)
{
    s_pCommandBufferCurrent = reinterpret_cast<u32*>(reinterpret_cast<u8*>(s_pCommandBufferCurrent) + offset);
}

// 0x0011F35C | nintendogs:bytes [tier B]
extern "C" void nngxEnableCmdlistCallback(GLint request)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    if (request == -1) {
        pCmdlist->m_IsCallbackEnabledAll = true;
    } else {
        pCmdlist->m_pRequests[request - 1].m_IsCallbackEnabled = true;
    }
}

// 0x0011F398 | nintendogs:bytes [tier B]
extern "C" void nngxSetCmdlistCallback(void (*pCallback)(GLint))
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist != nullptr) {
        pCmdlist->m_pCallback = pCallback;
    }
}

// 0x007B1D40 | nintendogs:bytes [tier A]
extern "C" void nngxRunCmdlist()
{
    nn::gx::detail::RunCmdlistInline();
}

// 0x007B1E9C | nintendogs:bytes [tier A]
extern "C" void nngxStopCmdlist()
{
    nn::gx::detail::StopCmdlistInline();
}

// 0x001280E0 | nintendogs:bytes [tier A]
extern "C" void nngxSplitDrawCmdlist()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    pCmdlist->m_UsedSize = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    FlushCommandsInline(pCmdlist);
}

// the commands since the last request as a request (if there are any)
// 0x007B1ABC | nintendogs:bytes [tier A]
extern "C" void nngxFlush3DCommand()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    pCmdlist->m_UsedSize = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    if (pCmdlist->m_FlushedSize == pCmdlist->m_UsedSize) {
        return;
    }
    FlushCommandsInline(pCmdlist);
}

// 3D commands outside the command buffer: copied into it, or a request of their own
// 0x007B1630 | nintendogs:bytes [tier B]
extern "C" void nngxAdd3DCommand(const GLvoid* pCommands, GLsizei size, GLboolean copy)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    u32 used = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    pCmdlist->m_UsedSize = used;
    if (copy) {
        memcpy(s_pCommandBufferCurrent, pCommands, size);
        s_pCommandBufferCurrent = reinterpret_cast<u32*>(reinterpret_cast<u8*>(s_pCommandBufferCurrent) + (size & ~3));
        return;
    }
    if (pCmdlist->m_FlushedSize != used) {
        nngxSplitDrawCmdlist();
    }
    if (pCmdlist->m_FlushedSize != pCmdlist->m_UsedSize) {
        return;
    }
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_3D_COMMANDS;
    pRequest->m_Commands.m_Size = size;
    pRequest->m_Commands.m_Address = reinterpret_cast<u32>(pCommands);
    pRequest->m_Commands.m_Flags |= REQUEST_FLAG_EXTERNAL;
    AddRequestInline(pCmdlist);
}

// 0x007B18A4 | nintendogs:callgraph [tier A]
extern "C" void nngxAddVramDmaCommand(const GLvoid* pSource, GLvoid* pDestination, GLsizei size)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_DMA;
    pRequest->m_Dma.m_Source = reinterpret_cast<u32>(pSource);
    pRequest->m_Dma.m_Destination = reinterpret_cast<u32>(pDestination);
    pRequest->m_Dma.m_Size = size;
    AddRequestInline(pCmdlist);
}

// 0x007B1918 | fefates:bytes [tier B]
extern "C" void nngxAddVramDmaCommandNoCacheFlush(const GLvoid* pSource, GLvoid* pDestination, GLsizei size)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_DMA;
    pRequest->m_Dma.m_Source = reinterpret_cast<u32>(pSource);
    pRequest->m_Dma.m_Destination = reinterpret_cast<u32>(pDestination);
    pRequest->m_Dma.m_Size = size;
    pRequest->m_Dma.m_Flags |= REQUEST_FLAG_NO_CACHE_FLUSH;
    AddRequestInline(pCmdlist);
}

// 0x007B175C | fefates:bytes [tier B]
extern "C" void nngxAddMemoryFillCommand(unsigned int address0, unsigned int size0, unsigned int value0, unsigned int width0,
                                         unsigned int address1, unsigned int size1, unsigned int value1, unsigned int width1)
{
    nn::gx::detail::AddMemoryFillRequest(s_Context.m_pBoundCmdlist, address0, size0, value0, width0, address1, size1, value1, width1);
}

// the requests and the commands of the bound list are dropped (the buffers stay)
// 0x007B1A2C | nintendogs:bytes [tier A]
extern "C" void nngxClearCmdlist()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    if (pCmdlist->m_pBuffer == nullptr && pCmdlist->m_pRequests == nullptr) {
        return;
    }
    memset(pCmdlist->m_pRequests, 0, pCmdlist->m_RequestCount * sizeof(CmdlistRequest));
    pCmdlist->m_UsedSize = 0;
    pCmdlist->m_ExecutedSize = 0;
    pCmdlist->m_RequestCount = 0;
    pCmdlist->m_CurrentRequest = 0;
    pCmdlist->m_FlushedSize = 0;
    pCmdlist->m_IssuedRequestCount = 0;
    pCmdlist->m_pSubroutine = nullptr;
    pCmdlist->m_SubroutineSize = 0;
    s_pCommandBufferCurrent = reinterpret_cast<u32*>(pCmdlist->m_pBuffer);
    s_pCommandBufferEnd = reinterpret_cast<u32*>(pCmdlist->m_pBuffer + pCmdlist->m_BufferSize);
}

// the number of VSyncs of a display (of both: the smaller one; the counters run up to
// 0x7FFFFFFE, then start at 0 again)
// 0x007B199C | fefates:bytes [tier B]
extern "C" GLint nngxCheckVSync(GLenum display)
{
    if (display == VSYNC_DISPLAY_0) {
        return s_Context.m_VSyncCounts[0];
    }
    if (display == VSYNC_DISPLAY_1) {
        return s_Context.m_VSyncCounts[1];
    }
    if (display != VSYNC_DISPLAY_BOTH) {
        return 0;
    }
    if (s_Context.m_VSyncCounts[0] == s_Context.m_VSyncCounts[1]) {
        return s_Context.m_VSyncCounts[0];
    }
    if (s_Context.m_VSyncCounts[0] == VSYNC_COUNT_MAX && s_Context.m_VSyncCounts[1] == 0) {
        return VSYNC_COUNT_MAX + 1;
    }
    if (s_Context.m_VSyncCounts[1] == VSYNC_COUNT_MAX && s_Context.m_VSyncCounts[0] == 0) {
        return VSYNC_COUNT_MAX + 1;
    }
    if (s_Context.m_VSyncCounts[1] <= s_Context.m_VSyncCounts[0]) {
        return s_Context.m_VSyncCounts[1];
    }
    return s_Context.m_VSyncCounts[0];
}

// 0x007B1E10 | nintendogs:bytes [tier B]
extern "C" void nngxStartCmdlistSave()
{
    AlignCommandBuffer();
    s_Context.m_IsSaving = true;
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    s_Context.m_SaveOffset = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    s_Context.m_SaveRequestCount = pCmdlist->m_RequestCount;
}

// 0x007B1EF4 | nintendogs:bytes [tier B]
extern "C" void nngxStopCmdlistSave(GLuint* pOffset, GLsizei* pSize, GLsizei* pRequestOffset, GLsizei* pRequestCount)
{
    s_Context.m_IsSaving = false;
    AlignCommandBuffer();
    *pOffset = s_Context.m_SaveOffset;
    *pSize = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - s_Context.m_pBoundCmdlist->m_pBuffer - s_Context.m_SaveOffset;
    *pRequestOffset = s_Context.m_SaveRequestCount;
    *pRequestCount = s_Context.m_pBoundCmdlist->m_RequestCount - s_Context.m_SaveRequestCount;
}

namespace {
// the anti-aliasing modes of a display transfer (names are ours)
const GLenum AA_MODE_NONE = 0x500;
const GLenum AA_MODE_2X1 = 0x501;
const GLenum AA_MODE_2X2 = 0x502;
// the formats (the OpenGL ES values)
const GLenum FORMAT_RGB8 = 0x8051;
const GLenum FORMAT_RGBA4 = 0x8056;
const GLenum FORMAT_RGB5_A1 = 0x8057;
const GLenum FORMAT_RGBA8 = 0x8058;
const GLenum FORMAT_RGB565 = 0x8D62;
} // namespace

namespace nn {
namespace gx {
namespace detail {
// a memory fill request: two ranges (size in bytes, fill width 16, 24 or 32 bits)
// 0x007C39F0 (name is ours)
void AddMemoryFillRequest(Cmdlist* pCmdlist, u32 address0, u32 size0, u32 value0, u32 width0, u32 address1, u32 size1, u32 value1, u32 width1)
{
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = 2;
    pRequest->m_Fill.m_Start0 = address0;
    pRequest->m_Fill.m_Value0 = value0;
    pRequest->m_Fill.m_Size0 = size0;
    if (width0 == 16) {
        pRequest->m_Fill.m_Width0 = 0;
    } else if (width0 == 24) {
        pRequest->m_Fill.m_Width0 = 1;
    } else if (width0 == 32) {
        pRequest->m_Fill.m_Width0 = 2;
    }
    pRequest->m_Fill.m_Size1 = size1;
    pRequest->m_Fill.m_Value1 = value1;
    pRequest->m_Fill.m_Start1 = address1;
    if (width1 == 16) {
        pRequest->m_Fill.m_Width1 = 0;
    } else if (width1 == 24) {
        pRequest->m_Fill.m_Width1 = 1;
    } else if (width1 == 32) {
        pRequest->m_Fill.m_Width1 = 2;
    }
    AddRequestInline(pCmdlist);
}

// a display transfer request (formats: the OpenGL ES values; AA: the destination size is scaled;
// flipped: the destination starts at its last line, moved by the cut of a smaller destination)
// 0x007C3748 (name is ours)
void AddDisplayTransferRequest(Cmdlist* pCmdlist, u32 source, GLsizei sourceWidth, GLsizei sourceHeight, GLenum sourceFormat, u32 destination,
                               GLsizei destinationWidth, GLsizei destinationHeight, GLenum destinationFormat, GLenum aaMode, GLboolean isFlipped,
                               GLsizei blockSize)
{
    u32 scaling;
    s32 scaleX;
    s32 scaleY;
    if (aaMode == AA_MODE_NONE) {
        scaling = 0;
        scaleX = 1;
        scaleY = 1;
    } else if (aaMode == AA_MODE_2X1) {
        scaling = 1;
        scaleX = 2;
        scaleY = 1;
    } else if (aaMode == AA_MODE_2X2) {
        scaling = 2;
        scaleX = 2;
        scaleY = 2;
    } else {
        return;
    }
    u32 inputFormat;
    switch (sourceFormat) {
    case FORMAT_RGB5_A1:
        inputFormat = 3;
        break;
    case FORMAT_RGB8:
        inputFormat = 1;
        break;
    case FORMAT_RGBA4:
        inputFormat = 4;
        break;
    case FORMAT_RGBA8:
        inputFormat = 0;
        break;
    case FORMAT_RGB565:
        inputFormat = 2;
        break;
    default:
        return;
    }
    s32 bytesPerPixel;
    u32 outputFormat;
    switch (destinationFormat) {
    case FORMAT_RGB5_A1:
        bytesPerPixel = 2;
        outputFormat = 3;
        break;
    case FORMAT_RGB8:
        bytesPerPixel = 3;
        outputFormat = 1;
        break;
    case FORMAT_RGBA4:
        bytesPerPixel = 2;
        outputFormat = 4;
        break;
    case FORMAT_RGBA8:
        bytesPerPixel = 4;
        outputFormat = 0;
        break;
    case FORMAT_RGB565:
        bytesPerPixel = 2;
        outputFormat = 2;
        break;
    default:
        return;
    }
    if (sourceWidth == 0 || sourceHeight == 0) {
        return;
    }
    if (destinationWidth == 0 || destinationHeight == 0) {
        return;
    }
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = 3;
    pRequest->m_Transfer.m_Source = source;
    pRequest->m_Transfer.m_Destination = destination;
    pRequest->m_Transfer.m_InputFormat = inputFormat;
    pRequest->m_Transfer.m_OutputFormat = outputFormat;
    pRequest->m_Transfer.m_SourceWidth = sourceWidth;
    pRequest->m_Transfer.m_SourceHeight = sourceHeight;
    pRequest->m_Transfer.m_DestinationWidth = destinationWidth * scaleX;
    pRequest->m_Transfer.m_DestinationHeight = destinationHeight * scaleY;
    if (!isFlipped) {
        pRequest->m_Transfer.m_IsFlipped = false;
    } else {
        if (!(pRequest->m_Transfer.m_DestinationWidth >= pRequest->m_Transfer.m_SourceWidth &&
              pRequest->m_Transfer.m_DestinationHeight >= pRequest->m_Transfer.m_SourceHeight)) {
            if (scaleX != 2) {
                s32 cut = pRequest->m_Transfer.m_SourceWidth - pRequest->m_Transfer.m_DestinationWidth;
                pRequest->m_Transfer.m_Destination = destination - cut * (pRequest->m_Transfer.m_DestinationHeight - 1) * bytesPerPixel;
            } else {
                s32 align = bytesPerPixel == 3 ? 16 : 8;
                s32 cut = pRequest->m_Transfer.m_SourceWidth - pRequest->m_Transfer.m_DestinationWidth;
                if ((cut & align) != 0) {
                    cut -= align;
                }
                pRequest->m_Transfer.m_Destination = destination - cut * (destinationHeight - 1) * bytesPerPixel / 2;
            }
        }
        pRequest->m_Transfer.m_IsFlipped = true;
    }
    pRequest->m_Transfer.m_Scaling = scaling;
    pRequest->m_Transfer.m_IsBlock32 = blockSize == 32;
    pRequest->m_Transfer.m_IsLinearOutput = false;
    pRequest->m_Transfer.m_IsTiledOutput = false;
    AddRequestInline(pCmdlist);
}
} // namespace detail
} // namespace gx
} // namespace nn

// 0x007B1710 | nintendogs:callseq-callee [tier A]
extern "C" void nngxAddB2LTransferCommand(const GLvoid* pSource, GLsizei sourceWidth, GLsizei sourceHeight, GLenum sourceFormat, GLvoid* pDestination,
                                          GLsizei destinationWidth, GLsizei destinationHeight, GLenum destinationFormat, GLenum aaMode, GLboolean isFlipped,
                                          GLsizei blockSize)
{
    nn::gx::detail::AddDisplayTransferRequest(s_Context.m_pBoundCmdlist, reinterpret_cast<u32>(pSource), sourceWidth, sourceHeight, sourceFormat,
                                              reinterpret_cast<u32>(pDestination), destinationWidth, destinationHeight, destinationFormat, aaMode,
                                              isFlipped, blockSize);
}

// a part of a saved list (nngxStartCmdlistSave / nngxStopCmdlistSave) is added to the bound list:
// with copy the commands and requests are copied (the requests are moved to the copies), else the
// requests run the commands of the saved list; the states are updated like nngxUpdateState
// 0x007B2288 | nintendogs:bytes [tier A]
extern "C" void nngxUseSavedCmdlist(GLuint name, GLuint offset, GLsizei size, GLuint requestOffset, GLsizei requestCount, GLbitfield states,
                                    GLboolean copy)
{
    using namespace nn::gx::detail;
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    GlState* pState = s_pGlState;
    Cmdlist* pSaved = FindCmdlist(name);
    CmdlistRequest* pRequest = nullptr;
    bool isFirst = true;

    // the commands so far end in a request of their own, or the copied 3D commands continue them
    u32 used = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    pCmdlist->m_UsedSize = used;
    if (pCmdlist->m_FlushedSize != used) {
        if (!copy) {
            nngxSplitDrawCmdlist();
        } else if (requestCount != 0) {
            if (pSaved->m_pRequests[requestOffset].m_Type != REQUEST_3D_COMMANDS) {
                nngxSplitDrawCmdlist();
            } else if ((used & 8) != 0) {
                u32* pCurrent = s_pCommandBufferCurrent;
                if (pCurrent < s_pCommandBufferEnd) {
                    pCurrent[0] = 0;
                    pCurrent[1] = DUMMY_COMMAND_HEADER;
                    s_pCommandBufferCurrent = pCurrent + 2;
                }
                pCmdlist->m_UsedSize += 8;
            }
        }
    }

    GLbitfield update = states & ~pState->m_IgnoredStates;
    if ((update & STATE_0) != 0) {
        pState->m_DirtyFlags |= DIRTY_0;
    }
    if ((update & STATE_1) != 0) {
        pState->m_DirtyFlags |= DIRTY_1;
    }
    if ((update & STATE_2) != 0) {
        pState->m_DirtyFlags |= DIRTY_2;
    }
    if ((update & STATE_3) != 0) {
        pState->m_DirtyFlags |= DIRTY_3;
    }
    if ((update & STATE_4) != 0) {
        pState->m_DirtyFlags |= DIRTY_4;
    }
    if ((update & STATE_5) != 0) {
        pState->m_DirtyFlags |= DIRTY_5;
    }
    if ((update & STATE_6) != 0) {
        pState->m_DirtyFlags |= DIRTY_6;
        for (s32 i = 0; i < 33; i++) {
            pState->m_UsedLutNames[i] = 0;
            pState->m_UsedLuts[i] = 0;
        }
    }
    if ((update & STATE_TEXTURE) != 0) {
        pState->m_DirtyFlags |= DIRTY_TEXTURE;
    }
    if ((update & STATE_FRAMEBUFFER) != 0) {
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER;
    }
    if ((update & STATE_VERTEX) != 0) {
        pState->m_DirtyFlags |= DIRTY_VERTEX;
    }
    if ((update & STATE_10) != 0) {
        pState->m_DirtyFlags |= DIRTY_10;
    }
    if ((update & STATE_11) != 0) {
        pState->m_DirtyFlags |= DIRTY_11;
    }
    if ((update & STATE_12) != 0) {
        pState->m_DirtyFlags |= DIRTY_12;
    }
    pState->m_UpdatedStates |= update;

    if (requestCount != 0) {
        memcpy(&pCmdlist->m_pRequests[pCmdlist->m_RequestCount], &pSaved->m_pRequests[requestOffset], requestCount * sizeof(CmdlistRequest));
        pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    }
    if (copy) {
        u32 copied = 0;
        u32 copySize = 0;
        u32 previousAddress = 0;
        for (s32 i = 0; i < requestCount; i++, pRequest++) {
            if (pRequest->m_Type == REQUEST_3D_COMMANDS) {
                u32 source;
                if (isFirst) {
                    isFirst = false;
                    // the first request starts at the offset
                    if ((pRequest->m_Commands.m_Flags & REQUEST_FLAG_EXTERNAL) == 0) {
                        pRequest->m_Commands.m_Size -= offset + reinterpret_cast<u32>(pSaved->m_pBuffer) - pRequest->m_Commands.m_Address;
                        pRequest->m_Commands.m_Address = offset + reinterpret_cast<u32>(pSaved->m_pBuffer);
                    }
                    source = pRequest->m_Commands.m_Address;
                    copySize = pRequest->m_Commands.m_Size;
                    pRequest->m_Commands.m_Size = pCmdlist->m_UsedSize - pCmdlist->m_FlushedSize + copySize;
                    pRequest->m_Commands.m_Address = reinterpret_cast<u32>(pCmdlist->m_pBuffer) + pCmdlist->m_FlushedSize;
                } else {
                    source = pRequest->m_Commands.m_Address;
                    pRequest->m_Commands.m_Address = previousAddress + copySize;
                    copySize = pRequest->m_Commands.m_Size;
                }
                memcpy(s_pCommandBufferCurrent, reinterpret_cast<const void*>(source), copySize);
                s_pCommandBufferCurrent = reinterpret_cast<u32*>(reinterpret_cast<u8*>(s_pCommandBufferCurrent) + (copySize & ~3));
                u32 flags = pRequest->m_Commands.m_Flags;
                pRequest->m_Commands.m_Flags = flags & ~REQUEST_FLAG_EXTERNAL;
                if ((flags & REQUEST_FLAG_EXTERNAL) == 0) {
                    copied += copySize;
                }
                copySize = pRequest->m_Commands.m_Size;
                pCmdlist->m_FlushedSize += copySize;
                previousAddress = pRequest->m_Commands.m_Address;
                if (pCmdlist->m_pSubroutine != nullptr) {
                    FinishSubroutineInline(pCmdlist, pRequest);
                }
            }
            pRequest->m_IsCallbackEnabled = false;
            pRequest->m_IsStopPoint = false;
        }
        // the commands behind the last request
        if (static_cast<u32>(size) > copied) {
            u32 rest = size - copied;
            memcpy(s_pCommandBufferCurrent, pSaved->m_pBuffer + offset + copied, rest);
            s_pCommandBufferCurrent = reinterpret_cast<u32*>(reinterpret_cast<u8*>(s_pCommandBufferCurrent) + (rest & ~3));
        }
        pCmdlist->m_UsedSize = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
    } else {
        for (s32 i = 0; i < requestCount; i++, pRequest++) {
            if (pRequest->m_Type == REQUEST_3D_COMMANDS && (pRequest->m_Commands.m_Flags & REQUEST_FLAG_EXTERNAL) == 0) {
                if (isFirst) {
                    isFirst = false;
                    pRequest->m_Commands.m_Size -= offset + reinterpret_cast<u32>(pSaved->m_pBuffer) - pRequest->m_Commands.m_Address;
                    pRequest->m_Commands.m_Address = offset + reinterpret_cast<u32>(pSaved->m_pBuffer);
                }
                pRequest->m_Commands.m_Flags |= REQUEST_FLAG_EXTERNAL;
            }
            pRequest->m_IsCallbackEnabled = false;
            pRequest->m_IsStopPoint = false;
        }
    }
    if (requestCount != 0) {
        AddRequestInline(pCmdlist, requestCount);
    }
}
