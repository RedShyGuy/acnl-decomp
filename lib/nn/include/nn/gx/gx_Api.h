#pragma once

// The C interface of the gx library (nngx*, the names are from the reference symbols). The gl*
// functions are the OpenGL ES API on top of it.

#include "decomp.h"
#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/gx_Types.h"
#include "nn/gxlow/CTR/CTR_Api.h"

extern "C" {
// errors (glGetError)
GLenum glGetError(); // 0x00127D94

// the start of the library (the allocator of the library objects and buffers)
GLboolean nngxInitialize(GxAllocateFunction pAllocate, GxFreeFunction pFree); // 0x00121E40

// the allocator and the command generation mode (0x800 immediate, 0x801 deferred)
void nngxGetAllocator(GxAllocateFunction* pAllocate, GxFreeFunction* pFree); // 0x007B1C64
void nngxGetCommandGenerationMode(GLenum* pMode); // 0x007B1C88
void nngxSetCommandGenerationMode(GLenum mode); // 0x007B1DE0

// command lists
void nngxGenCmdlists(GLsizei n, GLuint* pNames); // 0x00121B48
void nngxBindCmdlist(GLuint name); // 0x001216D0
void nngxCmdlistStorage(GLsizei bufferSize, GLsizei requestCount); // 0x0012199C
void nngxDeleteCmdlists(GLsizei n, const GLuint* pNames); // 0x00127DAC
void nngxGetCmdlistParameteri(GLenum name, GLint* pParameter); // 0x00127EF0
void nngxSetCmdlistParameteri(GLenum name, GLint parameter); // 0x007B1DA0
void nngxMoveCommandbufferPointer(GLint offset); // 0x001280C8
void nngxEnableCmdlistCallback(GLint request); // 0x0011F35C
void nngxSetCmdlistCallback(void (*pCallback)(GLint)); // 0x0011F398
void nngxRunCmdlist(); // 0x007B1D40
void nngxStopCmdlist(); // 0x007B1E9C
void nngxSplitDrawCmdlist(); // 0x001280E0
void nngxFlush3DCommand(); // 0x007B1ABC
void nngxAdd3DCommand(const GLvoid* pCommands, GLsizei size, GLboolean copy); // 0x007B1630
void nngxAddVramDmaCommand(const GLvoid* pSource, GLvoid* pDestination, GLsizei size); // 0x007B18A4
void nngxAddVramDmaCommandNoCacheFlush(const GLvoid* pSource, GLvoid* pDestination, GLsizei size); // 0x007B1918
void nngxClearCmdlist(); // 0x007B1A2C
void nngxAddB2LTransferCommand(const GLvoid* pSource, GLsizei sourceWidth, GLsizei sourceHeight, GLenum sourceFormat, GLvoid* pDestination,
                               GLsizei destinationWidth, GLsizei destinationHeight, GLenum destinationFormat, GLenum aaMode, GLboolean isFlipped,
                               GLsizei blockSize); // 0x007B1710
GLint nngxCheckVSync(GLenum display); // 0x007B199C
void nngxStartCmdlistSave(); // 0x007B1E10
void nngxStopCmdlistSave(GLuint* pOffset, GLsizei* pSize, GLsizei* pRequestOffset, GLsizei* pRequestCount); // 0x007B1EF4
void nngxAddMemoryFillCommand(unsigned int address0, unsigned int size0, unsigned int value0, unsigned int width0,
                              unsigned int address1, unsigned int size1, unsigned int value1, unsigned int width1); // 0x007B175C

// display buffers (display 0x400, 0x401 or 0x410)
void nngxActiveDisplay(GLenum display); // 0x001216A0
void nngxGenDisplaybuffers(GLsizei n, GLuint* pNames); // 0x00121C7C
void nngxBindDisplaybuffer(GLuint name); // 0x00121894
void nngxDisplaybufferStorage(GLenum format, GLsizei width, GLsizei height, GLenum area); // 0x00121A84
void nngxGetDisplaybufferParameteri(GLenum name, GLint* pParameter); // 0x007B1CB0
void nngxSetDisplayMode(GLenum mode); // 0x00122AE4

// the GL state, the buffer swap, the waits
void nngxUpdateState(GLbitfield states); // 0x007B2138
GLenum nngxValidateState(GLbitfield states, GLboolean isPartial); // 0x007B27B4
void nngxSwapBuffers(GLenum display); // 0x007B1F9C
void nngxUpdateBuffer(const GLvoid* pData, GLsizei size); // 0x007B2134
void nngxWaitCmdlistDone(); // 0x007B2998
void nngxWaitVSync(GLenum display); // 0x007B2A34
void nngxUseSavedCmdlist(GLuint name, GLuint offset, GLsizei size, GLuint requestOffset, GLsizei requestCount, GLbitfield states,
                         GLboolean copy); // 0x007B2288

// the addresses of the VRAM areas, the 3D slider
uptr nngxGetVramStartAddr(GLenum area); // 0x00121DF0
uptr nngxGetVramEndAddr(GLenum area); // 0x00121DB4
GLboolean nngxIsStereoVisionAllowed(); // 0x007B1D08
} // extern "C"

namespace nn {
namespace gx {
namespace detail {
// executes the next requests of the running command list (with nngxlowLock held)
void ExecuteRequests(); // 0x0012E7EC (name is ours)
// closes the subroutine of the list at the end of the request
void FinishSubroutine(Cmdlist* pCmdlist, CmdlistRequest* pRequest); // 0x0012E794 (name is ours)
// (the body of FinishSubroutine: the jump back gets its size, the request runs the commands of
// the subroutine; inline in nngxUseSavedCmdlist)
DECOMP_ALWAYS_INLINE void FinishSubroutineInline(Cmdlist* pCmdlist, CmdlistRequest* pRequest)
{
    u32* pSubroutine = pCmdlist->m_pSubroutine;
    pSubroutine[2] = (pRequest->m_Commands.m_Address + pRequest->m_Commands.m_Size - reinterpret_cast<u32>(pSubroutine) - 24) >> 3;
    nngxlowFlushDataCache(pRequest->m_Commands.m_Address, pRequest->m_Commands.m_Size);
    pRequest->m_Commands.m_Flags |= 1 << 2;
    pRequest->m_Commands.m_Size = pCmdlist->m_SubroutineSize;
    pCmdlist->m_pSubroutine = nullptr;
    pCmdlist->m_SubroutineSize = 0;
}
// a request of the running list is done (all interrupt handlers of requests end in it)
void FinishRequest(); // 0x0012E6C8 (name is ours)
// the interrupt handlers that nngxInitialize registers (names are ours)
void OnPsc0Interrupt(); // 0x00128348 (name is ours)
void OnPsc1Interrupt(); // 0x0012834C (name is ours)
void OnVBlankTopInterrupt(); // 0x001282B8 (name is ours)
void OnVBlankBottomInterrupt(); // 0x001282FC (name is ours)
void OnPpfInterrupt(); // 0x00128344 (name is ours)
void OnP3dInterrupt(); // 0x00128278 (name is ours)
void OnDmaInterrupt(); // 0x0012E6C4 (name is ours)
// the viewport commands of the GL state (GPU registers 0x41-0x44, 0x68 after 3dbrew)
void ValidateViewport(); // 0x007C6768 (name is ours)
// a display transfer request (nngxAddB2LTransferCommand)
void AddDisplayTransferRequest(Cmdlist* pCmdlist, u32 source, GLsizei sourceWidth, GLsizei sourceHeight, GLenum sourceFormat, u32 destination,
                               GLsizei destinationWidth, GLsizei destinationHeight, GLenum destinationFormat, GLenum aaMode, GLboolean isFlipped,
                               GLsizei blockSize); // 0x007C3748 (name is ours)
// a memory fill request (nngxAddMemoryFillCommand)
void AddMemoryFillRequest(Cmdlist* pCmdlist, u32 address0, u32 size0, u32 value0, u32 width0, u32 address1, u32 size1, u32 value1,
                          u32 width1); // 0x007C39F0 (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn
