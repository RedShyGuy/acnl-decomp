#include "g3d/dCamera.h"
#include "g3d/g3d_System.h"
#include "ssys/ma/dAllocator.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Camera_DynamicBuilder.h"
#include "nw/gfx/gfx_LookAtTargetViewUpdater.h"
#include "sead/seadCameraProjectionUpdaterNw4c.h"
#include "ssys/ma/ma_Math.h"

namespace g3d {
namespace {
const f32 DEG_TO_RAD = 0.0174532924f;
} // namespace

// 0x004F24CC (name is ours)
void g3d::Camera::SetTarget(const nn::math::VEC3& target, s16 upRotateX, s16 upRotateY, s16 upRotateX2)
{
    m_pViewData->targetPosition.x = target.x;
    m_pViewData->targetPosition.y = target.y;
    m_pViewData->targetPosition.z = target.z;
    nn::math::VEC3 up(0.0f, 1.0f, 0.0f);
    ssys::ma::RotateX(&up, upRotateX);
    ssys::ma::RotateY(&up, upRotateY);
    ssys::ma::RotateX(&up, upRotateX2);
    m_pViewData->upwardVector = up;
}

// 0x004F2558 (name is ours)
bool g3d::Camera::Initialize(const nn::math::VEC3& target, const PerspectiveParam& param, nw::os::IAllocator* allocator)
{
    if (allocator == NULL) {
        allocator = GetAllocator();
    }
    nw::gfx::LookAtTargetViewUpdater* viewUpdater = nw::gfx::LookAtTargetViewUpdater::Create(allocator);
    m_pViewData = const_cast<nw::gfx::res::ResLookAtTargetViewUpdaterData*>(
        static_cast<const nw::gfx::res::ResLookAtTargetViewUpdaterData*>(viewUpdater->GetResource().ptr()));
    m_pViewData->targetPosition = target;
    m_pViewData->upwardVector = nn::math::VEC3(0.0f, 1.0f, 0.0f);
    m_pProjectionUpdater = sead::CameraProjectionUpdaterNw4c::create(allocator, NULL);
    m_pProjectionUpdater->m_Pivot = nn::math::PIVOT_DIRECTION_1;
    m_Projection.set(param.near, param.far, param.fovyDegree * DEG_TO_RAD, param.aspect);
    m_pProjectionUpdater->mProjection = &m_Projection;
    nw::gfx::Camera::DynamicBuilder builder;
    builder.m_Description.isFixedSizeMemory = true;
    builder.m_Description.isAnimationEnabled = true;
    builder.m_Description.maxCallbacks = 0;
    builder.m_Description.maxChildren = 0;
    builder.m_Description.maxAnimObjectsPerGroup = 1;
    nw::gfx::GfxPtr<nw::gfx::CameraViewUpdater> viewUpdaterPtr(viewUpdater);
    builder.m_ViewUpdater = viewUpdaterPtr;
    nw::gfx::GfxPtr<nw::gfx::CameraProjectionUpdater> projectionUpdaterPtr(m_pProjectionUpdater);
    builder.m_ProjectionUpdater = projectionUpdaterPtr;
    nw::gfx::Camera* camera = builder.Create(allocator);
    if (camera == NULL) {
        return false;
    }
    m_pNode = camera;
    return true;
}

// 0x004F27C4 slot 0x08 (name after libgarden)
void g3d::Camera::Update()
{
    UpdateWorldMatrix(m_pNode);
    nw::gfx::DynamicCast<nw::gfx::Camera>(m_pNode)->UpdateCameraMatrix();
}

// 0x004F2820 (name is ours)
g3d::Camera::Camera() : m_pViewData(NULL)
{
}

// 0x004F2874 slot 0x00
// 0x004F2850 (deleting dtor)
g3d::Camera::~Camera()
{
    // (only the members and the base are destroyed)
}

} // namespace g3d
