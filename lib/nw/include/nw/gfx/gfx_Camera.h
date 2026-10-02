#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx6CameraE @ 0x008D07F0
// vtable 0x00902A50 (vptr 0x00902A58), offset_to_top 0, 11 entries
class Camera : public ::nw::gfx::TransformNode
{
public:
    class DynamicBuilder;
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Camera(); // ctor candidate(s) 0x004AE5B0 (unverified)
    virtual ~Camera(); // 0x004AE688 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004AE684 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x0073C838 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x004AE144 slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x004ACF98 slot 0x24 | nintendogs:bytes
    void StoreOriginal(nw::os::IAllocator*); // 0x004ACFE0 | nintendogs:bytes [tier A]
    void CreateAnimGroup(nw::os::IAllocator*); // 0x004AD684 | nintendogs:bytes [tier A]
    void DestroyResCamera(nw::os::IAllocator*, nw::gfx::res::ResCamera); // 0x004AD944 | nintendogs:callgraph [tier A]
    void UpdateCameraMatrix(); // 0x004ADA80 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResCamera, nw::gfx::Camera::Description); // 0x004ADEEC | nintendogs:bytes [tier A]
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::Camera::Description&, nw::os::IAllocator*); // 0x004AE198 | nintendogs:callseq [tier A]
    Camera(nw::os::IAllocator*, nw::gfx::res::ResTransformNode, const nw::gfx::Camera::Description&, nw::gfx::GfxPtr<nw::gfx::CameraViewUpdater>, nw::gfx::GfxPtr<nw::gfx::CameraProjectionUpdater>, float, bool); // 0x004AE5B0 | nintendogs:bytes [tier A]
    void GetFar() const; // 0x0073C844 | nintendogs:bytes [tier A]
    void GetNear() const; // 0x0073C860 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
