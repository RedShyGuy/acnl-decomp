#include "g3d/dFog.h"
#include "g3d/g3d_System.h"
#include "ssys/ma/dAllocator.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/gfx_Fog_DynamicBuilder.h"

namespace g3d {
namespace {
// the fog of a new Fog
const f32 DEFAULT_FOG_DENSITY = 10.0f;
const f32 DEFAULT_FOG_START = 100.0f;
const f32 DEFAULT_FOG_END = 1000.0f;
} // namespace

// 0x004F1218 (name is ours)
bool g3d::Fog::Initialize(nw::os::IAllocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::Fog::DynamicBuilder builder;
    builder.m_Description.isFixedSizeMemory = true;
    builder.m_Description.isAnimationEnabled = true;
    builder.m_Description.maxCallbacks = 0;
    builder.m_Description.maxChildren = 0;
    builder.m_Description.maxAnimObjectsPerGroup = 1;
    nw::gfx::Fog* fog = builder.Create(allocator);
    m_ResFog = nw::gfx::res::ResFog(fog->GetResSceneObject().ptr());
    nw::gfx::res::ResFogData* resFog = const_cast<nw::gfx::res::ResFogData*>(m_ResFog.ptr());
    resFog->color.r = 1.0f;
    resFog->color.g = 1.0f;
    resFog->color.b = 1.0f;
    resFog->color.a = 1.0f;
    nw::gfx::res::ResFogUpdaterData* updater = resFog->updater.To<nw::gfx::res::ResFogUpdaterData>();
    updater->fogType = nw::gfx::res::ResFogUpdaterData::FOG_TYPE_LINEAR;
    updater->density = DEFAULT_FOG_DENSITY;
    updater->start = DEFAULT_FOG_START;
    updater->end = DEFAULT_FOG_END;
    m_pNode = fog;
    return true;
}

// 0x004F12D4 slot 0x08 (name after libgarden)
void g3d::Fog::Update()
{
    TransformNode::Update();
}

// 0x004F12D8 (name is ours)
g3d::Fog::Fog() : m_ResFog(NULL)
{
}

// 0x004F1310 slot 0x00
// 0x004F1300 (deleting dtor)
g3d::Fog::~Fog()
{
    // (only the base is destroyed)
}

} // namespace g3d
