#include <new>

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_AimTargetViewUpdater.h"
#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_FrustumProjectionUpdater.h"
#include "nw/gfx/gfx_LookAtTargetViewUpdater.h"
#include "nw/gfx/gfx_OrthoProjectionUpdater.h"
#include "nw/gfx/gfx_PerspectiveProjectionUpdater.h"
#include "nw/gfx/gfx_RotateViewUpdater.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the inverse of a 4x4 matrix; the identity if it has none (name is ours)
inline void MTX44InverseOrIdentity(nn::math::MTX44* pOut, const nn::math::MTX44& matrix)
{
    const f32 (*a)[4] = matrix.m;
    const f32 s0 = a[0][0] * a[1][1] - a[1][0] * a[0][1];
    const f32 s1 = a[0][0] * a[1][2] - a[1][0] * a[0][2];
    const f32 s2 = a[0][0] * a[1][3] - a[1][0] * a[0][3];
    const f32 s3 = a[0][1] * a[1][2] - a[1][1] * a[0][2];
    const f32 s4 = a[0][1] * a[1][3] - a[1][1] * a[0][3];
    const f32 s5 = a[0][2] * a[1][3] - a[1][2] * a[0][3];
    const f32 c5 = a[2][2] * a[3][3] - a[3][2] * a[2][3];
    const f32 c4 = a[2][1] * a[3][3] - a[3][1] * a[2][3];
    const f32 c3 = a[2][1] * a[3][2] - a[3][1] * a[2][2];
    const f32 c2 = a[2][0] * a[3][3] - a[3][0] * a[2][3];
    const f32 c1 = a[2][0] * a[3][2] - a[3][0] * a[2][2];
    const f32 c0 = a[2][0] * a[3][1] - a[3][0] * a[2][1];
    const f32 determinant = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (determinant == 0.0f) {
        *pOut = nn::math::MTX44::Identity();
        return;
    }
    const f32 inverse = 1.0f / determinant;
    f32 (*b)[4] = pOut->m;
    b[0][0] = (a[1][1] * c5 - a[1][2] * c4 + a[1][3] * c3) * inverse;
    b[0][1] = (-a[0][1] * c5 + a[0][2] * c4 - a[0][3] * c3) * inverse;
    b[0][2] = (a[3][1] * s5 - a[3][2] * s4 + a[3][3] * s3) * inverse;
    b[0][3] = (-a[2][1] * s5 + a[2][2] * s4 - a[2][3] * s3) * inverse;
    b[1][0] = (-a[1][0] * c5 + a[1][2] * c2 - a[1][3] * c1) * inverse;
    b[1][1] = (a[0][0] * c5 - a[0][2] * c2 + a[0][3] * c1) * inverse;
    b[1][2] = (-a[3][0] * s5 + a[3][2] * s2 - a[3][3] * s1) * inverse;
    b[1][3] = (a[2][0] * s5 - a[2][2] * s2 + a[2][3] * s1) * inverse;
    b[2][0] = (a[1][0] * c4 - a[1][1] * c2 + a[1][3] * c0) * inverse;
    b[2][1] = (-a[0][0] * c4 + a[0][1] * c2 - a[0][3] * c0) * inverse;
    b[2][2] = (a[3][0] * s4 - a[3][1] * s2 + a[3][3] * s0) * inverse;
    b[2][3] = (-a[2][0] * s4 + a[2][1] * s2 - a[2][3] * s0) * inverse;
    b[3][0] = (-a[1][0] * c3 + a[1][1] * c1 - a[1][2] * c0) * inverse;
    b[3][1] = (a[0][0] * c3 - a[0][1] * c1 + a[0][2] * c0) * inverse;
    b[3][2] = (-a[3][0] * s3 + a[3][1] * s1 - a[3][2] * s0) * inverse;
    b[3][3] = (a[2][0] * s3 - a[2][1] * s1 + a[2][2] * s0) * inverse;
}

// (inline) a copy of a camera resource (the matrices with MTX34CopyAsm, as the copy constructor of
// MTX34 does in the original; name is ours)
inline void CopyResCamera(res::ResCameraData* pOut, const res::ResCameraData& resource)
{
    res::CopyResTransformNodeData(pOut, resource);
    pOut->viewType = resource.viewType;
    pOut->projectionType = resource.projectionType;
    pOut->viewUpdater.m_Offset = resource.viewUpdater.m_Offset;
    pOut->projectionUpdater.m_Offset = resource.projectionUpdater.m_Offset;
    pOut->wScale = resource.wScale;
}

// (inline) a copy of an updater resource in memory of the allocator (NULL: no memory; name is ours)
template <typename T>
inline T* CloneUpdaterResource(os::IAllocator* pAllocator, const void* pResource)
{
    void* pMemory = pAllocator->Alloc(sizeof(T), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    T* pCopy = static_cast<T*>(pMemory);
    *pCopy = *static_cast<const T*>(pResource);
    return pCopy;
}
} // namespace

// 0x0097605C
const TypeInfo nw::gfx::Camera::s_TypeInfo(&TransformNode::s_TypeInfo);

// sets s_TypeInfo
// 0x00790B64 (static initializer)

// 0x004ACF98 slot 0x24 | nintendogs:bytes
u32 nw::gfx::Camera::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= TransformNode::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= StoreOriginal(pAllocator);
    result |= CreateAnimGroup(pAllocator);
    return result;
}

// 0x004ACFE0 | nintendogs:bytes [tier A]
u32 nw::gfx::Camera::StoreOriginal(os::IAllocator* pAllocator)
{
    const res::ResCameraData* pResource = static_cast<const res::ResCameraData*>(GetResSceneObject().ptr());

    void* pMemory = pAllocator->Alloc(sizeof(res::ResCameraData), 4);
    res::ResCameraData* pCopy = static_cast<res::ResCameraData*>(pMemory);
    if (pCopy != nullptr) {
        CopyResCamera(pCopy, *pResource);
    }
    pCopy->viewUpdater.m_Offset = 0;
    pCopy->projectionUpdater.m_Offset = 0;

    void* pViewUpdater = nullptr;
    switch (static_cast<u8>(pResource->viewType)) {
    case res::ResCameraData::VIEW_TYPE_AIM:
        pViewUpdater = CloneUpdaterResource<res::ResAimTargetViewUpdaterData>(pAllocator, pResource->viewUpdater.ToPtr());
        break;
    case res::ResCameraData::VIEW_TYPE_LOOK_AT:
        pViewUpdater = CloneUpdaterResource<res::ResLookAtTargetViewUpdaterData>(pAllocator, pResource->viewUpdater.ToPtr());
        break;
    case res::ResCameraData::VIEW_TYPE_ROTATE:
        pViewUpdater = CloneUpdaterResource<res::ResRotateViewUpdaterData>(pAllocator, pResource->viewUpdater.ToPtr());
        break;
    default:
        break;
    }
    pCopy->viewUpdater.Set(pViewUpdater);

    void* pProjectionUpdater = nullptr;
    switch (static_cast<u8>(pResource->projectionType)) {
    case res::ResCameraData::PROJECTION_TYPE_PERSPECTIVE:
        pProjectionUpdater = CloneUpdaterResource<res::ResPerspectiveProjectionUpdaterData>(pAllocator, pResource->projectionUpdater.ToPtr());
        break;
    case res::ResCameraData::PROJECTION_TYPE_FRUSTUM:
        pProjectionUpdater = CloneUpdaterResource<res::ResFrustumProjectionUpdaterData>(pAllocator, pResource->projectionUpdater.ToPtr());
        break;
    case res::ResCameraData::PROJECTION_TYPE_ORTHO:
        pProjectionUpdater = CloneUpdaterResource<res::ResOrthoProjectionUpdaterData>(pAllocator, pResource->projectionUpdater.ToPtr());
        break;
    default:
        break;
    }
    pCopy->projectionUpdater.Set(pProjectionUpdater);

    m_pOriginalResource = pCopy;
    m_OriginalTransform = static_cast<const res::ResCameraData*>(GetResSceneObject().ptr())->transform;
    return RESULT_SUCCESS;
}

// 0x004AD684 | nintendogs:bytes [tier A]
u32 nw::gfx::Camera::CreateAnimGroup(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    if (m_AnimBinding.Get() == nullptr) {
        return result;
    }

    anim::res::ResAnimGroup resAnimGroup = GetResSceneObject().GetAnimGroup(0);
    void* pMemory = pAllocator->Alloc(sizeof(AnimGroup), 4);
    AnimGroup* pAnimGroup = nullptr;
    if (pMemory != nullptr) {
        pAnimGroup = new (pMemory) AnimGroup(resAnimGroup, this, pAllocator);
        if (pAnimGroup->Initialize(true) & RESULT_FAILURE_MASK) {
            pAnimGroup->~AnimGroup();
            pAnimGroup->GetAllocator()->Free(pAnimGroup);
            pAnimGroup = nullptr;
        }
    }
    if (pAnimGroup == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // the camera's own members: its transform, and the resources of its updaters
    m_pAnimGroup = pAnimGroup;
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = m_pAnimGroup->GetResAnimGroupMember(i);
        void* pObject;
        switch (member.GetMemberType()) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE:
            pObject = this;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_VIEW_UPDATER:
            pObject = const_cast<res::ResCameraViewUpdaterData*>(m_ViewUpdater->GetResource().ptr());
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_PROJECTION_UPDATER:
            pObject = const_cast<res::ResCameraProjectionUpdaterData*>(m_ProjectionUpdater->GetResource().ptr());
            break;
        default:
            pObject = nullptr;
            break;
        }
        m_pAnimGroup->m_Targets[i] = pObject;
        if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE && member.GetField() == 0) {
            m_pAnimGroup->m_TargetValues[i] = &m_Transform;
        } else {
            m_pAnimGroup->m_TargetValues[i] = static_cast<u8*>(pObject) + member.ptr()->valueOffset;
        }
        m_pAnimGroup->m_TargetObjects[i] = nullptr;

        void* pOriginalValue = nullptr;
        switch (member.GetMemberType()) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE:
            if (member.GetField() == 0) {
                pOriginalValue = &m_OriginalTransform;
            }
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_VIEW_UPDATER:
            pOriginalValue = static_cast<u8*>(m_pOriginalResource->viewUpdater.ToPtr()) + member.ptr()->valueOffset;
            break;
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_PROJECTION_UPDATER:
            pOriginalValue = static_cast<u8*>(m_pOriginalResource->projectionUpdater.ToPtr()) + member.ptr()->valueOffset;
            break;
        default:
            break;
        }
        m_pAnimGroup->m_OriginalValues[i] = pOriginalValue;
    }
    m_AnimBinding->m_AnimGroups[0] = m_pAnimGroup;
    return result;
}

// 0x004AD908 (name is ours)
void nw::gfx::Camera::SetUpwardVector(const nn::math::VEC3& upwardVector)
{
    res::ResLookAtTargetViewUpdaterData* pResource = const_cast<res::ResLookAtTargetViewUpdaterData*>(
        res::DynamicCastData<res::ResLookAtTargetViewUpdaterData>(m_ViewUpdater->GetResource().ptr(), res::ResLookAtTargetViewUpdaterData::TYPE));
    pResource->upwardVector = upwardVector;
}

// 0x004AD944 | nintendogs:callgraph [tier A]
void nw::gfx::Camera::DestroyResCamera(os::IAllocator* pAllocator, res::ResCamera resCamera)
{
    res::ResCameraData* pResource = resCamera.ptr();
    if (pResource == nullptr) {
        return;
    }
    if (pResource->projectionUpdater.ToPtr() != nullptr) {
        pAllocator->Free(pResource->projectionUpdater.ToPtr());
        pResource->projectionUpdater.m_Offset = 0;
    }
    if (pResource->viewUpdater.ToPtr() != nullptr) {
        pAllocator->Free(pResource->viewUpdater.ToPtr());
        pResource->viewUpdater.m_Offset = 0;
    }
    pAllocator->Free(pResource);
}

// 0x004AD9F4 (name is ours)
void nw::gfx::Camera::SetTargetPosition(const nn::math::VEC3& targetPosition)
{
    res::ResAimTargetViewUpdaterData* pAim = const_cast<res::ResAimTargetViewUpdaterData*>(
        res::DynamicCastData<res::ResAimTargetViewUpdaterData>(m_ViewUpdater->GetResource().ptr(), res::ResAimTargetViewUpdaterData::TYPE));
    res::ResLookAtTargetViewUpdaterData* pLookAt = const_cast<res::ResLookAtTargetViewUpdaterData*>(
        res::DynamicCastData<res::ResLookAtTargetViewUpdaterData>(m_ViewUpdater->GetResource().ptr(), res::ResLookAtTargetViewUpdaterData::TYPE));
    if (pAim != nullptr) {
        pAim->targetPosition = targetPosition;
    } else if (pLookAt != nullptr) {
        pLookAt->targetPosition = targetPosition;
    }
}

// 0x004ADA80 | nintendogs:bytes [tier A]
void nw::gfx::Camera::UpdateCameraMatrix()
{
    const nn::math::VEC3 cameraPosition(m_WorldMatrix.m[0][3], m_WorldMatrix.m[1][3], m_WorldMatrix.m[2][3]);
    if (m_pParent != nullptr) {
        m_ViewUpdater->Update(&m_ViewMatrix, m_pParent->TrackbackWorldMatrix(), cameraPosition);
    } else {
        m_ViewUpdater->Update(&m_ViewMatrix, nn::math::MTX34::Identity(), cameraPosition);
    }
    if (!nn::math::ARMv6::MTX34InverseAsm(&m_InverseViewMatrix, &m_ViewMatrix)) {
        m_InverseViewMatrix = nn::math::MTX34::Identity();
    }
    m_ProjectionUpdater->Update(&m_ProjectionMatrix, &m_TextureProjectionMatrix);
    MTX44InverseOrIdentity(&m_InverseProjectionMatrix, m_ProjectionMatrix);
}

// 0x004ADEEC | nintendogs:bytes [tier A]
void nw::gfx::Camera::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResCamera resCamera, Description description)
{
    const res::ResCameraData* pResource = resCamera.ptr();
    pSize->Add(sizeof(Camera));
    const u8 viewType = static_cast<u8>(pResource->viewType);
    switch (viewType) {
    case res::ResCameraData::VIEW_TYPE_AIM:
    case res::ResCameraData::VIEW_TYPE_LOOK_AT:
    case res::ResCameraData::VIEW_TYPE_ROTATE:
        pSize->Add(sizeof(LookAtTargetViewUpdater));
        break;
    default:
        break;
    }
    const u8 projectionType = static_cast<u8>(pResource->projectionType);
    switch (projectionType) {
    case res::ResCameraData::PROJECTION_TYPE_PERSPECTIVE:
    case res::ResCameraData::PROJECTION_TYPE_FRUSTUM:
    case res::ResCameraData::PROJECTION_TYPE_ORTHO:
        pSize->Add(sizeof(PerspectiveProjectionUpdater));
        break;
    default:
        break;
    }
    TransformNode::GetMemorySizeForInitialize(pSize, res::ResTransformNode(resCamera.ptr()), description);

    // StoreOriginal
    pSize->Add(sizeof(res::ResCameraData));
    switch (viewType) {
    case res::ResCameraData::VIEW_TYPE_AIM:
        pSize->Add(sizeof(res::ResAimTargetViewUpdaterData));
        break;
    case res::ResCameraData::VIEW_TYPE_LOOK_AT:
        pSize->Add(sizeof(res::ResLookAtTargetViewUpdaterData));
        break;
    case res::ResCameraData::VIEW_TYPE_ROTATE:
        pSize->Add(sizeof(res::ResRotateViewUpdaterData));
        break;
    default:
        break;
    }
    switch (projectionType) {
    case res::ResCameraData::PROJECTION_TYPE_PERSPECTIVE:
        pSize->Add(sizeof(res::ResPerspectiveProjectionUpdaterData));
        break;
    case res::ResCameraData::PROJECTION_TYPE_FRUSTUM:
        pSize->Add(sizeof(res::ResFrustumProjectionUpdaterData));
        break;
    case res::ResCameraData::PROJECTION_TYPE_ORTHO:
        pSize->Add(sizeof(res::ResOrthoProjectionUpdaterData));
        break;
    default:
        break;
    }

    // CreateAnimGroup
    if (description.isAnimationEnabled && pResource->animGroupCount > 0) {
        pSize->Add(sizeof(AnimGroup));
        AnimGroup::GetMemorySizeForInitialize(pSize, res::ResSceneObject(pResource).GetAnimGroup(0), true);
    }
}

// 0x004AE144 slot 0x10 | nintendogs:bytes
void nw::gfx::Camera::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitCamera(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004AE198 | nintendogs:callseq [tier A]
nw::gfx::Camera* nw::gfx::Camera::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                         os::IAllocator* pAllocator)
{
    const res::ResCameraData* pResource = res::DynamicCastData<res::ResCameraData>(resObject.ptr(), res::ResCameraData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(Camera), 4);

    CameraViewUpdater* pViewUpdater = nullptr;
    switch (static_cast<u8>(pResource->viewType)) {
    case res::ResCameraData::VIEW_TYPE_AIM:
        pViewUpdater = AimTargetViewUpdater::Create(pAllocator, res::ResAimTargetViewUpdater(pResource->viewUpdater.ToPtr()));
        break;
    case res::ResCameraData::VIEW_TYPE_LOOK_AT:
        pViewUpdater = LookAtTargetViewUpdater::Create(pAllocator, res::ResLookAtTargetViewUpdater(pResource->viewUpdater.ToPtr()));
        break;
    case res::ResCameraData::VIEW_TYPE_ROTATE:
        pViewUpdater = RotateViewUpdater::Create(pAllocator, res::ResRotateViewUpdater(pResource->viewUpdater.ToPtr()));
        break;
    default:
        break;
    }
    GfxPtr<CameraViewUpdater> viewUpdater(pViewUpdater);

    CameraProjectionUpdater* pProjectionUpdater = nullptr;
    switch (static_cast<u8>(pResource->projectionType)) {
    case res::ResCameraData::PROJECTION_TYPE_PERSPECTIVE:
        pProjectionUpdater = PerspectiveProjectionUpdater::Create(
            pAllocator, res::ResPerspectiveProjectionUpdater(pResource->projectionUpdater.ToPtr()));
        break;
    case res::ResCameraData::PROJECTION_TYPE_FRUSTUM:
        pProjectionUpdater = FrustumProjectionUpdater::Create(pAllocator, res::ResFrustumProjectionUpdater(pResource->projectionUpdater.ToPtr()));
        break;
    case res::ResCameraData::PROJECTION_TYPE_ORTHO:
        pProjectionUpdater = OrthoProjectionUpdater::Create(pAllocator, res::ResOrthoProjectionUpdater(pResource->projectionUpdater.ToPtr()));
        break;
    default:
        break;
    }
    GfxPtr<CameraProjectionUpdater> projectionUpdater(pProjectionUpdater);

    Camera* pCamera = nullptr;
    if (pMemory != nullptr) {
        pCamera = new (pMemory)
            Camera(pAllocator, res::ResTransformNode(pResource), description, viewUpdater, projectionUpdater, pResource->wScale, false);
    }
    pCamera->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pCamera);
    }
    return pCamera;
}

// 0x004AE5B0 | nintendogs:bytes [tier A]
nw::gfx::Camera::Camera(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description,
                        GfxPtr<CameraViewUpdater> viewUpdater, GfxPtr<CameraProjectionUpdater> projectionUpdater, f32 wScale,
                        bool isDynamic)
    : TransformNode(pAllocator, resNode, description), m_ViewUpdater(viewUpdater), m_ProjectionUpdater(projectionUpdater),
      m_pAnimGroup(nullptr), m_pOriginalResource(nullptr), m_WScale(wScale), m_IsDynamic(isDynamic)
{
    // (the matrices are copy-constructed in the original: with MTX34CopyAsm / MTX44CopyAsm)
    nn::math::ARMv6::MTX34CopyAsm(&m_ViewMatrix, &nn::math::MTX34::Identity());
    nn::math::ARMv6::MTX34CopyAsm(&m_InverseViewMatrix, &nn::math::MTX34::Identity());
    nn::math::ARMv6::MTX44CopyAsm(&m_ProjectionMatrix, &nn::math::MTX44::Identity());
    nn::math::ARMv6::MTX44CopyAsm(&m_InverseProjectionMatrix, &nn::math::MTX44::Identity());
    nn::math::ARMv6::MTX34CopyAsm(&m_TextureProjectionMatrix, &nn::math::MTX34::Identity());
}

// 0x004AE688 slot 0x00 | nintendogs:bytes
// 0x004AE684 (deleting dtor)
nw::gfx::Camera::~Camera()
{
    const res::ResCameraData* pResource = res::DynamicCastData<res::ResCameraData>(GetResSceneObject().ptr(), res::ResCameraData::TYPE);
    if (m_IsDynamic && pResource != nullptr) {
        GetAllocator()->Free(const_cast<res::ResCameraData*>(pResource));
    }
    DestroyResCamera(GetAllocator(), res::ResCamera(m_pOriginalResource));
    m_pOriginalResource = nullptr;
    if (m_pAnimGroup != nullptr) {
        m_pAnimGroup->~AnimGroup();
        m_pAnimGroup->GetAllocator()->Free(m_pAnimGroup);
        m_pAnimGroup = nullptr;
    }
}

} // namespace gfx
} // namespace nw
