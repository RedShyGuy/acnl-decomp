#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
class MeshRenderer;
} // namespace gfx
} // namespace nw

namespace g3d {
// a node that the renderer of the engine calls instead of drawing meshes: a function for the opaque and one for the
// translucent pass (member names are ours)
// RTTI N3g3d8FuncNodeE @ 0x008D0D98
// vtable 0x00903C2C (vptr 0x00903C34), offset_to_top 0, 11 entries
class FuncNode : public ::nw::gfx::TransformNode
{
public:
    // a drawing function of the node
    typedef void (*DrawFunc)(nw::gfx::MeshRenderer* renderer, void* userData);

    // (inline in Func::Initialize, which passes no resource)
    FuncNode(nw::os::IAllocator* pAllocator, const Description& description)
        : nw::gfx::TransformNode(pAllocator, nw::gfx::res::ResTransformNode(), description), m_pOpaqueFunc(NULL),
          m_pTranslucentFunc(NULL), m_pUserData(NULL)
    {
    }
    virtual ~FuncNode(); // 0x004F2DF4 slot 0x00
    // 0x004F2DE8 (deleting dtor)
    DECOMP_EMIT_COPY virtual const nw::gfx::TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00747500 slot 0x08

    static const nw::gfx::TypeInfo s_TypeInfo;

    DrawFunc m_pOpaqueFunc;        // 0x148
    DrawFunc m_pTranslucentFunc;   // 0x14C
    void* m_pUserData;             // 0x150
};
ASSERT_SIZE(FuncNode, 0x154);
} // namespace g3d
