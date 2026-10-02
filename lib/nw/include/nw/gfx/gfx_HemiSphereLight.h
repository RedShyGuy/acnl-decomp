#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Light.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx15HemiSphereLightE @ 0x008D064C
// vtable 0x00902618 (vptr 0x00902620), offset_to_top 0, 13 entries
class HemiSphereLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    HemiSphereLight(); // ctor candidate(s) 0x0049A9E0, 0x0049AEB4 (unverified)
    virtual ~HemiSphereLight(); // 0x0049B0CC slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
    virtual void vf_0x04(); // 0x0049B0C0 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738BC0 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x0049AE60 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x0049A98C slot 0x24 | nintendogs:callseq
    virtual void vf_0x2C(); // 0x00738BB8 slot 0x2C | virtual slot, introduced by nw::gfx::Light
    virtual void vf_0x30(); // 0x00738BB0 slot 0x30 | virtual slot, introduced by nw::gfx::Light
    void CreateOriginalValue(nw::os::IAllocator*); // 0x0049AAB8 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResHemiSphereLight, nw::gfx::HemiSphereLight::Description); // 0x0049ABA8 | nintendogs:bytes [tier A]
    void CreateResHemiSphereLight(nw::os::IAllocator*, const char*); // 0x0049ACA0 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace gfx
} // namespace nw
