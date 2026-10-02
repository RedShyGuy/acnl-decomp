#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_Camera.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004AE5B0 (unverified)
nw::gfx::Camera::Camera()
{
}

// 0x004AE688 slot 0x00 | nintendogs:bytes
nw::gfx::Camera::~Camera()
{
}

// 0x004AE684 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::Camera::vf_0x04()
{
}

// 0x0073C838 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::Camera::GetRuntimeTypeInfo() const
{
}

// 0x004AE144 slot 0x10 | nintendogs:bytes
void nw::gfx::Camera::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x004ACF98 slot 0x24 | nintendogs:bytes
void nw::gfx::Camera::Initialize(nw::os::IAllocator*)
{
}

// 0x004ACFE0 | nintendogs:bytes [tier A]
void nw::gfx::Camera::StoreOriginal(nw::os::IAllocator*)
{
}

// 0x004AD684 | nintendogs:bytes [tier A]
void nw::gfx::Camera::CreateAnimGroup(nw::os::IAllocator*)
{
}

// 0x004AD944 | nintendogs:callgraph [tier A]
void nw::gfx::Camera::DestroyResCamera(nw::os::IAllocator*, nw::gfx::res::ResCamera)
{
}

// 0x004ADA80 | nintendogs:bytes [tier A]
void nw::gfx::Camera::UpdateCameraMatrix()
{
}

// 0x004ADEEC | nintendogs:bytes [tier A]
void nw::gfx::Camera::GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResCamera, nw::gfx::Camera::Description)
{
}

// 0x004AE198 | nintendogs:callseq [tier A]
void nw::gfx::Camera::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::Camera::Description&, nw::os::IAllocator*)
{
}

// 0x004AE5B0 | nintendogs:bytes [tier A]
nw::gfx::Camera::Camera(nw::os::IAllocator*, nw::gfx::res::ResTransformNode, const nw::gfx::Camera::Description&, nw::gfx::GfxPtr<nw::gfx::CameraViewUpdater>, nw::gfx::GfxPtr<nw::gfx::CameraProjectionUpdater>, float, bool)
{
}

// 0x0073C844 | nintendogs:bytes [tier A]
void nw::gfx::Camera::GetFar() const
{
}

// 0x0073C860 | nintendogs:bytes [tier A]
void nw::gfx::Camera::GetNear() const
{
}

} // namespace gfx
} // namespace nw
