#include "nn/gx/gx_Api.h"
#include "nw/font/font_ResourceFormat.h"
#include "nw/font/internal/internal_Api.h"

namespace nw {
namespace font {
namespace internal {
namespace {
// the values of the OpenGL ES API and its DMP extensions (names are ours)
const GLenum TEXTURE_2D = 0x0DE1;
// the image stays where it is (in FCRAM, not copied)
const GLenum TEXTURE_2D_FCRAM_NO_COPY = 0x02010000 | TEXTURE_2D;
const GLenum TEXTURE_MAG_FILTER = 0x2800;
const GLenum TEXTURE_MIN_FILTER = 0x2801;
const GLenum TEXTURE_WRAP_S = 0x2802;
const GLenum TEXTURE_WRAP_T = 0x2803;
const GLenum TEXTURE_LOD_BIAS = 0x8501;
const GLenum TEXTURE_MIN_LOD = 0x813A;
const GLint NEAREST = 0x2600;
const GLint LINEAR = 0x2601;
const GLint CLAMP_TO_EDGE = 0x812F;
const GLenum FORMAT_RGBA_NATIVE = 0x6752;
const GLenum FORMAT_RGB_NATIVE = 0x6754;
const GLenum FORMAT_ALPHA_NATIVE = 0x6756;
const GLenum FORMAT_LUMINANCE_ALPHA_NATIVE = 0x6758;
const GLenum TYPE_UNSIGNED_BYTE = 0x1401;
const GLenum TYPE_UNSIGNED_SHORT_4_4_4_4 = 0x8033;
const GLenum TYPE_UNSIGNED_SHORT_5_5_5_1 = 0x8034;
const GLenum TYPE_UNSIGNED_SHORT_5_6_5 = 0x8363;
const GLenum TYPE_UNSIGNED_BYTE_4_4 = 0x6760;
const GLenum TYPE_UNSIGNED_4BITS = 0x6761;
// the lowest mipmap level (no mipmaps)
const GLint MIN_LOD = -1000;
} // namespace

// 0x004D97A4 | nintendogs:bytes-fuzzy [tier A]
void LoadTexture(u16 width, u16 height, u16 format, const void* pImage, bool isSmallLinear, bool isLargeLinear)
{
    GLenum glFormat = FORMAT_ALPHA_NATIVE;
    GLenum glType = TYPE_UNSIGNED_BYTE;
    switch (format) {
    case FONT_SHEET_FORMAT_RGBA8:
        glFormat = FORMAT_RGBA_NATIVE;
        break;
    case FONT_SHEET_FORMAT_RGB8:
        glFormat = FORMAT_RGB_NATIVE;
        break;
    case FONT_SHEET_FORMAT_RGBA5551:
        glFormat = FORMAT_RGBA_NATIVE;
        glType = TYPE_UNSIGNED_SHORT_5_5_5_1;
        break;
    case FONT_SHEET_FORMAT_RGB565:
        glFormat = FORMAT_RGB_NATIVE;
        glType = TYPE_UNSIGNED_SHORT_5_6_5;
        break;
    case FONT_SHEET_FORMAT_RGBA4:
        glFormat = FORMAT_RGBA_NATIVE;
        glType = TYPE_UNSIGNED_SHORT_4_4_4_4;
        break;
    case FONT_SHEET_FORMAT_LA8:
        glFormat = FORMAT_LUMINANCE_ALPHA_NATIVE;
        break;
    case FONT_SHEET_FORMAT_LA4:
        glFormat = FORMAT_LUMINANCE_ALPHA_NATIVE;
        glType = TYPE_UNSIGNED_BYTE_4_4;
        break;
    case FONT_SHEET_FORMAT_A4:
        glType = TYPE_UNSIGNED_4BITS;
        break;
    case FONT_SHEET_FORMAT_HILO8:
    case FONT_SHEET_FORMAT_L8:
    case FONT_SHEET_FORMAT_A8:
    case FONT_SHEET_FORMAT_L4:
    default:
        break;
    }
    glTexImage2D(TEXTURE_2D_FCRAM_NO_COPY, 0, glFormat, width, height, 0, glFormat, glType, pImage);
    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, TEXTURE_MIN_FILTER, isSmallLinear ? LINEAR : NEAREST);
    glTexParameteri(TEXTURE_2D, TEXTURE_MAG_FILTER, isLargeLinear ? LINEAR : NEAREST);
    glTexParameterf(TEXTURE_2D, TEXTURE_LOD_BIAS, 0.0f);
    glTexParameteri(TEXTURE_2D, TEXTURE_MIN_LOD, MIN_LOD);
}

} // namespace internal
} // namespace font
} // namespace nw
