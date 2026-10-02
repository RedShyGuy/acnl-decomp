#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"

namespace nw {
namespace gfx {
namespace res {
// 0x0013F170 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::res::ResFragmentShader::Cleanup()
{
}

// 0x004A7984 | nintendogs:bytes [tier B]
void nw::gfx::res::ResFragmentShader::CheckFragmentShader(nw::gfx::res::ResMaterialData*)
{
}

// 0x004A7BF4 | nintendogs:bytes [tier B]
void nw::gfx::res::ResFragmentShader::Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile, nw::gfx::res::ResMaterialData*)
{
}

} // namespace res
} // namespace gfx
} // namespace nw
