#pragma once

#include "nn/gx/gx_Types.h"

namespace nw {
namespace lyt {
namespace internal {
// The GL enums lyt uses (names after the OpenGL ES API; the names of the values of the gx extensions
// are ours).
namespace gl {
const GLenum TRIANGLES = 0x0004;
const GLenum BLEND = 0x0BE2;
const GLenum COLOR_LOGIC_OP = 0x0BF2;
const GLenum TEXTURE_2D = 0x0DE1;
const GLenum BYTE = 0x1400;
const GLenum UNSIGNED_BYTE = 0x1401;
const GLenum SHORT = 0x1402;
const GLenum UNSIGNED_SHORT = 0x1403;
const GLenum SRC_ALPHA = 0x0302;
const GLenum ONE_MINUS_SRC_ALPHA = 0x0303;
const GLenum TEXTURE_MAG_FILTER = 0x2800;
const GLenum TEXTURE_MIN_FILTER = 0x2801;
const GLenum TEXTURE_WRAP_S = 0x2802;
const GLenum TEXTURE_WRAP_T = 0x2803;
const GLenum TEXTURE_MIN_LOD = 0x813A;
const GLenum TEXTURE0 = 0x84C0;
const GLenum TEXTURE_LOD_BIAS = 0x8501;
const GLenum ARRAY_BUFFER = 0x8892;
const GLenum ELEMENT_ARRAY_BUFFER = 0x8893;
const GLenum STATIC_DRAW = 0x88E4;
const GLenum VERTEX_SHADER = 0x8B31;
const GLenum SHADER_BINARY_FORMAT = 0x6000;
const GLenum FRAGMENT_MODE_DEFAULT = 0x6030;
const GLenum PREVIOUS = 0x8578;
const GLenum PREVIOUS_BUFFER = 0x8579;
// the texture parameter of the physical address of the image (glGetTexParameteriv; name is ours)
const GLenum TEXTURE_DATA_ADDRESS = 0x6790;
} // namespace gl
} // namespace internal
} // namespace lyt
} // namespace nw
