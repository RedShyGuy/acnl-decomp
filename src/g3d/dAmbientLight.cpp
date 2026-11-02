#include "g3d/dAmbientLight.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_AmbientLight_DynamicBuilder.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the color of a new light
const f32 DEFAULT_COLOR = 0.2f;
} // namespace

// 0x004ECE74 (name is ours)
bool g3d::AmbientLight::SetColor(const nw::ut::FloatColor& color)
{
    nw::gfx::res::ResAmbientLightData* data = const_cast<nw::gfx::res::ResAmbientLightData*>(m_ResLight.ptr());
    data->color = color;
    data->colorU32 = m_ResLight.ptr()->color.ToPicaU32();
    return true;
}

// 0x004ECEA8 slot 0x10 (name is ours)
bool g3d::AmbientLight::Initialize(nw::os::IAllocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::AmbientLight::DynamicBuilder builder;
    builder.m_Description.isFixedSizeMemory = true;
    builder.m_Description.isAnimationEnabled = true;
    builder.m_Description.maxCallbacks = 0;
    builder.m_Description.maxChildren = 0;
    builder.m_Description.maxAnimObjectsPerGroup = 1;
    nw::gfx::AmbientLight* light = builder.Create(allocator);
    m_pLight = light;
    m_pNode = light;
    m_ResLight = nw::gfx::res::ResAmbientLight(light->GetResSceneObject().ptr());
    nw::gfx::res::ResAmbientLightData* data = const_cast<nw::gfx::res::ResAmbientLightData*>(m_ResLight.ptr());
    data->isLightEnabled = true;
    data = const_cast<nw::gfx::res::ResAmbientLightData*>(m_ResLight.ptr());
    data->color.r = DEFAULT_COLOR;
    data->color.g = DEFAULT_COLOR;
    data->color.b = DEFAULT_COLOR;
    data->color.a = 1.0f;
    const_cast<nw::gfx::res::ResAmbientLightData*>(m_ResLight.ptr())->colorU32 = m_ResLight.ptr()->color.ToPicaU32();
    return true;
}

// 0x004ECF44 (name is ours)
g3d::AmbientLight::AmbientLight() : m_ResLight(NULL), m_pLight(NULL)
{
}

// 0x004EEB3C slot 0x08 (name after libgarden)
void g3d::AmbientLight::Update()
{
    TransformNode::Update();
}

// 0x004EEB98 slot 0x00
// 0x004ECF78 (deleting dtor)
g3d::AmbientLight::~AmbientLight()
{
    // (only the base is destroyed)
}

} // namespace g3d
