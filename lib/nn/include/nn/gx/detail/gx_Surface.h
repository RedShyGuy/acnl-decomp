#pragma once

// The surfaces of the GL functions (__srf_*): the images of the textures and the buffers of the
// framebuffers. The hardware formats are the texture formats of the PICA200 on 3dbrew (the render
// buffers use the color buffer formats 0-4 and the depth buffer formats 0, 2 and 3). All names are
// ours except __srf_initSurface.

#include "decomp.h"
#include "nn/gx/gx_Types.h"

namespace nn {
namespace gx {
namespace detail {
// the formats (the OpenGL ES values; 0x6040-0x6761 are extensions, names ours: the native formats
// take the data in the order of the GPU)
const GLenum FORMAT_ALPHA = 0x1906;
const GLenum FORMAT_RGB = 0x1907;
const GLenum FORMAT_RGBA = 0x1908;
const GLenum FORMAT_LUMINANCE = 0x1909;
const GLenum FORMAT_LUMINANCE_ALPHA = 0x190A;
const GLenum FORMAT_SHADOW = 0x6040; // (the texture unit gets the shadow type)
const GLenum FORMAT_UNKNOWN_0x6050 = 0x6050;
const GLenum FORMAT_HILO8 = 0x6700;
const GLenum FORMAT_NATIVE_RGBA = 0x6752;
const GLenum FORMAT_NATIVE_RGB = 0x6754;
const GLenum FORMAT_NATIVE_ALPHA = 0x6756;
const GLenum FORMAT_NATIVE_LUMINANCE = 0x6757;
const GLenum FORMAT_NATIVE_LUMINANCE_ALPHA = 0x6758;
const GLenum FORMAT_NATIVE_HILO8 = 0x6759;
const GLenum FORMAT_ETC1 = 0x675A;
const GLenum FORMAT_ETC1A4 = 0x675B;
const GLenum FORMAT_NATIVE_SHADOW = 0x675C;
const GLenum FORMAT_NATIVE_UNKNOWN_0x6050 = 0x675D;
const GLenum FORMAT_RGB8 = 0x8051;
const GLenum FORMAT_RGBA4 = 0x8056;
const GLenum FORMAT_RGB5_A1 = 0x8057;
const GLenum FORMAT_RGBA8 = 0x8058;
const GLenum FORMAT_DEPTH_COMPONENT16 = 0x81A5;
const GLenum FORMAT_DEPTH_COMPONENT24 = 0x81A6;
const GLenum FORMAT_DEPTH24_STENCIL8 = 0x88F0;
const GLenum FORMAT_RGB565 = 0x8D62;
// the types
const GLenum TYPE_UNSIGNED_BYTE = 0x1401;
const GLenum TYPE_UNSIGNED_SHORT = 0x1403;
const GLenum TYPE_UNSIGNED_BYTE_4_4 = 0x6760;
const GLenum TYPE_UNSIGNED_4BITS = 0x6761;
const GLenum TYPE_UNSIGNED_SHORT_4_4_4_4 = 0x8033;
const GLenum TYPE_UNSIGNED_SHORT_5_5_5_1 = 0x8034;
const GLenum TYPE_UNSIGNED_SHORT_5_6_5 = 0x8363;

// the hardware formats
const u32 HW_FORMAT_RGBA8 = 0;
const u32 HW_FORMAT_RGB8 = 1;
const u32 HW_FORMAT_RGB5A1 = 2;
const u32 HW_FORMAT_RGB565 = 3;
const u32 HW_FORMAT_RGBA4 = 4;
const u32 HW_FORMAT_IA8 = 5;
const u32 HW_FORMAT_HILO8 = 6;
const u32 HW_FORMAT_I8 = 7;
const u32 HW_FORMAT_A8 = 8;
const u32 HW_FORMAT_IA4 = 9;
const u32 HW_FORMAT_I4 = 10;
const u32 HW_FORMAT_A4 = 11;
const u32 HW_FORMAT_ETC1 = 12;
const u32 HW_FORMAT_ETC1A4 = 13;
const u32 HW_FORMAT_D16 = 0;
const u32 HW_FORMAT_D24 = 2;
const u32 HW_FORMAT_D24S8 = 3;

// the memory of a surface (0x44 bytes)
struct Surface
{
    u32 m_Address;          // 0x00 (what the GPU reads)
    u32 m_Copy;             // 0x04 (the copy in the main memory)
    u32 m_Source;           // 0x08 (the data of the caller)
    GLsizei m_Width;        // 0x0C
    GLsizei m_Height;       // 0x10
    GLenum m_Format;        // 0x14
    GLenum m_Type;          // 0x18
    u32 m_HwFormat;         // 0x1C
    u32 m_Size;             // 0x20 (all levels)
    u32 m_ComponentBits[4]; // 0x24 (red, green, blue, alpha; depth and stencil for the depth formats)
    s32 m_LevelCount;       // 0x34
    s32 m_BitsPerPixel;     // 0x38
    GLenum m_Area;          // 0x3C (the VRAM area of the address)
    GLuint m_Name;          // 0x40 (the name of the object)
};
ASSERT_SIZE(Surface, 0x44);

// the memory of a surface for a location (LOCATION_*) / its release
bool AllocateSurfaceMemory(u32 location, Surface* pSurface); // 0x007C3B24 (name is ours)
void ReleaseSurface(u32 location, Surface* pSurface); // 0x0013D8D8 (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
// the fields of a surface for a format and a type (the image size is that of a compressed format)
void __srf_initSurface(GLsizei width, GLsizei height, GLenum format, GLenum type, s32 levelCount, u32 imageSize,
                       nn::gx::detail::Surface* pSurface);
}
