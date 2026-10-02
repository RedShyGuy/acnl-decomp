#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"

namespace nw {
namespace gfx {
namespace res {
// 0x0013F13C | nintendogs:callgraph [tier A]
void nw::gfx::res::ResTextureMapper::Cleanup()
{
}

// 0x004A77D0 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::SetTexture(nw::gfx::res::ResTexture)
{
}

// 0x004A7894 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::CloneDynamic(nw::os::IAllocator*)
{
}

// 0x004A78F4 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::DestroyDynamic()
{
}

// 0x004A7934 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile)
{
}

// 0x0073C724 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::GetMemorySizeForCloneInternal(nw::os::MemorySizeCalculator*) const
{
}

} // namespace res
} // namespace gfx
} // namespace nw
