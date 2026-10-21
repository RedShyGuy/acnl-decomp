#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include <cstring>

// The state of the GL functions (nngxUpdateState / nngxValidateState), the buffer swap and the
// waits for the command list and the VSync.

using nn::gx::detail::Displaybuffer;
using nn::gx::detail::GlState;
using nn::gx::detail::s_Context;
using namespace nn::gx::detail;

namespace nn {
namespace gx {
namespace detail {
// 0x00975AA8 (name is ours)
VertexState* s_pVertexState;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the displays of nngxSwapBuffers / nngxWaitVSync (names are ours)
const GLenum DISPLAY_0 = 0x400;
const GLenum DISPLAY_1 = 0x401;
const GLenum DISPLAY_BOTH = 0x402;
// the display modes (nngxSetDisplayMode)
const GLenum DISPLAY_MODE_0 = 0x700;
const GLenum DISPLAY_MODE_1 = 0x701;
const GLenum DISPLAY_MODE_2 = 0x702;
// the formats of the display buffers (the OpenGL ES values) and the framebuffer formats of the LCD
// (3dbrew: 0 RGBA8, 1 BGR8, 2 RGB565, 3 RGB5A1, 4 RGBA4)
const GLenum FORMAT_RGB8 = 0x8051;
const GLenum FORMAT_RGBA4 = 0x8056;
const GLenum FORMAT_RGB5_A1 = 0x8057;
const GLenum FORMAT_RGBA8 = 0x8058;
const GLenum FORMAT_RGB565 = 0x8D62;
// the other bits of the framebuffer format (names are ours)
const u32 FRAMEBUFFER_STEREO = 0x20;
const u32 FRAMEBUFFER_MONO = 0x40;
const u32 FRAMEBUFFER_FROM_MAIN = 0x100;
const u32 FRAMEBUFFER_FROM_VRAM = 0x300;
} // namespace

extern "C" {
// the validators of the GL state (names from the symbols)
void __txv_validateTextureValidator(const u32* pDirtyFlags); // 0x007A0EB0
void __fbv_validateFBValidator(const u32* pDirtyFlags); // 0x0077A894
void __vbv_validateVBValidator(GlState* pState); // 0x001356EC
void __shv_partialValidateShaderValidator(GlState* pState, GLbitfield states); // 0x0077C890
}

// 0x007B2138 | libgarden [tier A]
extern "C" void nngxUpdateState(GLbitfield states)
{
    GlState* pState = nn::gx::detail::s_pGlState;
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
        memset(pState->m_StateBlock0, 0, sizeof(pState->m_StateBlock0));
        memset(pState->m_StateBlock1, 0, sizeof(pState->m_StateBlock1));
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
}

// the commands of the updated states; the error of the validation
// 0x007B27B4 | libgarden [tier A]
extern "C" GLenum nngxValidateState(GLbitfield states, GLboolean isPartial)
{
    GlState* pState = nn::gx::detail::s_pGlState;
    glGetError();
    GLbitfield validate = states & ~pState->m_IgnoredStates;
    if ((pState->m_UpdatedStates & validate & STATE_VIEWPORT) != 0) {
        nn::gx::detail::ValidateViewport();
    }
    if ((validate & STATE_TEXTURE) != 0) {
        u32 dirty = DIRTY_TEXTURE;
        if ((pState->m_DirtyFlags & DIRTY_TEXTURE) != 0) {
            __txv_validateTextureValidator(&dirty);
        }
    }
    if ((validate & STATE_FRAMEBUFFER) != 0) {
        u32 dirty = DIRTY_FRAMEBUFFER;
        if ((pState->m_DirtyFlags & DIRTY_FRAMEBUFFER) != 0) {
            __fbv_validateFBValidator(&dirty);
        }
    }
    if ((validate & STATE_VERTEX) != 0 && (nn::gx::detail::s_pVertexState->m_DirtyMask & pState->m_DirtyFlags) != 0) {
        __vbv_validateVBValidator(pState);
    }
    if ((validate & STATE_SHADER_MASK) != 0) {
        pState->m_IsValidating = true;
        if (!isPartial) {
            pState->m_IsFullValidation = true;
        }
        __shv_partialValidateShaderValidator(pState, validate);
        pState->m_IsValidating = false;
        pState->m_IsFullValidation = false;
    }
    if ((validate & STATE_0) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_0;
    }
    if ((validate & STATE_2) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_2;
    }
    if ((validate & STATE_3) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_3;
    }
    if ((validate & STATE_4) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_4;
    }
    if ((validate & STATE_5) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_5;
    }
    if ((validate & STATE_6) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_6;
    }
    if ((validate & STATE_TEXTURE) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_TEXTURE;
    }
    if ((validate & STATE_FRAMEBUFFER) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_FRAMEBUFFER;
    }
    if ((validate & STATE_VERTEX) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_VERTEX;
    }
    if ((validate & STATE_10) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_10;
    }
    if ((validate & STATE_11) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_11;
    }
    if ((validate & STATE_12) != 0) {
        pState->m_DirtyFlags &= ~DIRTY_12;
    }
    pState->m_UpdatedStates &= ~validate;
    return glGetError();
}

// the bound display buffers of the displays become the shown framebuffers (at the next VSync)
// 0x007B1F9C | nintendogs:bytes [tier A]
extern "C" void nngxSwapBuffers(GLenum display)
{
    for (s32 i = 0; i < 2; i++) {
        if (display != DISPLAY_0 + i && display != DISPLAY_BOTH) {
            continue;
        }
        Displaybuffer* pBuffer = s_Context.m_pBoundDisplaybuffers[i];
        u32 bytesPerPixel;
        u32 format;
        switch (pBuffer->m_Format) {
        case FORMAT_RGB5_A1:
            bytesPerPixel = 2;
            format = 3;
            break;
        case FORMAT_RGB8:
            bytesPerPixel = 3;
            format = 1;
            break;
        case FORMAT_RGBA4:
            bytesPerPixel = 2;
            format = 4;
            break;
        case FORMAT_RGBA8:
            bytesPerPixel = 4;
            format = 0;
            break;
        case FORMAT_RGB565:
            bytesPerPixel = 2;
            format = 2;
            break;
        default:
            format = 0;
            bytesPerPixel = 0;
            break;
        }
        // the shown part of the buffer (the buffer is turned: it starts at the bottom)
        u32 left = pBuffer->m_Address +
                   bytesPerPixel * (pBuffer->m_Width * (pBuffer->m_Height - s_Context.m_DisplayWidths[i] - s_Context.m_DisplayOffsetsY[i]) + s_Context.m_DisplayOffsetsX[i]);
        u32 stride = pBuffer->m_Width * bytesPerPixel;
        format |= pBuffer->m_Area == nn::gx::detail::MEMORY_AREA_MAIN ? FRAMEBUFFER_FROM_MAIN : FRAMEBUFFER_FROM_VRAM;
        u32 right = 0;
        u32 mode;
        if (i != 0) {
            mode = format;
        } else if (s_Context.m_DisplayMode == DISPLAY_MODE_1) {
            mode = format;
        } else if (s_Context.m_DisplayMode == DISPLAY_MODE_2) {
            // the right eye of the upper screen
            mode = format | FRAMEBUFFER_STEREO;
            Displaybuffer* pRight = s_Context.m_pBoundDisplaybuffers[2];
            right = pRight->m_Address +
                    bytesPerPixel * (pRight->m_Width * (pRight->m_Height - s_Context.m_DisplayWidths[0] - s_Context.m_DisplayOffsetsY[2]) + s_Context.m_DisplayOffsetsX[2]);
        } else {
            mode = format | FRAMEBUFFER_MONO;
        }
        nngxlowSetBufferSwap(i, s_Context.m_ActiveFramebuffers[i], reinterpret_cast<void*>(left), reinterpret_cast<void*>(right), stride, mode,
                             s_Context.m_ActiveFramebuffers[i]);
        s_Context.m_ActiveFramebuffers[i] = s_Context.m_ActiveFramebuffers[i] == 0 ? 1 : 0;
    }
}

// the data at the address is written back from the cache
// 0x007B2134 | mk7dlp:callgraph [tier C]
extern "C" void nngxUpdateBuffer(const GLvoid* pData, GLsizei size)
{
    GSPGPU_FlushDataCache(reinterpret_cast<unsigned>(pData), size);
}

// waits until the running command list is done (with the timeout: its callback)
// 0x007B2998 | nintendogs:callseq-callee [tier A]
extern "C" void nngxWaitCmdlistDone()
{
    nn::gx::detail::WaitCmdlistDoneInline();
}

// waits for the next VSync of the display (both: of each one) without a pending buffer swap
// 0x007B2A34 | fefates:bytes [tier B]
extern "C" void nngxWaitVSync(GLenum display)
{
    if (display == DISPLAY_BOTH) {
        s32 count0 = s_Context.m_VSyncCounts[0];
        s32 count1 = s_Context.m_VSyncCounts[1];
        s32 now0;
        s32 now1;
        do {
            nngxlowYieldThread();
            now0 = s_Context.m_VSyncCounts[0];
            now1 = s_Context.m_VSyncCounts[1];
            if (count0 != now0 && nngxlowIsSwapPending(0)) {
                count0 = now0;
            }
            if (count1 != now1 && nngxlowIsSwapPending(1)) {
                count1 = now1;
            }
        } while (count0 == now0 || count1 == now1);
        return;
    }
    s32 index = display == DISPLAY_0 ? 0 : 1;
    s32 count = s_Context.m_VSyncCounts[index];
    for (;;) {
        nngxlowYieldThread();
        s32 now = s_Context.m_VSyncCounts[index];
        if (count == now) {
            continue;
        }
        if (!nngxlowIsSwapPending(index)) {
            break;
        }
        count = now;
    }
}
