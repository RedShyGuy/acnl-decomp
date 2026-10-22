#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace lyt {
class Pane;

// the texture of a BCLIM image of size bytes: a GL texture (g_IsUseGlTexture) or the image copied
// into VRAM (texFormat: the target and memory area of the texture; 0: from the image flags); an
// empty info if it fails
TextureInfo LoadTexture(const void* pImgRes, u32 size, int texFormat); // 0x004B4B78 | nintendogs:bytes [tier B]
// the texture is released (name is ours)
void DeleteTexture(const TextureInfo& texInfo); // 0x004B5280 (name is ours)
// the point (in layout coordinates) is inside the pane
bool IsContain(Pane* pPane, const nn::math::VEC2& pos); // 0x004BF5C4 | nintendogs:bytes [tier A]

namespace internal {
// textures are GL textures; else the images go to VRAM directly (name is ours)
extern bool g_IsUseGlTexture;
} // namespace internal
} // namespace lyt
} // namespace nw
