#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResMaterial.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx8MaterialE @ 0x008D0808
// vtable 0x00902A98 (vptr 0x00902AA0), offset_to_top 0, 3 entries
class Material : public ::nw::gfx::SceneObject
{
public:
    Material(); // ctor candidate(s) 0x004B04EC (unverified)
    virtual ~Material(); // 0x004B0658 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004B05A8 slot 0x04 | virtual slot, introduced by nw::gfx::Material
    virtual void vf_0x08(); // 0x0073C918 slot 0x08 | virtual slot, introduced by nw::gfx::Material
    void CreateBuffers(nw::os::IAllocator*); // 0x004AEECC | nintendogs:callgraph [tier A]
    void CopyResMaterial(nw::os::IAllocator*, unsigned); // 0x004AF064 | nintendogs:callseq [tier A]
    void DestroyResMaterial(nw::os::IAllocator*, nw::gfx::res::ResMaterial); // 0x004AF5C4 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResMaterial, int, unsigned); // 0x004AF818 | nintendogs:bytes-fuzzy [tier A]
    void DestroyResFragmentShader(nw::os::IAllocator*, nw::gfx::res::ResFragmentShader); // 0x004AFC78 | nintendogs:callgraph [tier A]
    void CopyResLightingLookupTable(nw::os::IAllocator*, nw::gfx::res::ResLightingLookupTable); // 0x004AFF70 | nintendogs:bytes [tier A]
    void CopyResFragmentLightingTable(nw::os::IAllocator*, nw::gfx::res::ResFragmentLightingTable); // 0x004B00B4 | nintendogs:bytes [tier A]
    void Create(nw::gfx::res::ResMaterial, int, nw::gfx::Model*, nw::os::IAllocator*); // 0x004B0388 | nintendogs:bytes [tier A]
    void CanUseBuffer(unsigned) const; // 0x0073C888 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace gfx
} // namespace nw
