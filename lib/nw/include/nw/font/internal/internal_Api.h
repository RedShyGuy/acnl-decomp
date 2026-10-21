#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
class Color8;
} // namespace ut
} // namespace nw

namespace nw {
namespace font {
namespace internal {
// the colors of the vertices of a character (CharWriter::Vertex order) as float vectors from the two
// text colors and the gradation mode (CharWriter::GradationMode); name is ours
void GetVertexColors(f32 (*pVertexColors)[4], const nw::ut::Color8* pTextColors, u8 gradationMode); // 0x004D9210 (name is ours)
// the image of a sheet as the bound 2D texture (FontSheetFormat), clamped, with the filters
void LoadTexture(u16 width, u16 height, u16 format, const void* pImage, bool isSmallLinear, bool isLargeLinear); // 0x004D97A4 | nintendogs:bytes-fuzzy [tier A]
} // namespace internal
} // namespace font
} // namespace nw
