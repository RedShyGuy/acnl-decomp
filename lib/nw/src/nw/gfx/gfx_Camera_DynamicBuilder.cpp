#include <cstring>
#include <new>

#include "nn/math/math_MTX34.h"
#include "nw/gfx/gfx_Camera_DynamicBuilder.h"
#include "nw/gfx/gfx_LookAtTargetViewUpdater.h"
#include "nw/gfx/gfx_PerspectiveProjectionUpdater.h"

namespace nw {
namespace gfx {
namespace {
// the header of the resource of a dynamic camera (the signature reads "CTFN"; names are ours)
const u32 DYNAMIC_CAMERA_SIGNATURE = 0x4E465443;
const u32 DYNAMIC_CAMERA_REVISION = 0x07010000;
} // namespace

// 0x004AD2A4 | nintendogs:bytes [tier A]
nw::gfx::Camera* nw::gfx::Camera::DynamicBuilder::Create(os::IAllocator* pAllocator)
{
    if (m_ViewUpdater.Get() == nullptr) {
        m_ViewUpdater = GfxPtr<CameraViewUpdater>(LookAtTargetViewUpdater::Create(pAllocator));
    }
    if (m_ProjectionUpdater.Get() == nullptr) {
        m_ProjectionUpdater = GfxPtr<CameraProjectionUpdater>(PerspectiveProjectionUpdater::Create(pAllocator));
    }

    // the resource: a camera at the origin with the resources of the updaters
    res::ResCameraData* pResource = static_cast<res::ResCameraData*>(pAllocator->Alloc(sizeof(res::ResCameraData), 4));
    std::memset(pResource, 0, sizeof(res::ResCameraData));
    pResource->viewUpdater.Set(m_ViewUpdater->GetResource().ptr());
    pResource->projectionUpdater.Set(m_ProjectionUpdater->GetResource().ptr());
    switch (m_ViewUpdater->GetResource().ptr()->typeInfo) {
    case res::ResAimTargetViewUpdaterData::TYPE:
        pResource->viewType = res::ResCameraData::VIEW_TYPE_AIM;
        break;
    case res::ResRotateViewUpdaterData::TYPE:
        pResource->viewType = res::ResCameraData::VIEW_TYPE_ROTATE;
        break;
    case res::ResLookAtTargetViewUpdaterData::TYPE:
        pResource->viewType = res::ResCameraData::VIEW_TYPE_LOOK_AT;
        break;
    default:
        break;
    }
    switch (m_ProjectionUpdater->GetResource().ptr()->typeInfo) {
    case res::ResFrustumProjectionUpdaterData::TYPE:
        pResource->projectionType = res::ResCameraData::PROJECTION_TYPE_FRUSTUM;
        break;
    case res::ResPerspectiveProjectionUpdaterData::TYPE:
        pResource->projectionType = res::ResCameraData::PROJECTION_TYPE_PERSPECTIVE;
        break;
    case res::ResOrthoProjectionUpdaterData::TYPE:
        pResource->projectionType = res::ResCameraData::PROJECTION_TYPE_ORTHO;
        break;
    default:
        break;
    }
    pResource->m_Unknown0x8 = DYNAMIC_CAMERA_REVISION;
    pResource->signature = DYNAMIC_CAMERA_SIGNATURE;
    pResource->typeInfo = res::ResCameraData::TYPE;
    pResource->userDataCount = 0;
    pResource->userDataDic.m_Offset = 0;
    pResource->name.m_Offset = 0;
    pResource->childCount = 0;
    pResource->children.m_Offset = 0;
    pResource->animGroupCount = 0;
    pResource->animGroupDic.m_Offset = 0;
    pResource->transform.scale = nn::math::VEC3(1.0f, 1.0f, 1.0f);
    pResource->transform.rotate = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    pResource->transform.translate = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    pResource->worldMatrix = nn::math::MTX34::Identity();
    pResource->flags |= res::ResSceneObjectData::FLAG_IS_BRANCH_VISIBLE;

    void* pMemory = pAllocator->Alloc(sizeof(Camera), 4);
    Camera* pCamera = nullptr;
    if (pMemory != nullptr) {
        pCamera = new (pMemory) Camera(pAllocator, res::ResTransformNode(pResource), m_Description, m_ViewUpdater, m_ProjectionUpdater, 0.0f, true);
    }
    pCamera->Initialize(pAllocator);
    return pCamera;
}

} // namespace gfx
} // namespace nw
