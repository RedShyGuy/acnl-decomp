#include "nn/gx/detail/gx_Surface.h"
#include "nn/gx/gx_Api.h"
#include <cstdlib>

// The surfaces of the GL functions: their fields for a format and their memory.

using namespace nn::gx::detail;

namespace {
inline void SetComponents(Surface* pSurface, u32 hwFormat, u32 red, u32 green, u32 blue, u32 alpha, s32 bitsPerPixel)
{
    pSurface->m_HwFormat = hwFormat;
    pSurface->m_ComponentBits[0] = red;
    pSurface->m_ComponentBits[1] = green;
    pSurface->m_ComponentBits[2] = blue;
    pSurface->m_ComponentBits[3] = alpha;
    pSurface->m_BitsPerPixel = bitsPerPixel;
}
} // namespace

extern "C" {
// (the size counts the levels down to 8x8 pixels; a compressed format has the image size)
// 0x00783DA4 | nintendogs:bytes [tier B]
void __srf_initSurface(GLsizei width, GLsizei height, GLenum format, GLenum type, s32 levelCount, u32 imageSize, Surface* pSurface)
{
    if (pSurface->m_Address != 0 || pSurface->m_Copy != 0 || pSurface->m_Source != 0) {
        abort();
    }
    pSurface->m_Area = 0;
    pSurface->m_Height = height;
    pSurface->m_LevelCount = levelCount;
    pSurface->m_Width = width;
    pSurface->m_Format = format;
    pSurface->m_Type = type;
    switch (format) {
    case FORMAT_RGBA:
        switch (type) {
        case TYPE_UNSIGNED_BYTE:
            SetComponents(pSurface, HW_FORMAT_RGBA8, 8, 8, 8, 8, 32);
            break;
        case TYPE_UNSIGNED_SHORT_4_4_4_4:
            SetComponents(pSurface, HW_FORMAT_RGBA4, 4, 4, 4, 4, 16);
            break;
        case TYPE_UNSIGNED_SHORT_5_5_5_1:
            SetComponents(pSurface, HW_FORMAT_RGB5A1, 5, 5, 5, 1, 16);
            break;
        default:
            break;
        }
        break;
    case FORMAT_RGB:
        switch (type) {
        case TYPE_UNSIGNED_BYTE:
            SetComponents(pSurface, HW_FORMAT_RGB8, 8, 8, 8, 0, 24);
            break;
        case TYPE_UNSIGNED_SHORT_5_6_5:
            SetComponents(pSurface, HW_FORMAT_RGB565, 5, 6, 5, 0, 16);
            break;
        default:
            break;
        }
        break;
    case FORMAT_ALPHA:
        switch (type) {
        case TYPE_UNSIGNED_BYTE:
            SetComponents(pSurface, HW_FORMAT_A8, 0, 0, 0, 8, 8);
            break;
        case TYPE_UNSIGNED_4BITS:
            SetComponents(pSurface, HW_FORMAT_A4, 0, 0, 0, 4, 4);
            break;
        default:
            break;
        }
        break;
    case FORMAT_LUMINANCE:
        switch (type) {
        case TYPE_UNSIGNED_BYTE:
            SetComponents(pSurface, HW_FORMAT_I8, 8, 8, 8, 0, 8);
            break;
        case TYPE_UNSIGNED_4BITS:
            SetComponents(pSurface, HW_FORMAT_I4, 4, 4, 4, 0, 4);
            break;
        default:
            break;
        }
        break;
    case FORMAT_LUMINANCE_ALPHA:
        switch (type) {
        case TYPE_UNSIGNED_BYTE:
            SetComponents(pSurface, HW_FORMAT_IA8, 8, 8, 8, 8, 16);
            break;
        case TYPE_UNSIGNED_BYTE_4_4:
            SetComponents(pSurface, HW_FORMAT_IA4, 4, 4, 4, 4, 8);
            break;
        default:
            break;
        }
        break;
    case FORMAT_SHADOW:
    case FORMAT_UNKNOWN_0x6050:
    case FORMAT_RGBA8:
        SetComponents(pSurface, HW_FORMAT_RGBA8, 8, 8, 8, 8, 32);
        break;
    case FORMAT_HILO8:
        SetComponents(pSurface, HW_FORMAT_HILO8, 8, 8, 0, 0, 16);
        break;
    case FORMAT_ETC1:
        SetComponents(pSurface, HW_FORMAT_ETC1, 8, 8, 8, 0, 0);
        pSurface->m_Size = imageSize;
        break;
    case FORMAT_ETC1A4:
        SetComponents(pSurface, HW_FORMAT_ETC1A4, 8, 8, 8, 4, 0);
        pSurface->m_Size = imageSize;
        break;
    case FORMAT_DEPTH_COMPONENT16:
        SetComponents(pSurface, HW_FORMAT_D16, 16, 0, 0, 0, 16);
        break;
    case FORMAT_DEPTH_COMPONENT24:
        SetComponents(pSurface, HW_FORMAT_D24, 24, 0, 0, 0, 24);
        break;
    case FORMAT_DEPTH24_STENCIL8:
        SetComponents(pSurface, HW_FORMAT_D24S8, 24, 8, 0, 0, 32);
        break;
    case FORMAT_RGB565:
        SetComponents(pSurface, HW_FORMAT_RGB565, 5, 6, 5, 0, 16);
        pSurface->m_Format = FORMAT_RGB;
        pSurface->m_Type = TYPE_UNSIGNED_SHORT_5_6_5;
        break;
    case FORMAT_RGBA4:
        SetComponents(pSurface, HW_FORMAT_RGBA4, 4, 4, 4, 4, 16);
        pSurface->m_Format = FORMAT_RGBA;
        pSurface->m_Type = TYPE_UNSIGNED_SHORT_4_4_4_4;
        break;
    case FORMAT_RGB5_A1:
        SetComponents(pSurface, HW_FORMAT_RGB5A1, 5, 5, 5, 1, 16);
        pSurface->m_Format = FORMAT_RGBA;
        pSurface->m_Type = TYPE_UNSIGNED_SHORT_5_5_5_1;
        break;
    case FORMAT_RGB8:
        SetComponents(pSurface, HW_FORMAT_RGB8, 8, 8, 8, 0, 24);
        break;
    default:
        break;
    }
    if (format == FORMAT_ETC1 || format == FORMAT_ETC1A4) {
        return;
    }
    pSurface->m_Size = 0;
    for (s32 i = 0; i < levelCount && width >= 8 && height >= 8; i++) {
        pSurface->m_Size += width * height * pSurface->m_BitsPerPixel / 8;
        width >>= 1;
        height >>= 1;
    }
}
} // extern "C"

namespace nn {
namespace gx {
namespace detail {
// 0x007C3B24 (name is ours)
bool AllocateSurfaceMemory(u32 location, Surface* pSurface)
{
    bool isAllocated = true;
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    switch (location) {
    case LOCATION_MAIN: {
        void* p = pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, pSurface->m_Size) : nullptr;
        if (p == nullptr) {
            isAllocated = false;
        }
        pSurface->m_Copy = reinterpret_cast<u32>(p);
        pSurface->m_Address = reinterpret_cast<u32>(p);
        break;
    }
    case LOCATION_VRAM_A_WITH_COPY:
    case LOCATION_VRAM_B_WITH_COPY: {
        GLenum area = location == LOCATION_VRAM_A_WITH_COPY ? MEMORY_AREA_VRAM_A : MEMORY_AREA_VRAM_B;
        pSurface->m_Area = area;
        pSurface->m_Address = reinterpret_cast<u32>(pAllocate != nullptr ? pAllocate(area, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, pSurface->m_Size) : nullptr);
        pAllocate = s_Allocator.m_pAllocate;
        pSurface->m_Copy = reinterpret_cast<u32>(pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, pSurface->m_Size) : nullptr);
        if (pSurface->m_Address == 0 || pSurface->m_Copy == 0) {
            // both or none
            isAllocated = false;
            if (pSurface->m_Address != 0 && s_Allocator.m_pFree != nullptr) {
                s_Allocator.m_pFree(pSurface->m_Area, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Address));
            }
            if (pSurface->m_Copy != 0 && s_Allocator.m_pFree != nullptr) {
                s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Copy));
            }
        }
        break;
    }
    case LOCATION_VRAM_A:
    case LOCATION_VRAM_B: {
        GLenum area = location == LOCATION_VRAM_A ? MEMORY_AREA_VRAM_A : MEMORY_AREA_VRAM_B;
        pSurface->m_Area = area;
        void* p = pAllocate != nullptr ? pAllocate(area, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, pSurface->m_Size) : nullptr;
        if (p == nullptr) {
            isAllocated = false;
        }
        pSurface->m_Address = reinterpret_cast<u32>(p);
        break;
    }
    default:
        break;
    }
    return isAllocated;
}

// 0x0013D8D8 (name is ours)
void ReleaseSurface(u32 location, Surface* pSurface)
{
    GxFreeFunction pFree = s_Allocator.m_pFree;
    switch (location) {
    case LOCATION_MAIN:
        if (pSurface->m_Copy != 0 && pFree != nullptr) {
            pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Copy));
        }
        break;
    case LOCATION_VRAM_A_WITH_COPY:
    case LOCATION_VRAM_B_WITH_COPY:
        if (pSurface->m_Address != 0 && pFree != nullptr) {
            pFree(pSurface->m_Area, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Address));
        }
        if (pSurface->m_Copy != 0 && s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Copy));
        }
        break;
    case LOCATION_VRAM_A:
    case LOCATION_VRAM_B:
        if (pSurface->m_Address != 0 && pFree != nullptr) {
            pFree(pSurface->m_Area, MEMORY_PURPOSE_TEXTURE, pSurface->m_Name, reinterpret_cast<void*>(pSurface->m_Address));
        }
        break;
    default:
        break;
    }
    pSurface->m_Address = 0;
    pSurface->m_Copy = 0;
    pSurface->m_Source = 0;
}
} // namespace detail
} // namespace gx
} // namespace nn
