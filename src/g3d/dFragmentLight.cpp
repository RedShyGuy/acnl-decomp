#include "g3d/dFragmentLight.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_FragmentLight_DynamicBuilder.h"
#include "nw/gfx/gfx_SceneBuilder.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/ma_Math.h"

namespace g3d {
namespace {
// the colors of a new light
const f32 DEFAULT_AMBIENT = 0.2f;
const f32 DEFAULT_DIFFUSE = 0.8f;

// (inline) a color with its PICA word (name is ours)
inline void SetColor(nw::ut::FloatColor* color, u32* colorU32, f32 r, f32 g, f32 b, f32 a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
    *colorU32 = color->ToPicaU32();
}
} // namespace

// 0x004ED620 slot 0x10 (name is ours)
bool g3d::FragmentLight::Load(const sead::SafeStringBase<char>& path, ssys::ma::Allocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    if (!m_ResourceLoader.Load(path, allocator->m_pHeap, ResourceLoader::CGFX_ALIGNMENT)) {
        return false;
    }
    m_ResourceLoader.InitializeCgfx(allocator, NULL, true);
    m_ResLight = nw::gfx::res::ResFragmentLight(nw::gfx::res::DynamicCastData<nw::gfx::res::ResFragmentLightData>(
        m_ResourceLoader.m_GraphicsFile.ptr()->lights.GetData(0), nw::gfx::res::ResFragmentLightData::TYPE));
    return m_ResLight.IsValid();
}

// 0x004ED6CC (name is ours)
bool g3d::FragmentLight::SetPosition(const nn::math::VEC3& position)
{
    SetTranslate(position);
    return true;
}

// 0x004ED6DC (name is ours)
bool g3d::FragmentLight::SetDirection(const nn::math::VEC3& direction)
{
    nn::math::VEC3 normalized = direction;
    ssys::ma::NormalizeIfPossible(&normalized);
    m_pLight->m_Direction = normalized;
    return true;
}

// 0x004ED720 (name is ours)
bool g3d::FragmentLight::SetAmbient(const nw::ut::FloatColor& color)
{
    m_ResLight.ptr()->ambient = color;
    m_ResLight.ptr()->ambientU8 = m_ResLight.ptr()->ambient.ToPicaU32();
    return true;
}

// 0x004ED754 (name is ours)
bool g3d::FragmentLight::SetDiffuse(const nw::ut::FloatColor& color)
{
    m_ResLight.ptr()->diffuse = color;
    m_ResLight.ptr()->diffuseU8 = m_ResLight.ptr()->diffuse.ToPicaU32();
    return true;
}

// 0x004ED788 (name is ours)
bool g3d::FragmentLight::SetSpecular0(const nw::ut::FloatColor& color)
{
    m_ResLight.ptr()->specular0 = color;
    m_ResLight.ptr()->specular0U8 = m_ResLight.ptr()->specular0.ToPicaU32();
    return true;
}

// 0x004ED7BC (name is ours)
bool g3d::FragmentLight::SetSpecular1(const nw::ut::FloatColor& color)
{
    m_ResLight.ptr()->specular1 = color;
    m_ResLight.ptr()->specular1U8 = m_ResLight.ptr()->specular1.ToPicaU32();
    return true;
}

// 0x004ED7F0 (name is ours)
bool g3d::FragmentLight::SetDistanceAttenuationEnd(f32 end)
{
    m_ResLight.ptr()->distanceAttenuationEnd = end;
    m_ResLight.SetDistanceAttenuation(m_ResLight.ptr()->distanceAttenuationStart, end);
    return true;
}

// 0x004ED814 (name is ours)
bool g3d::FragmentLight::SetDistanceAttenuationStart(f32 start)
{
    m_ResLight.ptr()->distanceAttenuationStart = start;
    m_ResLight.SetDistanceAttenuation(start, m_ResLight.ptr()->distanceAttenuationEnd);
    return true;
}

// 0x004ED834 slot 0x18 (name is ours)
bool g3d::FragmentLight::Create(nw::gfx::res::ResFragmentLight resource, nw::os::IAllocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::SceneBuilder builder;
    builder.m_Resource = nw::gfx::res::ResSceneObject(resource.ptr());
    builder.m_IsFixedSizeMemory = true;
    builder.m_MaxChildren = 0;
    builder.m_MaxCallbacks = 4;
    builder.m_BufferOption = 0;
    builder.m_pSharedMaterialModel = NULL;
    builder.m_pSharedMeshNodeVisibilityModel = NULL;
    builder.m_MaxAnimObjectsPerGroup = 1;
    builder.m_ParticleSetMargin = 0;
    builder.m_IsAnimationEnabled = true;
    nw::gfx::SceneObject* object = builder.CreateObject(allocator, allocator);
    if (object == NULL) {
        return false;
    }
    nw::gfx::FragmentLight* light = nw::gfx::DynamicCast<nw::gfx::FragmentLight>(object);
    m_pLight = light;
    m_pNode = light;
    m_ResLight = nw::gfx::res::ResFragmentLight(light->GetResSceneObject().ptr());
    m_ResLight.ptr()->isLightEnabled = true;
    return true;
}

// 0x004ED8F4 slot 0x14 (name is ours)
bool g3d::FragmentLight::Initialize(nw::os::IAllocator* allocator)
{
    if (m_ResLight.IsValid()) {
        return Create(m_ResLight, allocator);
    }
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::FragmentLight::DynamicBuilder builder;
    builder.m_Description.isFixedSizeMemory = true;
    builder.m_Description.isAnimationEnabled = true;
    builder.m_Description.maxCallbacks = 0;
    builder.m_Description.maxChildren = 0;
    builder.m_Description.maxAnimObjectsPerGroup = 1;
    nw::gfx::FragmentLight* light = builder.Create(allocator);
    m_pLight = light;
    m_pNode = light;
    m_ResLight = nw::gfx::res::ResFragmentLight(light->GetResSceneObject().ptr());
    m_ResLight.ptr()->isLightEnabled = true;
    SetColor(&m_ResLight.ptr()->ambient, &m_ResLight.ptr()->ambientU8, DEFAULT_AMBIENT, DEFAULT_AMBIENT, DEFAULT_AMBIENT, 1.0f);
    SetColor(&m_ResLight.ptr()->diffuse, &m_ResLight.ptr()->diffuseU8, DEFAULT_DIFFUSE, DEFAULT_DIFFUSE, DEFAULT_DIFFUSE, 1.0f);
    SetColor(&m_ResLight.ptr()->specular0, &m_ResLight.ptr()->specular0U8, 0.0f, 0.0f, 0.0f, 1.0f);
    SetColor(&m_ResLight.ptr()->specular1, &m_ResLight.ptr()->specular1U8, 0.0f, 0.0f, 0.0f, 1.0f);
    return true;
}

// 0x004EDA64 slot 0x0C (name after libgarden)
void g3d::FragmentLight::Finalize()
{
    m_ResourceLoader.Unload();
    TransformNode::Finalize();
}

// 0x004EDA80 slot 0x08 (name after libgarden)
void g3d::FragmentLight::Update()
{
    TransformNode::Update();
}

// 0x004EDA84 (name is ours)
g3d::FragmentLight::FragmentLight() : m_ResLight(NULL), m_pLight(NULL)
{
}

// 0x004EDADC slot 0x00
// 0x004EDAB8 (deleting dtor)
g3d::FragmentLight::~FragmentLight()
{
    // (only the members and the base are destroyed)
}

} // namespace g3d
