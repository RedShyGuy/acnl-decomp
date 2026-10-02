#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResFog.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx3FogE @ 0x008D07CC
// vtable 0x009029AC (vptr 0x009029B4), offset_to_top 0, 11 entries
class Fog : public ::nw::gfx::TransformNode
{
public:
    class DynamicBuilder;
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Fog(); // ctor candidate(s) 0x004A4BE4, 0x004A52EC (unverified)
    virtual ~Fog(); // 0x004A5650 slot 0x00 | nintendogs:bytes-fuzzy
    virtual void vf_0x04(); // 0x004A5644 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x0073C6AC slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x004A5298 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x004A4788 slot 0x24 | nintendogs:bytes
    void CreateResFog(nw::os::IAllocator*, const char*); // 0x004A4880 | nintendogs:bytes-fuzzy [tier A]
    void DestroyResFog(nw::os::IAllocator*, nw::gfx::res::ResFogData*); // 0x004A4B1C | nintendogs:bytes [tier A]
    void CreateAnimGroup(nw::os::IAllocator*); // 0x004A4CB0 | nintendogs:bytes [tier A]
    void SetupFogSampler(nw::gfx::res::ResImageLookupTable, nw::gfx::res::ResFogUpdater, const nn::math::MTX44&); // 0x004A4E4C | nintendogs:callgraph [tier A]
    void CreateOriginalValue(nw::os::IAllocator*); // 0x004A50E4 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResFog, nw::gfx::Fog::Description); // 0x004A5188 | nintendogs:bytes [tier A]
    void Update(const nw::gfx::Camera*); // 0x004A550C | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
