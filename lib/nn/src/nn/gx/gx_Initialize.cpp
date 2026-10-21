#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include <cstring>

// nngxInitialize: the context, the GL state (the managers of the GL functions in one memory
// block), a first command list, the interrupt handlers and, at the first initialization of the
// GPU module, the hardware registers (GPU and LCD timings).

using namespace nn::gx::detail;

extern "C" {
// the parts of the GL state (names from the symbols; the arguments are their memory)
int __sta_initializeState(void* pState, void* pValidator); // 0x00127644
int __fb_initializeFBManager(void* pManager); // 0x00126DB8
int __vb_initializeVBManager(void* pManager); // 0x00127C90
int __shm_initializeShaderManager(void* pManager); // 0x00126DFC
int __tx_initializeTexManager(void* pManager); // 0x00127ADC
int __vldtr_initializeStateValidator(void* pValidator); // 0x00127D04
}

namespace nn {
namespace gx {
namespace detail {
// the size of the commands of the GL state set up by nngxInitialize
// 0x00975AC0 (name is ours)
u32 s_InitialCommandSize;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the memory of the GL state and the places of its parts
const GLsizei STATE_MEMORY_SIZE = 0x30FC;
const u32 FB_MANAGER_OFFSET = 0x868;
const u32 VB_MANAGER_OFFSET = 0x878;
const u32 SHADER_MANAGER_OFFSET = 0x10CC;
const u32 TEXTURE_MANAGER_OFFSET = 0x2840;
const u32 STATE_VALIDATOR_OFFSET = 0x30EC;
// the first command list of nngxInitialize (deleted again at the end)
const GLsizei INITIAL_BUFFER_SIZE = 0x10000;
const GLsizei INITIAL_REQUEST_COUNT = 16;
// the display mode at the start (nngxSetDisplayMode)
const GLenum DISPLAY_MODE_0 = 0x700;
// the first display transfer: 128 x 128 from the end of VRAM B into a buffer of the process
const GLsizei FIRST_TRANSFER_SIZE = 0x8010;
const u32 FIRST_TRANSFER_SOURCE_OFFSET = 0x7FFF;
const u32 FIRST_TRANSFER_EDGE = 128;

DECOMP_ALWAYS_INLINE void WriteRegister(u32 address, u32 value)
{
    u32 data = value;
    GSPGPU_WriteHWRegs(address, &data, sizeof(data));
}

DECOMP_ALWAYS_INLINE void WriteRegisterWithMask(u32 address, u32 value, u32 mask)
{
    u32 data[2] = {value, mask};
    nngxlowWriteHWRegsWithMask(address, &data[0], &data[1], sizeof(u32));
}

// the registers of the GPU and the timings of the LCDs (the addresses of the GPU module: 0x400400
// and 0x400500 are the two LCD controllers of 3dbrew "GPU/External Registers")
DECOMP_ALWAYS_INLINE void InitializeHardware()
{
    WriteRegister(0x401000, 0);
    WriteRegister(0x401080, 0x12345678);
    WriteRegister(0x4010C0, 0xFFFFFFF0);
    WriteRegister(0x4010D0, 1);
    // the upper screen
    WriteRegister(0x400400, 0x1C2);
    WriteRegister(0x400404, 0xD1);
    WriteRegister(0x400408, 0x1C1);
    WriteRegister(0x40040C, 0x1C1);
    WriteRegister(0x400410, 0);
    WriteRegister(0x400414, 0xCF);
    WriteRegister(0x400418, 0xD1);
    WriteRegister(0x40041C, 0x1C501C1);
    WriteRegister(0x400420, 0x10000);
    WriteRegister(0x400424, 0x19D);
    WriteRegister(0x400428, 2);
    WriteRegister(0x40042C, 0x192);
    WriteRegister(0x400430, 0x192);
    WriteRegister(0x400434, 0x192);
    WriteRegister(0x400438, 1);
    WriteRegister(0x40043C, 2);
    WriteRegister(0x400440, 0x1960192);
    WriteRegister(0x400444, 0);
    WriteRegister(0x400448, 0);
    WriteRegister(0x40045C, 0x19000F0);
    WriteRegister(0x400460, 0x1C100D1);
    WriteRegister(0x400464, 0x1920002);
    WriteRegister(0x400470, 0x80340);
    WriteRegister(0x40049C, 0);
    // the lower screen
    WriteRegister(0x400500, 0x1C2);
    WriteRegister(0x400504, 0xD1);
    WriteRegister(0x400508, 0x1C1);
    WriteRegister(0x40050C, 0x1C1);
    WriteRegister(0x400510, 0xCD);
    WriteRegister(0x400514, 0xCF);
    WriteRegister(0x400518, 0xD1);
    WriteRegister(0x40051C, 0x1C501C1);
    WriteRegister(0x400520, 0x10000);
    WriteRegister(0x400524, 0x19D);
    WriteRegister(0x400528, 0x52);
    WriteRegister(0x40052C, 0x192);
    WriteRegister(0x400530, 0x192);
    WriteRegister(0x400534, 0x4F);
    WriteRegister(0x400538, 0x50);
    WriteRegister(0x40053C, 0x52);
    WriteRegister(0x400540, 0x1980194);
    WriteRegister(0x400544, 0);
    WriteRegister(0x400548, 0x11);
    WriteRegister(0x40055C, 0x14000F0);
    WriteRegister(0x400560, 0x1C100D1);
    WriteRegister(0x400564, 0x1920052);
    WriteRegister(0x400570, 0x80300);
    WriteRegister(0x40059C, 0);
    // the framebuffer addresses of both screens
    WriteRegister(0x400468, 0x18300000);
    WriteRegister(0x40046C, 0x18300000);
    WriteRegister(0x400568, 0x18300000);
    WriteRegister(0x40056C, 0x18300000);
    WriteRegister(0x400494, 0x18300000);
    WriteRegister(0x400498, 0x18300000);
    WriteRegister(0x400478, 1);
    WriteRegister(0x400578, 1);
    WriteRegisterWithMask(0x400C18, 0, 0xFF00);
    WriteRegister(0x400004, 0x70100);
    WriteRegisterWithMask(0x40001C, 0, 0xFF);
    WriteRegisterWithMask(0x40002C, 0, 0xFF);
    WriteRegister(0x400050, 0x22221200);
    WriteRegisterWithMask(0x400054, 0xFF2, 0xFFFF);
    WriteRegister(0x400474, 0x10501);
    WriteRegister(0x400574, 0x10501);
}
} // namespace

// 0x00121E40 | nintendogs:callgraph [tier A]
extern "C" GLboolean nngxInitialize(GxAllocateFunction pAllocate, GxFreeFunction pFree)
{
    GLuint name = 1;
    if (pAllocate == nullptr || pFree == nullptr || s_Context.m_IsInitialized) {
        return 0;
    }
    memset(&s_Context, 0, 0x180);
    s_Context.m_DisplayHeights[0] = 240;
    s_Context.m_DisplayHeights[1] = 240;
    s_Context.m_DisplayWidths[0] = 400;
    s_Context.m_DisplayWidths[1] = 320;
    s_Allocator.m_NextDisplaybufferName = 1;
    s_Allocator.m_NextCmdlistName = 1;
    s_Allocator.m_pAllocate = pAllocate;
    s_Allocator.m_pFree = pFree;
    s_Context.m_DisplayMode = DISPLAY_MODE_0;

    u8* pTemporary;
    bool isFirst;
    u8* pState = static_cast<u8*>(pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, STATE_MEMORY_SIZE));
    s_Allocator.m_pStateMemory = pState;
    if (pState == nullptr) {
        goto failed;
    }
    if (__sta_initializeState(pState, pState + STATE_VALIDATOR_OFFSET) < 0) {
        goto failed;
    }
    if (__fb_initializeFBManager(pState + FB_MANAGER_OFFSET) < 0) {
        goto failed;
    }
    if (__vb_initializeVBManager(pState + VB_MANAGER_OFFSET) < 0) {
        goto failed;
    }
    if (__shm_initializeShaderManager(pState + SHADER_MANAGER_OFFSET) < 0) {
        goto failed;
    }
    if (__tx_initializeTexManager(pState + TEXTURE_MANAGER_OFFSET) < 0) {
        goto failed;
    }
    glGetError();

    // a command list for the commands of the GL state and the first requests
    nngxBindCmdlist(name);
    CmdlistStorageInline(INITIAL_BUFFER_SIZE, INITIAL_REQUEST_COUNT);
    if (s_Context.m_pBoundCmdlist != nullptr) {
        s_Context.m_pBoundCmdlist->m_RunMode = RUN_MODE_AUTO;
    }
    if (glGetError() != 0) {
        goto failed;
    }
    if (__vldtr_initializeStateValidator(pState + STATE_VALIDATOR_OFFSET) < 0) {
        goto failed;
    }
    {
        Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
        u32 used = reinterpret_cast<u8*>(s_pCommandBufferCurrent) - pCmdlist->m_pBuffer;
        pCmdlist->m_UsedSize = used;
        s_InitialCommandSize = used;
    }

    nn::gxlow::CTR::Initialize();
    isFirst = nngxlowIsFirstInitialization();
    s_Context.m_MaxSpeculativeRequests = nngxlowGetNumSpeculativeRequests();
    nngxlowRegisterInterruptHandler(OnPsc0Interrupt, NN_GXLOW_INTERRUPT_PSC0);
    nngxlowRegisterInterruptHandler(OnPsc1Interrupt, NN_GXLOW_INTERRUPT_PSC1);
    nngxlowRegisterInterruptHandler(OnVBlankTopInterrupt, NN_GXLOW_INTERRUPT_VBLANK_TOP);
    nngxlowRegisterInterruptHandler(OnVBlankBottomInterrupt, NN_GXLOW_INTERRUPT_VBLANK_BOTTOM);
    nngxlowRegisterInterruptHandler(OnPpfInterrupt, NN_GXLOW_INTERRUPT_PPF);
    nngxlowRegisterInterruptHandler(OnP3dInterrupt, NN_GXLOW_INTERRUPT_P3D);
    nngxlowRegisterInterruptHandler(OnDmaInterrupt, NN_GXLOW_INTERRUPT_DMA);

    if (isFirst) {
        InitializeHardware();
        nngxSplitDrawCmdlist();
        // a display transfer from the end of VRAM B into a buffer of the process
        Cmdlist* pCmdlist = s_Context.m_pBoundCmdlist;
        CmdlistRequest* pRequest = &pCmdlist->m_pRequests[pCmdlist->m_RequestCount];
        pRequest->m_Type = 3;
        pRequest->m_Transfer.m_InputFormat = 4;
        pRequest->m_Transfer.m_Source = nngxGetVramEndAddr(MEMORY_AREA_VRAM_B) - FIRST_TRANSFER_SOURCE_OFFSET;
        if (s_Allocator.m_pAllocate == nullptr) {
            goto failed;
        }
        pTemporary = static_cast<u8*>(s_Allocator.m_pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, FIRST_TRANSFER_SIZE));
        if (pTemporary == nullptr) {
            goto failed;
        }
        pRequest->m_Transfer.m_Destination = reinterpret_cast<u32>(pTemporary);
        if ((reinterpret_cast<u32>(pTemporary) & 15) != 0) {
            pRequest->m_Transfer.m_Destination = reinterpret_cast<u32>(pTemporary) + 16 - (reinterpret_cast<u32>(pTemporary) & 15);
        }
        pRequest->m_Transfer.m_SourceWidth = FIRST_TRANSFER_EDGE;
        pRequest->m_Transfer.m_SourceHeight = FIRST_TRANSFER_EDGE;
        pRequest->m_Transfer.m_DestinationWidth = FIRST_TRANSFER_EDGE;
        pRequest->m_Transfer.m_DestinationHeight = FIRST_TRANSFER_EDGE;
        pRequest->m_Transfer.m_IsLinearOutput = false;
        pRequest->m_Transfer.m_IsTiledOutput = false;
        pRequest->m_Transfer.m_OutputFormat = 4;
        pRequest->m_Transfer.m_IsFlipped = false;
        pRequest->m_Transfer.m_Scaling = 0;
        pRequest->m_Transfer.m_IsBlock32 = false;
        pCmdlist->m_RequestCount++;
    } else {
        nngxSplitDrawCmdlist();
    }

    // the commands run, then the list is deleted again
    RunCmdlistInline();
    WaitCmdlistDoneInline();
    if (isFirst && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pTemporary);
    }
    StopCmdlistInline();
    nngxBindCmdlist(0);
    nngxDeleteCmdlists(1, &name);
    s_Context.m_IsInitialized = true;
    return 1;

failed:
    nngxDeleteCmdlists(1, &name);
    return 0;
}
