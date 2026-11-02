#include "g3d/dHemiSphereLight.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_HemiSphereLight_DynamicBuilder.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the light of a new hemisphere light
const f32 DEFAULT_GROUND_COLOR = 0.2f;
const f32 DEFAULT_SKY_COLOR = 0.8f;
const f32 DEFAULT_LERP_FACTOR = 0.5f;

// (inline) a resource of the light to write to (name is ours)
inline nw::gfx::res::ResHemiSphereLightData* GetData(const nw::gfx::res::ResHemiSphereLight& light)
{
    return const_cast<nw::gfx::res::ResHemiSphereLightData*>(light.ptr());
}
} // namespace

// 0x004EF0A8 (name is ours)
bool g3d::HemiSphereLight::SetSkyColor(const nw::ut::FloatColor& color)
{
    GetData(m_ResLight)->skyColor = color;
    return true;
}

// 0x004EF0C0 (name is ours)
bool g3d::HemiSphereLight::SetDirection(const nn::math::VEC3& direction)
{
    GetData(m_ResLight)->direction = direction;
    return true;
}

// 0x004EF150 (name is ours)
bool g3d::HemiSphereLight::SetLerpFactor(f32 lerpFactor)
{
    GetData(m_ResLight)->lerpFactor = lerpFactor;
    return true;
}

// 0x004EF160 (name is ours)
bool g3d::HemiSphereLight::SetGroundColor(const nw::ut::FloatColor& color)
{
    GetData(m_ResLight)->groundColor = color;
    return true;
}

// 0x004EF178 slot 0x10 (name is ours)
bool g3d::HemiSphereLight::Initialize(nw::os::IAllocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::HemiSphereLight::DynamicBuilder builder;
    builder.m_Description.isFixedSizeMemory = true;
    builder.m_Description.isAnimationEnabled = true;
    builder.m_Description.maxCallbacks = 0;
    builder.m_Description.maxChildren = 0;
    builder.m_Description.maxAnimObjectsPerGroup = 1;
    nw::gfx::HemiSphereLight* light = builder.Create(allocator);
    m_pLight = light;
    m_pNode = light;
    m_ResLight = nw::gfx::res::ResHemiSphereLight(light->GetResSceneObject().ptr());
    GetData(m_ResLight)->isLightEnabled = true;
    nw::gfx::res::ResHemiSphereLightData* data = GetData(m_ResLight);
    data->direction.x = 0.0f;
    data->direction.y = -1.0f;
    data->direction.z = 0.0f;
    data = GetData(m_ResLight);
    data->groundColor.r = DEFAULT_GROUND_COLOR;
    data->groundColor.g = DEFAULT_GROUND_COLOR;
    data->groundColor.b = DEFAULT_GROUND_COLOR;
    data->groundColor.a = 1.0f;
    data = GetData(m_ResLight);
    data->skyColor.r = DEFAULT_SKY_COLOR;
    data->skyColor.g = DEFAULT_SKY_COLOR;
    data->skyColor.b = DEFAULT_SKY_COLOR;
    data->skyColor.a = 1.0f;
    GetData(m_ResLight)->lerpFactor = DEFAULT_LERP_FACTOR;
    return true;
}

// 0x004EF25C slot 0x08 (name after libgarden)
void g3d::HemiSphereLight::Update()
{
    TransformNode::Update();
}

// 0x004EF260 (name is ours)
g3d::HemiSphereLight::HemiSphereLight() : m_ResLight(NULL), m_pLight(NULL)
{
}

// 0x004EF2A4 slot 0x00
// 0x004EF294 (deleting dtor)
g3d::HemiSphereLight::~HemiSphereLight()
{
    // (only the base is destroyed)
}

} // namespace g3d
