#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_Light.h"

namespace nw {
namespace gfx {
// ctor address unknown
nw::gfx::Light::Light()
{
}

// 0x004AAA38 slot 0x00 | nintendogs:bytes
nw::gfx::Light::~Light()
{
}

// 0x004AAA2C slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::Light::vf_0x04()
{
}

// 0x0073C820 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::Light::GetRuntimeTypeInfo() const
{
}

// 0x004AA9D8 slot 0x10 | nintendogs:bytes
void nw::gfx::Light::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
void nw::gfx::Light::vf_0x2C()
{
}

// 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
void nw::gfx::Light::vf_0x30()
{
}

// 0x004AA760 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Light::CreateAnimGroup(nw::os::IAllocator*)
{
}

// 0x004AA9B0 | nintendogs:bytes [tier A]
void nw::gfx::Light::DestroyOriginalValue()
{
}

} // namespace gfx
} // namespace nw
