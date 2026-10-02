#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResTexture.h"

namespace nw {
namespace gfx {
namespace res {
class ResTextureMapper
{
public:
    void Cleanup(); // 0x0013F13C | nintendogs:callgraph [tier A]
    void SetTexture(nw::gfx::res::ResTexture); // 0x004A77D0 | nintendogs:bytes [tier A]
    void CloneDynamic(nw::os::IAllocator*); // 0x004A7894 | nintendogs:bytes [tier A]
    void DestroyDynamic(); // 0x004A78F4 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A7934 | nintendogs:bytes [tier A]
    void GetMemorySizeForCloneInternal(nw::os::MemorySizeCalculator*) const; // 0x0073C724 | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
