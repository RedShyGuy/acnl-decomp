#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/detail/detail_Api.h"

// The display buffers of the gx library (named objects in a hash table, one bound per display),
// the display mode and the global state of the library.

using nn::gx::detail::Displaybuffer;
using nn::gx::detail::s_Allocator;
using nn::gx::detail::s_Context;

namespace nn {
namespace gx {
namespace detail {
// 0x00AE08C0 (name is ours)
Context s_Context;
// 0x00975AAC (name is ours)
AllocatorState s_Allocator;
// 0x00975A9C (name is ours)
u32* s_pCommandBufferCurrent;
// 0x00975AA0 (name is ours)
u32* s_pCommandBufferEnd;
// 0x00975AA4 (name is ours)
GLenum s_Error;
// 0x0097E7A4 (name is ours)
GlState* s_pGlState;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the displays (nngxActiveDisplay; names are ours)
const GLenum DISPLAY_0 = 0x400;
const GLenum DISPLAY_1 = 0x401;
const GLenum DISPLAY_BOTH = 0x410;
// the parameters of nngxGetDisplaybufferParameteri (names are ours)
const GLenum DISPLAYBUFFER_ADDRESS = 0x600;
const GLenum DISPLAYBUFFER_FORMAT = 0x601;
const GLenum DISPLAYBUFFER_WIDTH = 0x602;
const GLenum DISPLAYBUFFER_HEIGHT = 0x603;
// the display modes of nngxSetDisplayMode (names are ours)
const GLenum DISPLAY_MODE_0 = 0x700;
const GLenum DISPLAY_MODE_1 = 0x701;
const GLenum DISPLAY_MODE_2 = 0x702;
// the formats of the display buffers (the OpenGL ES values)
const GLenum FORMAT_RGB8 = 0x8051;
const GLenum FORMAT_RGBA4 = 0x8056;
const GLenum FORMAT_RGB5_A1 = 0x8057;
const GLenum FORMAT_RGB565 = 0x8D62;
// the addresses of the VRAM areas (the start moves in applet mode)
const uptr VRAM_A_START = 0x1F000000;
const uptr VRAM_A_START_APPLET = 0x1F180000;
const uptr VRAM_A_END = 0x1F2FFFFF;
const uptr VRAM_B_START = 0x1F300000;
const uptr VRAM_B_START_APPLET = 0x1F380000;
const uptr VRAM_B_END = 0x1F5FFFFF;
// the 3D slider and the 3D LED in the shared page (3dbrew: 3D_SLIDERSTATE, 3D_LEDSTATE)
const uptr SHARED_PAGE_3D_SLIDER = 0x1FF81080;
const uptr SHARED_PAGE_3D_LED = 0x1FF81084;

// a new display buffer with the name in the hash table (sorted); false if the name is taken
inline bool InsertDisplaybuffer(Displaybuffer* pBuffer, GLuint name)
{
    Displaybuffer** ppBucket = &s_Context.m_pDisplaybuffers[name % nn::gx::detail::BUCKET_COUNT];
    Displaybuffer* pHead = *ppBucket;
    if (pHead == nullptr) {
        pBuffer->m_Name = name;
        pBuffer->m_pNext = nullptr;
        *ppBucket = pBuffer;
        return true;
    }
    if (pHead->m_Name == name) {
        return false;
    }
    if (pHead->m_Name > name) {
        pBuffer->m_Name = name;
        pBuffer->m_pNext = pHead;
        *ppBucket = pBuffer;
        return true;
    }
    Displaybuffer* pPrev = pHead;
    for (Displaybuffer* p = pHead->m_pNext; p != nullptr; p = p->m_pNext) {
        if (p->m_Name == name) {
            return false;
        }
        if (p->m_Name > name) {
            pBuffer->m_Name = name;
            pPrev->m_pNext = pBuffer;
            pBuffer->m_pNext = p;
            return true;
        }
        pPrev = p;
    }
    pPrev->m_pNext = pBuffer;
    pBuffer->m_Name = name;
    pBuffer->m_pNext = nullptr;
    return true;
}

// a new display buffer object (not in the hash table yet)
inline Displaybuffer* CreateDisplaybuffer()
{
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    Displaybuffer* p = pAllocate != nullptr
                           ? static_cast<Displaybuffer*>(pAllocate(nn::gx::detail::MEMORY_AREA_MAIN, nn::gx::detail::MEMORY_PURPOSE_OBJECT, 0, sizeof(Displaybuffer)))
                           : nullptr;
    p->m_Name = 0;
    p->m_Address = 0;
    p->m_Format = 0;
    p->m_Width = 0;
    p->m_Height = 0;
    p->m_Area = 0;
    p->m_pNext = nullptr;
    return p;
}
} // namespace

// 0x00127D94 | libgarden [tier A]
extern "C" GLenum glGetError()
{
    GLenum error = nn::gx::detail::s_Error;
    nn::gx::detail::s_Error = 0;
    return error;
}

// 0x007B1C64 | nintendogs:bytes [tier A]
extern "C" void nngxGetAllocator(GxAllocateFunction* pAllocate, GxFreeFunction* pFree)
{
    if (pAllocate != nullptr) {
        *pAllocate = s_Allocator.m_pAllocate;
    }
    if (pFree != nullptr) {
        *pFree = s_Allocator.m_pFree;
    }
}

// 0x007B1C88 | nintendogs:bytes [tier A]
extern "C" void nngxGetCommandGenerationMode(GLenum* pMode)
{
    *pMode = nn::gx::detail::s_pGlState->m_IsCommandGenerationDeferred ? 0x801 : 0x800;
}

// 0x007B1DE0 | libgarden [tier A]
extern "C" void nngxSetCommandGenerationMode(GLenum mode)
{
    if (mode == 0x800) {
        nn::gx::detail::s_pGlState->m_IsCommandGenerationDeferred = false;
    } else if (mode == 0x801) {
        nn::gx::detail::s_pGlState->m_IsCommandGenerationDeferred = true;
    }
}

// 0x001216A0 | nintendogs:bytes [tier A]
extern "C" void nngxActiveDisplay(GLenum display)
{
    if (display == DISPLAY_0) {
        s_Context.m_ActiveDisplay = 0;
    } else if (display == DISPLAY_1) {
        s_Context.m_ActiveDisplay = 1;
    } else if (display == DISPLAY_BOTH) {
        s_Context.m_ActiveDisplay = 2;
    }
}

// 0x00121C7C | nintendogs:bytes [tier A]
extern "C" void nngxGenDisplaybuffers(GLsizei n, GLuint* pNames)
{
    for (GLsizei i = 0; i < n; i++) {
        Displaybuffer* pBuffer = CreateDisplaybuffer();
        GLuint name = s_Allocator.m_NextDisplaybufferName;
        while (!InsertDisplaybuffer(pBuffer, name)) {
            name++;
        }
        pNames[i] = name;
        s_Allocator.m_NextDisplaybufferName = name + 1;
    }
}

// 0x00121894 | nintendogs:bytes [tier A]
extern "C" void nngxBindDisplaybuffer(GLuint name)
{
    Displaybuffer* pBuffer = s_Context.m_pDisplaybuffers[name % nn::gx::detail::BUCKET_COUNT];
    while (pBuffer != nullptr && pBuffer->m_Name != name) {
        pBuffer = pBuffer->m_pNext;
    }
    if (name != 0 && pBuffer == nullptr) {
        pBuffer = CreateDisplaybuffer();
        InsertDisplaybuffer(pBuffer, name);
    }
    s_Context.m_pBoundDisplaybuffers[s_Context.m_ActiveDisplay] = pBuffer;
}

// 0x00121A84 | nintendogs:callgraph [tier A]
extern "C" void nngxDisplaybufferStorage(GLenum format, GLsizei width, GLsizei height, GLenum area)
{
    Displaybuffer* pBuffer = s_Context.m_pBoundDisplaybuffers[s_Context.m_ActiveDisplay];
    u32 bytesPerPixel;
    if (format == FORMAT_RGB8) {
        bytesPerPixel = 3;
    } else if (format == FORMAT_RGBA4 || format == FORMAT_RGB5_A1 || format == FORMAT_RGB565) {
        bytesPerPixel = 2;
    } else {
        return;
    }
    if (area != nn::gx::detail::MEMORY_AREA_MAIN && area != nn::gx::detail::MEMORY_AREA_VRAM_A && area != nn::gx::detail::MEMORY_AREA_VRAM_B) {
        return;
    }
    if (pBuffer->m_Address != 0 && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(pBuffer->m_Area, nn::gx::detail::MEMORY_PURPOSE_DISPLAYBUFFER, pBuffer->m_Name, reinterpret_cast<void*>(pBuffer->m_Address));
    }
    void* pAddress = s_Allocator.m_pAllocate != nullptr
                         ? s_Allocator.m_pAllocate(area, nn::gx::detail::MEMORY_PURPOSE_DISPLAYBUFFER, pBuffer->m_Name, bytesPerPixel * width * height)
                         : nullptr;
    pBuffer->m_Area = area;
    pBuffer->m_Address = reinterpret_cast<u32>(pAddress);
    pBuffer->m_Format = format;
    pBuffer->m_Width = width;
    pBuffer->m_Height = height;
}

// 0x007B1CB0 | nintendogs:bytes [tier A]
extern "C" void nngxGetDisplaybufferParameteri(GLenum name, GLint* pParameter)
{
    Displaybuffer* pBuffer = s_Context.m_pBoundDisplaybuffers[s_Context.m_ActiveDisplay];
    if (pBuffer == nullptr) {
        return;
    }
    if (name == DISPLAYBUFFER_ADDRESS) {
        *pParameter = pBuffer->m_Address;
    } else if (name == DISPLAYBUFFER_FORMAT) {
        *pParameter = pBuffer->m_Format;
    } else if (name == DISPLAYBUFFER_WIDTH) {
        *pParameter = pBuffer->m_Width;
    } else if (name == DISPLAYBUFFER_HEIGHT) {
        *pParameter = pBuffer->m_Height;
    }
}

// 0x00122AE4 | nintendogs:bytes [tier A]
extern "C" void nngxSetDisplayMode(GLenum mode)
{
    if (s_Context.m_DisplayMode == mode) {
        return;
    }
    if (mode == DISPLAY_MODE_0) {
        s_Context.m_DisplayWidths[0] = 400;
    } else if (mode == DISPLAY_MODE_1) {
        s_Context.m_DisplayWidths[0] = 800;
    } else if (mode == DISPLAY_MODE_2) {
        s_Context.m_DisplayWidths[0] = 400;
    } else {
        return;
    }
    s_Context.m_DisplayMode = mode;
}

// 0x00121DF0 | nintendogs:bytes [tier A]
extern "C" uptr nngxGetVramStartAddr(GLenum area)
{
    if (area == nn::gx::detail::MEMORY_AREA_VRAM_A) {
        return nn::gxlow::CTR::detail::IsAppletMode() ? VRAM_A_START_APPLET : VRAM_A_START;
    }
    if (area == nn::gx::detail::MEMORY_AREA_VRAM_B) {
        return nn::gxlow::CTR::detail::IsAppletMode() ? VRAM_B_START_APPLET : VRAM_B_START;
    }
    return 0;
}

// 0x00121DB4 | nintendogs:bytes [tier A]
extern "C" uptr nngxGetVramEndAddr(GLenum area)
{
    // (the end is the same in applet mode; the original asks anyway)
    if (area == nn::gx::detail::MEMORY_AREA_VRAM_A) {
        nn::gxlow::CTR::detail::IsAppletMode();
        return VRAM_A_END;
    }
    if (area == nn::gx::detail::MEMORY_AREA_VRAM_B) {
        nn::gxlow::CTR::detail::IsAppletMode();
        return VRAM_B_END;
    }
    return 0;
}

// 0x007B1D08 | nintendogs:bytes [tier A]
extern "C" GLboolean nngxIsStereoVisionAllowed()
{
    // the 3D LED is off (no parental restriction) and the slider is up
    if (*reinterpret_cast<volatile u8*>(SHARED_PAGE_3D_LED) == 0 && *reinterpret_cast<volatile float*>(SHARED_PAGE_3D_SLIDER) > 0.0f) {
        return 1;
    }
    return 0;
}
