#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Light.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13FragmentLightE @ 0x008D05D4
// vtable 0x009024C0 (vptr 0x009024C8), offset_to_top 0, 13 entries
class FragmentLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    FragmentLight(); // ctor candidate(s) 0x00492AD8, 0x004932A0 (unverified)
    virtual ~FragmentLight(); // 0x00493374 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00493368 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738A18 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x0049324C slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x00492A84 slot 0x24 | nintendogs:callseq
    virtual void UpdateDirection(); // 0x00492BA0 slot 0x28 | nintendogs:bytes
    virtual void vf_0x2C(); // 0x00738A10 slot 0x2C | virtual slot, introduced by nw::gfx::Light
    virtual void vf_0x30(); // 0x00738A04 slot 0x30 | virtual slot, introduced by nw::gfx::Light
    void CreateOriginalValue(nw::os::IAllocator*); // 0x00492C78 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResFragmentLight, nw::gfx::FragmentLight::Description); // 0x00492E04 | nintendogs:bytes [tier A]
    void CreateResFragmentLight(nw::os::IAllocator*, const char*); // 0x00492EFC | nintendogs:bytes-fuzzy [tier A]
    void DestroyResFragmentLight(nw::os::IAllocator*, nw::gfx::res::ResFragmentLightData*); // 0x0049319C | nintendogs:bytes [tier A]
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::FragmentLight::Description&, nw::os::IAllocator*); // 0x004932A0 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
