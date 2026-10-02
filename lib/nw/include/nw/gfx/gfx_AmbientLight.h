#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Light.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx12AmbientLightE @ 0x008D058C
// vtable 0x009023D0 (vptr 0x009023D8), offset_to_top 0, 13 entries
class AmbientLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    AmbientLight(); // ctor candidate(s) 0x0048DB5C, 0x0048E028 (unverified)
    virtual ~AmbientLight(); // 0x0048E240 slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
    virtual void vf_0x04(); // 0x0048E234 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00737890 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x0048DFD4 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x0048DB08 slot 0x24 | nintendogs:callseq
    virtual void vf_0x2C(); // 0x00737888 slot 0x2C | virtual slot, introduced by nw::gfx::Light
    virtual void vf_0x30(); // 0x00737880 slot 0x30 | virtual slot, introduced by nw::gfx::Light
    void CreateOriginalValue(nw::os::IAllocator*); // 0x0048DC34 | nintendogs:bytes [tier A]
    void CreateResAmbientLight(nw::os::IAllocator*, const char*); // 0x0048DD1C | nintendogs:bytes-fuzzy [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResAmbientLight, nw::gfx::AmbientLight::Description); // 0x0048DEDC | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
