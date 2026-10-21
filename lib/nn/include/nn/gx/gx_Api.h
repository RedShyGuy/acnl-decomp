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
void glEnable(GLenum cap); // 0x007AA1F8
void glDisable(GLenum cap); // 0x007A6440
void glBlendEquation(GLenum mode); // 0x007A85F4
void glBlendEquationSeparate(GLenum modeRgb, GLenum modeAlpha); // 0x007A85FC
void glBlendFunc(GLenum sourceFactor, GLenum destinationFactor); // 0x007A8774
void glBlendFuncSeparate(GLenum sourceRgb, GLenum destinationRgb, GLenum sourceAlpha, GLenum destinationAlpha); // 0x007A8780
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height); // 0x007AE564
void glDrawArrays(GLenum mode, GLint first, GLsizei count); // 0x007A980C
void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* pIndices); // 0x007A9C98
void glLogicOp(GLenum opcode); // 0x007AC2D4
void glLinkProgram(GLuint program); // 0x007AA83C
void glUseProgram(GLuint program); // 0x007ADF48
void glBindAttribLocation(GLuint program, GLuint index, const GLchar* pName); // 0x007A7828
GLint glGetUniformLocation(GLuint program, const GLchar* pName); // 0x007AA6C4

// vertex attributes and uniforms (gx_Vertex.cpp)
void glDisableVertexAttribArray(GLuint index); // 0x007A97D4
void glEnableVertexAttribArray(GLuint index); // 0x007AA200
void glVertexAttrib1f(GLuint index, GLfloat x); // 0x007AE45C
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pPointer); // 0x007AE490
void glUniform1f(GLint location, GLfloat x); // 0x007ADE1C
void glUniform1i(GLint location, GLint x); // 0x007ADE54
void glUniform2i(GLint location, GLint x, GLint y); // 0x007ADE70
void glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w); // 0x007ADE94
void glUniform4fv(GLint location, GLsizei count, const GLfloat* pValues); // 0x007ADED8
void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* pValues); // 0x007ADF08

// texture objects (gx_Texture.cpp)
void glGenTextures(GLsizei n, GLuint* pNames); // 0x007AA368
void glBindTexture(GLenum target, GLuint texture); // 0x007A7DB8
void glDeleteTextures(GLsizei n, const GLuint* pNames); // 0x00142184
void glTexParameterf(GLenum target, GLenum pname, GLfloat param); // 0x007AD7DC
void glTexParameterfv(GLenum target, GLenum pname, const GLfloat* pParams); // 0x007AD7F4
void glTexParameteri(GLenum target, GLenum pname, GLint param); // 0x007ADB28
void glTexParameteriv(GLenum target, GLenum pname, const GLint* pParams); // 0x007ADB3C
void glGetTexParameteriv(GLenum target, GLenum pname, GLint* pParams); // 0x007AA4A4
void glActiveTexture(GLenum texture); // 0x007A7750
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format,
                  GLenum type, const GLvoid* pPixels); // 0x007AD61C
void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border,
                            GLsizei imageSize, const GLvoid* pData); // 0x007A8D7C
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height,
                      GLint border); // 0x007A8F3C

// buffer objects (gx_Buffer.cpp)
void glGenBuffers(GLsizei n, GLuint* pNames); // 0x007AA238
void glBindBuffer(GLenum target, GLuint buffer); // 0x007A7974
void glDeleteBuffers(GLsizei n, const GLuint* pNames); // 0x007A92D0
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* pData, GLenum usage); // 0x007A8C1C

// programs and shaders (gx_Shader.cpp)
GLuint glCreateProgram(); // 0x007A9098
GLuint glCreateShader(GLenum type); // 0x007A91B0
void glDeleteProgram(GLuint program); // 0x007A94DC
void glDeleteShader(GLuint shader); // 0x007A9548
void glAttachShader(GLuint program, GLuint shader); // 0x007A776C
void glDetachShader(GLuint program, GLuint shader); // 0x007A9638
void glShaderBinary(GLsizei n, const GLuint* pShaders, GLenum binaryFormat, const GLvoid* pBinary, GLsizei length); // 0x007AC410

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

// the requests of the GL functions (names from the symbols)
void __sys_markGasAcmMax();
void __sys_setB2BCommandRequest(u32 source, u32 destination, GLsizei width, GLsizei height, u32 format);
void __sys_setCTXCommandRequest(u32 destination, u32 source, u32 size, u16 sourceGap, u16 sourceWidth, u16 destinationGap,
                                u16 destinationWidth);
void __sys_setDMACommandRequest(u32 destination, u32 source, u32 size);
void __mem_readMemory(const void* pSource, void* pDestination, u32 size);

// the register writes of the GL functions (names from the symbols)
void __cb_writeRegs(u32 reg, u32 count, const u32* pValues);
void __cb_fillRegs(u32 reg, u32 count, u32 value);
void __cb_multiWriteReg(u32 reg, u32 count, const u32* pValues);
void __cb_addDummyWrite(u32 reg, u32 count);

// the parts of the GL state (names from the symbols; the arguments are their memory)
int __sta_initializeState(void* pState, void* pValidator);
int __vb_initializeVBManager(void* pMemory);
int __shm_initializeShaderManager(void* pMemory);
int __tx_initializeTexManager(void* pMemory);
int __vldtr_initializeStateValidator(nn::gx::detail::StateValidator* pValidator);
// the validators: the dirty parts of the GL state become commands (the dirty flags are words with
// the bits DIRTY_*, the first one is that of the GL state)
void __vldtr_validateAllState();
void __txv_initializeTextureValidator(u32* pDirtyMask);
void __txv_initializeTextureValidatorHWRegister();
void __txv_validateTextureValidator(const u32* pDirtyFlags);
void __fbv_initializeFBValidator(u32* pDirtyMask);
void __fbv_validateFBValidator(const u32* pDirtyFlags);
void __vbv_initializeVBValidator(u32* pDirtyMask);
void __vbv_validateVBValidator(const u32* pDirtyFlags);
void __shv_initializeShaderValidator(u32* pDirtyMask);
void __shv_initializeShaderValidatorHWRegister();
void __shv_validateShaderValidator(const u32* pDirtyFlags);
void __shv_partialValidateShaderValidator(const u32* pDirtyFlags, GLbitfield states);
void __shv_preDraw();
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
// the constant value of a vertex attribute
void SetVertexAttributeValue(GLuint index, const GLfloat* pValues); // 0x007C6618 (name is ours)
// the uniforms of the program in use: components per value, count values (matrices: count
// matrices of four rows; isTransposeNeeded for an untransposed matrix)
void SetUniformFloat(GLint location, const GLfloat* pValues, GLsizei components, GLsizei count, bool isTransposeNeeded, bool isMatrix); // 0x007B9794 (name is ours)
void SetUniformInt(GLint location, const GLint* pValues, GLsizei components, GLsizei count); // 0x007BE1E4 (name is ours)
// the viewport commands of the GL state (GPU registers 0x41-0x44, 0x68 after 3dbrew) and the
// registers of the fragment operations again
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
