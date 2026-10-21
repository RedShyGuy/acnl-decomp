#include "nn/gx/detail/gx_Framebuffer.h"
#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include <cstring>

// The framebuffer manager of the GL functions.

using namespace nn::gx::detail;

namespace {
// the states of a framebuffer (the OpenGL ES values)
const GLenum STATUS_COMPLETE = 0x8CD5;
const GLenum STATUS_INCOMPLETE_ATTACHMENT = 0x8CD6;
const GLenum STATUS_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
const GLenum STATUS_INCOMPLETE_DIMENSIONS = 0x8CD9;
// the dirty flags of the polygon offset and of the scissor
const u32 DIRTY_POLYGON_OFFSET = 0x4;
const u32 DIRTY_SCISSOR = 0x200;
// the commands (register | mask << 16; the registers after 3dbrew)
const u32 COMMAND_RENDERBUFFER_DIMENSION = 0xF006E;
const u32 COMMAND_FRAMEBUFFER_INVALIDATE = 0xF0110;
const u32 COMMAND_FRAMEBUFFER_FLUSH = 0xF0111;
const u32 COMMAND_DEPTHBUFFER_FORMAT = 0xF0116;
const u32 COMMAND_COLORBUFFER_FORMAT = 0xF0117;
const u32 COMMAND_DEPTHBUFFER_LOCATION = 0xF011C;
const u32 COMMAND_COLORBUFFER_LOCATION = 0xF011D;
const u32 COMMAND_FRAMEBUFFER_DIMENSION = 0xF011E;

// a surface with memory and a size
inline bool IsSurfaceReady(const Surface* pSurface)
{
    return pSurface->m_Address != 0 && pSurface->m_Width != 0 && pSurface->m_Height != 0;
}

// the location of a buffer (the physical address is only looked up if the command fits)
inline void PutLocationCommand(u32 address, u32 header)
{
    if (s_pCommandBufferCurrent < s_pCommandBufferEnd) {
        u32 value = nngxlowGetPhysicalAddr(address) >> 3;
        u32* p = s_pCommandBufferCurrent;
        p[0] = value;
        p[1] = header;
        s_pCommandBufferCurrent = p + 2;
    }
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x0097E48C (name is ours)
FramebufferManager* s_pFramebufferManager;

// 0x0012E4E4 (name is ours)
Framebuffer* CreateFramebuffer()
{
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    if (pAllocate == nullptr) {
        return nullptr;
    }
    Framebuffer* p = static_cast<Framebuffer*>(pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, sizeof(Framebuffer)));
    if (p != nullptr) {
        memset(p, 0, sizeof(Framebuffer));
    }
    return p;
}
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
// (the manager starts with the default framebuffer, which is the current one)
// 0x00126DB8 | nintendogs:bytes [tier A]
int __fb_initializeFBManager(void* pMemory)
{
    s_pFramebufferManager = static_cast<FramebufferManager*>(pMemory);
    Framebuffer* pFramebuffer = CreateFramebuffer();
    if (pFramebuffer == nullptr) {
        return -1;
    }
    pFramebuffer->m_Unknown0x20 = 0;
    pFramebuffer->m_pNext = nullptr;
    FramebufferManager* pManager = s_pFramebufferManager;
    pManager->m_pFramebuffers = pFramebuffer;
    pManager->m_Unknown0x4 = 0;
    pManager->m_pCurrent = pFramebuffer;
    pManager->m_Unknown0xC = 0;
    return 0;
}

// 0x0077A864 | nintendogs:bytes [tier B]
Surface* __fb_getCurrentFBColorSurface()
{
    return s_pFramebufferManager->m_pCurrent->m_Attachments[ATTACHMENT_COLOR].m_pSurface;
}

// 0x0077A87C | nintendogs:bytes [tier B]
Surface* __fb_getCurrentFBDepthSurface()
{
    return s_pFramebufferManager->m_pCurrent->m_Attachments[ATTACHMENT_DEPTH].m_pSurface;
}

// (the framebuffer state is dirty if the current framebuffer loses it)
// 0x0013D848 | nintendogs:bytes [tier B]
void __fb_detachSurfaceContainer(Surface* pSurface)
{
    if (pSurface == nullptr) {
        return;
    }
    FramebufferManager* pManager = s_pFramebufferManager;
    for (Framebuffer* p = pManager->m_pFramebuffers; p != nullptr; p = p->m_pNext) {
        for (s32 i = 0; i < static_cast<s32>(ATTACHMENT_COUNT); i++) {
            FramebufferAttachment* pAttachment = &p->m_Attachments[i];
            if (pAttachment->m_pSurface != pSurface) {
                continue;
            }
            if (pManager->m_pCurrent == p) {
                s_pGlState->m_DirtyFlags |= DIRTY_FRAMEBUFFER;
            }
            pAttachment->m_pSurface = nullptr;
            pAttachment->_unknown0x0[0] = 0;
            pAttachment->_unknown0x0[1] = 0;
            pAttachment->_unknown0x0[2] = 0;
        }
    }
}

// the color and depth buffers of the current framebuffer (if it is complete) become commands; the
// viewport is its size if none was set
// 0x0077A894 | nintendogs:bytes [tier B]
void __fbv_validateFBValidator(const u32* pDirtyFlags)
{
    Framebuffer* pFramebuffer = s_pFramebufferManager->m_pCurrent;
    GlState* pState = s_pGlState;
    Surface* pColor = pFramebuffer->m_Attachments[ATTACHMENT_COLOR].m_pSurface;
    GLsizei width = 0;
    GLsizei height = 0;
    GLenum status;
    if (pColor == nullptr && pFramebuffer->m_Attachments[ATTACHMENT_DEPTH].m_pSurface == nullptr) {
        status = STATUS_INCOMPLETE_MISSING_ATTACHMENT;
    } else {
        status = STATUS_INCOMPLETE_ATTACHMENT;
        Surface* pDepth = pFramebuffer->m_Attachments[ATTACHMENT_DEPTH].m_pSurface;
        if (pColor != nullptr) {
            if (!IsSurfaceReady(pColor)) {
                goto done;
            }
            width = pColor->m_Width;
            height = pColor->m_Height;
        }
        if (pDepth != nullptr) {
            if (!IsSurfaceReady(pDepth)) {
                goto done;
            }
            if (width != 0 && (pDepth->m_Width != width || pDepth->m_Height != height)) {
                status = STATUS_INCOMPLETE_DIMENSIONS;
                goto done;
            }
        }
        if (pColor != pDepth) {
            status = STATUS_COMPLETE;
        }
    }
done:
    if (status != STATUS_COMPLETE) {
        pState->m_FramebufferStatus = 0;
        pState->m_HasColorBuffer = false;
        pState->m_HasDepthBuffer = false;
        pState->m_HasStencilBuffer = false;
        return;
    }
    pState->m_FramebufferStatus = status;
    if ((pState->m_IgnoredStates & STATE_FRAMEBUFFER) != 0) {
        return;
    }
    pColor = pFramebuffer->m_Attachments[ATTACHMENT_COLOR].m_pSurface;
    Surface* pDepth = pFramebuffer->m_Attachments[ATTACHMENT_DEPTH].m_pSurface;
    width = 0;
    height = 0;
    // a new buffer: the old ones are flushed and invalidated first
    if ((pColor != nullptr && pState->m_ColorAddress != pColor->m_Address) || (pDepth != nullptr && pState->m_DepthAddress != pDepth->m_Address) ||
        (pState->m_UpdatedStates & STATE_FRAMEBUFFER) != 0) {
        PutCommand(1, COMMAND_FRAMEBUFFER_FLUSH);
        PutCommand(1, COMMAND_FRAMEBUFFER_INVALIDATE);
    }
    if (pFramebuffer->m_Attachments[ATTACHMENT_COLOR].m_pSurface == nullptr) {
        pState->m_HasColorBuffer = false;
    } else {
        pState->m_HasColorBuffer = true;
        pState->m_ColorAddress = pColor->m_Address;
        pState->m_ColorSize = pColor->m_Size;
        switch (pColor->m_HwFormat) {
        case HW_FORMAT_RGBA8:
            pState->m_ColorFormat = HW_FORMAT_RGBA8;
            pState->m_ColorPixelSize = 2;
            break;
        case HW_FORMAT_RGB5A1:
            pState->m_ColorFormat = HW_FORMAT_RGB5A1;
            pState->m_ColorPixelSize = 0;
            break;
        case HW_FORMAT_RGB565:
            pState->m_ColorFormat = HW_FORMAT_RGB565;
            pState->m_ColorPixelSize = 0;
            break;
        case HW_FORMAT_RGBA4:
            pState->m_ColorFormat = HW_FORMAT_RGBA4;
            pState->m_ColorPixelSize = 0;
            break;
        default:
            break;
        }
        PutCommand(pState->m_ColorPixelSize | (pState->m_ColorFormat << 16), COMMAND_COLORBUFFER_FORMAT);
        PutLocationCommand(pState->m_ColorAddress, COMMAND_COLORBUFFER_LOCATION);
        width = pColor->m_Width;
        height = pColor->m_Height;
    }
    if (pFramebuffer->m_Attachments[ATTACHMENT_DEPTH].m_pSurface == nullptr) {
        pState->m_HasDepthBuffer = false;
        pState->m_HasStencilBuffer = false;
    } else {
        u32 oldFormat = pState->m_DepthFormat;
        pState->m_HasDepthBuffer = true;
        pState->m_DepthAddress = pDepth->m_Address;
        switch (pDepth->m_BitsPerPixel) {
        case 16:
            pState->m_DepthFormatRegister = HW_FORMAT_D16;
            pState->m_DepthFormat = HW_FORMAT_D16;
            break;
        case 24:
            pState->m_DepthFormatRegister = HW_FORMAT_D24;
            pState->m_DepthFormat = HW_FORMAT_D24;
            break;
        case 32:
            pState->m_DepthFormatRegister = HW_FORMAT_D24S8;
            pState->m_DepthFormat = HW_FORMAT_D24S8;
            break;
        default:
            break;
        }
        // the polygon offset depends on a 16 bit depth buffer
        if (pState->m_IsPolygonOffsetFillEnabled && (pState->m_DepthFormat == HW_FORMAT_D16) != (oldFormat == HW_FORMAT_D16)) {
            pState->m_DirtyFlags |= DIRTY_POLYGON_OFFSET;
        }
        pState->m_HasStencilBuffer = pState->m_DepthFormat == HW_FORMAT_D24S8;
        pState->m_DepthSize = pDepth->m_Size;
        PutCommand(pState->m_DepthFormatRegister, COMMAND_DEPTHBUFFER_FORMAT);
        PutLocationCommand(pState->m_DepthAddress, COMMAND_DEPTHBUFFER_LOCATION);
        width = pDepth->m_Width;
        height = pDepth->m_Height;
    }
    if (pState->m_FramebufferWidth != width || pState->m_FramebufferHeight != height || (pState->m_UpdatedStates & STATE_FRAMEBUFFER) != 0) {
        u32 dimension = (width & 0xFFF) | (((height - 1) & ~0xFF000) << 12);
        PutCommand(dimension | 0x1000000, COMMAND_FRAMEBUFFER_DIMENSION);
        PutCommand(dimension | 0x1000000, COMMAND_RENDERBUFFER_DIMENSION);
        pState->m_FramebufferWidth = width;
        pState->m_FramebufferHeight = height;
        if (pState->m_IsScissorTestEnabled) {
            pState->m_DirtyFlags |= DIRTY_SCISSOR;
        }
    }
    if (!pState->m_IsViewportSet) {
        pState->m_IsViewportSet = true;
        glViewport(0, 0, width, height);
    }
}
} // extern "C"
