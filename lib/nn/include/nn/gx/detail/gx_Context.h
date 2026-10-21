#pragma once

// The state of the gx library (the nngx* and gl* functions): command lists, display buffers and
// the global context. The layouts come from the accesses of the nngx functions; all names are
// ours.

#include "decomp.h"
#include "nn/gx/gx_Types.h"

extern "C" {
// the allocator the program gives nngxInitialize: area (MEMORY_AREA_*), purpose (MEMORY_PURPOSE_*),
// the name of the object and the size
typedef void* (*GxAllocateFunction)(GLenum area, GLenum purpose, GLuint name, GLsizei size);
typedef void (*GxFreeFunction)(GLenum area, GLenum purpose, GLuint name, void* pAddress);
} // extern "C"

namespace nn {
namespace gx {
namespace detail {
// the memory areas of the allocator (the main memory, the two halves of VRAM after 3dbrew)
const GLenum MEMORY_AREA_MAIN = 0x10000;
const GLenum MEMORY_AREA_VRAM_A = 0x20000;
const GLenum MEMORY_AREA_VRAM_B = 0x30000;
// what the allocator allocates
const GLenum MEMORY_PURPOSE_OBJECT = 0x100;        // the objects of the library
const GLenum MEMORY_PURPOSE_DISPLAYBUFFER = 0x104; // the memory of a display buffer
const GLenum MEMORY_PURPOSE_CMDLIST = 0x105;       // the command buffer and requests of a list

// the state bits of nngxUpdateState / nngxValidateState and the dirty flags each one sets (names
// are ours; the meaning is known where a validator handles the bit)
const GLbitfield STATE_0 = 1 << 0;
const GLbitfield STATE_1 = 1 << 1;
const GLbitfield STATE_2 = 1 << 2;
const GLbitfield STATE_3 = 1 << 3;
const GLbitfield STATE_4 = 1 << 4;
const GLbitfield STATE_5 = 1 << 5;
const GLbitfield STATE_6 = 1 << 6;
const GLbitfield STATE_TEXTURE = 1 << 7;     // __txv_validateTextureValidator
const GLbitfield STATE_FRAMEBUFFER = 1 << 8; // __fbv_validateFBValidator
const GLbitfield STATE_VERTEX = 1 << 9;      // __vbv_validateVBValidator
const GLbitfield STATE_10 = 1 << 10;
const GLbitfield STATE_11 = 1 << 11;
const GLbitfield STATE_12 = 1 << 12;
const GLbitfield STATE_VIEWPORT = 1 << 13;   // ValidateViewport
// the states the shader validation handles
const GLbitfield STATE_SHADER_MASK = 0x1E7F;

const u32 DIRTY_0 = 0x100000;
const u32 DIRTY_1 = 0x10000;
const u32 DIRTY_2 = 0x1800000;
const u32 DIRTY_3 = 0x600000;
const u32 DIRTY_4 = 0x10000;
const u32 DIRTY_5 = 0x80000;
const u32 DIRTY_6 = 0x2004038;
const u32 DIRTY_TEXTURE = 0x1C00;
const u32 DIRTY_FRAMEBUFFER = 0x1;
const u32 DIRTY_VERTEX = 0x80C2;
const u32 DIRTY_10 = 0x4;
const u32 DIRTY_11 = 0x100;
const u32 DIRTY_12 = 0x200;

// the hash tables of the named objects: 32 lists sorted by name
const u32 BUCKET_COUNT = 32;

// a request of a command list; the types are those of the GX commands on 3dbrew (0 DMA, 1 command
// list, 2 memory fill, 3 display transfer, 4 texture copy)
struct CmdlistRequest
{
    u8 m_Type;                // 0x00
    bool m_IsCallbackEnabled; // 0x01
    bool m_IsStopPoint;       // 0x02 (nngxStopCmdlist: the execution stops after it)
    u8 _unknown0x03;
    union
    {
        // type 1: the 3D commands (flags: bit 0 the flag of nngxSetCmdlistParameteri 0x20F,
        // bit 1 commands outside the buffer, bit 2 the cache is flushed already)
        struct
        {
            u32 m_Address; // 0x04
            u32 m_Size;    // 0x08
            u32 m_Flags;   // 0x0C
        } m_Commands;
        // type 0: a DMA (flags: bit 0 without cache flush)
        struct
        {
            u32 m_Source;      // 0x04
            u32 m_Destination; // 0x08
            u32 m_Size;        // 0x0C
            u32 m_Flags;       // 0x10
        } m_Dma;
        // type 2: two memory fills
        struct
        {
            u32 m_Start0;      // 0x04
            u32 m_Value0;      // 0x08
            u32 m_Size0 : 23;  // 0x0C
            u32 m_Width0 : 2;  // (0: 16 bits, 1: 24 bits, 2: 32 bits)
            u32 : 7;
            u32 m_Start1;      // 0x10
            u32 m_Value1;      // 0x14
            u32 m_Size1 : 23;  // 0x18
            u32 m_Width1 : 2;
            u32 : 7;
        } m_Fill;
        // type 3: a display transfer (the mode bits after the display transfer flags on 3dbrew)
        struct
        {
            u32 m_Source;                   // 0x04
            u32 m_Destination;              // 0x08
            u32 m_SourceWidth : 16;         // 0x0C
            u32 m_SourceHeight : 16;        // 0x0E
            u32 m_DestinationWidth : 16;    // 0x10
            u32 m_DestinationHeight : 16;   // 0x12
            u32 m_InputFormat : 3;          // 0x14
            u32 m_OutputFormat : 3;
            u32 m_IsFlipped : 1;
            u32 m_Scaling : 2;              // (0: none, 1: 2x1, 2: 2x2)
            u32 m_IsBlock32 : 1;
            u32 m_IsLinearOutput : 1;
            u32 m_IsTiledOutput : 1;
            u32 : 20;
        } m_Transfer;
        // type 4: a texture copy (lines: line width << 16 | gap)
        struct
        {
            u32 m_Source;           // 0x04
            u32 m_Destination;      // 0x08
            u32 m_Size;             // 0x0C
            u32 m_SourceLine;       // 0x10
            u32 m_DestinationLine;  // 0x14
        } m_Copy;
        u32 m_Parameters[6];   // 0x04
    };
};
ASSERT_SIZE(CmdlistRequest, 0x1C);

// a command list: the command buffer, the requests behind it and the progress of its execution
struct Cmdlist
{
    GLuint m_Name;                // 0x00
    u8* m_pBuffer;                // 0x04 (the commands, then the requests)
    u32 m_BufferSize;             // 0x08
    u32 m_UsedSize;               // 0x0C (written when the list is unbound)
    u32 m_ExecutedSize;           // 0x10
    u32 m_FlushedSize;            // 0x14 (the commands up to it are in requests)
    CmdlistRequest* m_pRequests;  // 0x18
    u32 m_MaxRequestCount;        // 0x1C
    s32 m_RequestCount;           // 0x20
    s32 m_IssuedRequestCount;     // 0x24 (the requests given to the GPU)
    s32 m_CurrentRequest;         // 0x28 (the request being executed)
    GLint m_RunMode;              // 0x2C (parameter 0x200; 0x300: nngxRunCmdlist starts it)
    u32 m_SubroutineSize;         // 0x30
    u32* m_pSubroutine;           // 0x34 (FinishSubroutine)
    void (*m_pCallback)(GLint);   // 0x38
    bool m_IsCallbackEnabledAll;  // 0x3C
    bool m_IsRequestFlagSet;      // 0x3D (parameter 0x20F; bit 0 of the flags of new requests)
    u8 _unknown0x3E[2];
    Cmdlist* m_pNext;             // 0x40
};
ASSERT_SIZE(Cmdlist, 0x44);

// a display buffer
struct Displaybuffer
{
    GLuint m_Name;          // 0x00
    u32 m_Address;          // 0x04
    GLenum m_Format;        // 0x08
    GLsizei m_Width;        // 0x0C
    GLsizei m_Height;       // 0x10
    GLenum m_Area;          // 0x14
    Displaybuffer* m_pNext; // 0x18
};
ASSERT_SIZE(Displaybuffer, 0x1C);

// the global state of the library (only the part known so far). Display index 0 is the upper
// screen, 1 the lower one, 2 the right eye of the upper screen.
struct Context
{
    u32 m_DisplayHeights[2];                         // 0x000 (240, 240)
    u32 m_DisplayWidths[2];                          // 0x008 (400 or 800 in nngxSetDisplayMode, 320)
    bool m_IsInitialized;                            // 0x010
    bool m_IsRunning;                                // 0x011
    bool m_IsRunRequested;                           // 0x012
    bool m_IsSaving;                                 // 0x013
    s32 m_MaxSpeculativeRequests;                    // 0x014 (requests given to the GPU ahead)
    bool m_IsStopRequested;                          // 0x018
    u8 _unknown0x19[3];
    Cmdlist* m_pCmdlists[BUCKET_COUNT];              // 0x01C
    Cmdlist* m_pBoundCmdlist;                        // 0x09C
    Cmdlist* m_pRunningCmdlist;                      // 0x0A0
    Displaybuffer* m_pDisplaybuffers[BUCKET_COUNT];  // 0x0A4
    s32 m_ActiveDisplay;                             // 0x124
    Displaybuffer* m_pBoundDisplaybuffers[3];        // 0x128
    s32 m_DisplayOffsetsX[3];                        // 0x134
    s32 m_DisplayOffsetsY[3];                        // 0x140
    s32 m_ActiveFramebuffers[2];                     // 0x14C (0 or 1, toggled by nngxSwapBuffers)
    volatile s32 m_VSyncCounts[2];                   // 0x154 (per display, counted by the interrupt)
    void (*m_pVSyncCallbacks[2])(GLenum);            // 0x15C
    GLenum m_DisplayMode;                            // 0x164
    u32 m_SaveOffset;                                // 0x168
    s32 m_SaveRequestCount;                          // 0x16C
    void (*m_pTimeoutCallback)();                    // 0x170 (nngxWaitCmdlistDone)
    u32 _unknown0x174;
    s64 m_Timeout;                                   // 0x178 (ticks; 0: no timeout)
};
ASSERT_OFFSET(Context, m_pCmdlists, 0x1C);
ASSERT_OFFSET(Context, m_pBoundCmdlist, 0x9C);
ASSERT_OFFSET(Context, m_pDisplaybuffers, 0xA4);
ASSERT_OFFSET(Context, m_ActiveDisplay, 0x124);
ASSERT_OFFSET(Context, m_DisplayOffsetsX, 0x134);
ASSERT_OFFSET(Context, m_ActiveFramebuffers, 0x14C);
ASSERT_OFFSET(Context, m_VSyncCounts, 0x154);
ASSERT_OFFSET(Context, m_DisplayMode, 0x164);
ASSERT_OFFSET(Context, m_Timeout, 0x178);

// the allocator of nngxInitialize and the next free names
struct AllocatorState
{
    GxAllocateFunction m_pAllocate;  // 0x0
    GxFreeFunction m_pFree;          // 0x4
    GLuint m_NextCmdlistName;        // 0x8
    GLuint m_NextDisplaybufferName;  // 0xC
    u8* m_pStateMemory;              // 0x10 (the memory of the GL state, 0x30FC bytes)
};

// the state block of the GL functions (only the part known so far)
struct GlState
{
    u32 m_DirtyFlags;                   // 0x000 (what the validators must regenerate)
    u32 m_UpdatedStates;                // 0x004 (nngxUpdateState, cleared by nngxValidateState)
    u32 m_IgnoredStates;                // 0x008
    bool m_IsCommandGenerationDeferred; // 0x00C (nngxSetCommandGenerationMode)
    u8 _unknown0x0D[0x18 - 0x0D];
    bool m_IsValidating;                // 0x018 (during the shader validation of nngxValidateState)
    bool m_IsFullValidation;            // 0x019
    u8 _unknown0x1A[0x10C - 0x1A];
    u32 m_StateBlock0[33];              // 0x10C (cleared with the update of state bit 6)
    u32 m_StateBlock1[33];              // 0x190
};
ASSERT_OFFSET(GlState, m_IsValidating, 0x18);
ASSERT_OFFSET(GlState, m_StateBlock0, 0x10C);
ASSERT_OFFSET(GlState, m_StateBlock1, 0x190);

// the state of the vertex buffers (only the part known so far)
struct VertexState
{
    u8 _unknown0x0[8];
    u32 m_DirtyMask; // 0x8 (the dirty flags the vertex buffer validator handles)
};

extern Context s_Context;
extern AllocatorState s_Allocator;
extern u32* s_pCommandBufferCurrent;
extern u32* s_pCommandBufferEnd;
extern GLenum s_Error;
extern GlState* s_pGlState;
extern VertexState* s_pVertexState;
} // namespace detail
} // namespace gx
} // namespace nn
