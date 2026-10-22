#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"

namespace nw {
namespace gfx {
namespace res {
// the shader of the name in the graphics file (a reference: the shader it refers to)
ReferenceTarget<ResShader> GetReferenceShaderTarget(ResReferenceShader shader, ResGraphicsFile graphicsFile); // 0x004A874C | nintendogs:bytes [tier A]
} // namespace res
} // namespace gfx
} // namespace nw
