#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
void SetupReferenceLut(nw::gfx::res::ResReferenceLookupTable, nw::gfx::res::ResGraphicsFile); // 0x004A7D9C | nintendogs:bytes [tier A]
void GetReferenceLutTarget(nw::gfx::res::ResReferenceLookupTable, nw::gfx::res::ResGraphicsFile); // 0x004A811C | nintendogs:bytes-fuzzy [tier A]
void GetReferenceShaderTarget(nw::gfx::res::ResReferenceShader, nw::gfx::res::ResGraphicsFile); // 0x004A874C | nintendogs:bytes [tier A]
void GetReferenceTextureTarget(nw::gfx::res::ResReferenceTexture, nw::gfx::res::ResGraphicsFile); // 0x004A8D28 | nintendogs:bytes [tier A]
} // namespace res
} // namespace gfx
} // namespace nw
