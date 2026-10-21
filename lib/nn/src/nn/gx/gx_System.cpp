#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include <cstring>

// The requests the GL functions add to the bound command list (__sys_*) and the memory copy of the
// state (__mem_readMemory).

using namespace nn::gx::detail;

namespace {
// the request types (3dbrew GX commands)
const u8 REQUEST_DMA = 0;
const u8 REQUEST_DISPLAY_TRANSFER = 3;
const u8 REQUEST_TEXTURE_COPY = 4;
// the scaling of a display transfer (halves width and height)
const u32 SCALING_2X2 = 2;
} // namespace

extern "C" {
// the commands so far as a request whose flag bit 0 is set
// 0x007A0C70 | nintendogs:bytes [tier B]
void __sys_markGasAcmMax()
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    if (pCmdlist == nullptr) {
        return;
    }
    bool isSet = pCmdlist->m_IsRequestFlagSet;
    pCmdlist->m_IsRequestFlagSet = true;
    nngxSplitDrawCmdlist();
    if (!isSet) {
        s_Context.m_pBoundCmdlist->m_IsRequestFlagSet = false;
    }
}

// a display transfer that halves the image into the tiles of the next level (the mipmaps)
// 0x007A0CB0 | nintendogs:bytes [tier B]
void __sys_setB2BCommandRequest(u32 source, u32 destination, GLsizei width, GLsizei height, u32 format)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_DISPLAY_TRANSFER;
    pRequest->m_Transfer.m_Source = source;
    pRequest->m_Transfer.m_Destination = destination;
    pRequest->m_Transfer.m_DestinationWidth = width;
    pRequest->m_Transfer.m_SourceWidth = pRequest->m_Transfer.m_DestinationWidth;
    pRequest->m_Transfer.m_DestinationHeight = height;
    pRequest->m_Transfer.m_OutputFormat = format;
    pRequest->m_Transfer.m_InputFormat = pRequest->m_Transfer.m_OutputFormat;
    pRequest->m_Transfer.m_IsFlipped = false;
    pRequest->m_Transfer.m_Scaling = SCALING_2X2;
    pRequest->m_Transfer.m_IsBlock32 = false;
    pRequest->m_Transfer.m_IsLinearOutput = false;
    pRequest->m_Transfer.m_SourceHeight = pRequest->m_Transfer.m_DestinationHeight;
    pRequest->m_Transfer.m_IsTiledOutput = true;
    AddRequestInline(pCmdlist);
}

// a texture copy (lines and gaps in units of 16 bytes)
// 0x007A0D70 | nintendogs:bytes [tier B]
void __sys_setCTXCommandRequest(u32 destination, u32 source, u32 size, u16 sourceGap, u16 sourceWidth, u16 destinationGap,
                                u16 destinationWidth)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_TEXTURE_COPY;
    pRequest->m_Copy.m_Source = source;
    pRequest->m_Copy.m_Destination = destination;
    pRequest->m_Copy.m_Size = size;
    pRequest->m_Copy.m_SourceGap = sourceGap;
    pRequest->m_Copy.m_SourceWidth = sourceWidth;
    pRequest->m_Copy.m_DestinationGap = destinationGap;
    pRequest->m_Copy.m_DestinationWidth = destinationWidth;
    AddRequestInline(pCmdlist);
}

// 0x007A0E04 | nintendogs:bytes [tier B]
void __sys_setDMACommandRequest(u32 destination, u32 source, u32 size)
{
    Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
    CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
    pRequest->m_Type = REQUEST_DMA;
    pRequest->m_Dma.m_Source = source;
    pRequest->m_Dma.m_Destination = destination;
    pRequest->m_Dma.m_Size = size;
    AddRequestInline(pCmdlist);
}

// 0x0077C874 | nintendogs:bytes [tier B]
void __mem_readMemory(const void* pSource, void* pDestination, u32 size)
{
    if (size != 0) {
        memcpy(pDestination, pSource, size);
    }
}
} // extern "C"
