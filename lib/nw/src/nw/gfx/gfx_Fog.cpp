#include <cmath>
#include <cstring>
#include <new>

#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/res/gfx_ResDynamic.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace gfx {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters
// (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0)
{
    return reg | (mask << 16) | (extraCount << 20);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_FOG_LUT_INDEX = 0x0E6;
const u32 REG_FOG_LUT_DATA0 = 0x0E8;

// the entries of the fog table, the commands that load it
const int FOG_TABLE_SIZE = 128;
const u32 FOG_TABLE_COMMAND_SIZE = 528;

// (inline) the fog at a depth as an unsigned 0.11 fixed point number (name is ours)
inline u32 FogValueToFix11(f32 value)
{
    u32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    if (value <= 0.0f || ((bits << 1) >> 24) == 0xFF) {
        return 0;
    }
    const f32 scaled = value * 2048.0f;
    if (scaled >= 2048.0f) {
        return 0x7FF;
    }
    return static_cast<u32>(scaled);
}

// (inline) the change of the fog to the next depth as a signed 1.11 fixed point number in 13 bits
// (name is ours)
inline u32 FogDeltaToFix13(f32 delta)
{
    f32 scaled = (delta + 2.0f) * 2048.0f;
    if (scaled < 0.0f) {
        scaled = 0.0f;
    } else if (scaled >= 8192.0f) {
        scaled = 8191.0f;
    }
    return static_cast<u32>(static_cast<s32>(scaled - 4096.0f)) & 0x1FFF;
}
} // namespace

// 0x00976060
const TypeInfo nw::gfx::Fog::s_TypeInfo(&TransformNode::s_TypeInfo);

// sets s_TypeInfo
// 0x00785758 (static initializer)

// 0x004A4788 slot 0x24 | nintendogs:bytes
u32 nw::gfx::Fog::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= TransformNode::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // the updater the table is made with (the old one is freed)
    void* pBuffer = pAllocator->Alloc(sizeof(res::ResFogUpdaterData), 4);
    std::memset(pBuffer, 0, sizeof(res::ResFogUpdaterData));
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    } else {
        res::ResFogUpdaterData* pOldUpdater = m_pLastUpdater;
        os::IAllocator* pOldAllocator = m_pLastUpdaterAllocator;
        m_pLastUpdater = static_cast<res::ResFogUpdaterData*>(pBuffer);
        m_pLastUpdaterAllocator = pAllocator;
        if (pOldUpdater != nullptr) {
            pOldAllocator->Free(pOldUpdater);
        }
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    result |= CreateOriginalValue(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateAnimGroup(pAllocator);
    return result;
}

// 0x004A4880 | nintendogs:bytes-fuzzy [tier A]
nw::gfx::res::ResFogData* nw::gfx::Fog::CreateResFog(os::IAllocator* pAllocator, const char* name)
{
    res::ResFogData* pResource = static_cast<res::ResFogData*>(pAllocator->Alloc(sizeof(res::ResFogData), 4));
    std::memset(pResource, 0, sizeof(res::ResFogData));
    res::SetupDynamicResSceneObject(pResource, res::ResFogData::TYPE, pAllocator, name, res::ResFogData::SIGNATURE, res::ResFogData::DYNAMIC_REVISION);
    res::SetupDynamicResTransformNode(pResource);

    res::ResImageLookupTableData* pSampler = static_cast<res::ResImageLookupTableData*>(pAllocator->Alloc(sizeof(res::ResImageLookupTableData), 4));
    std::memset(pSampler, 0, sizeof(res::ResImageLookupTableData));
    res::ResFogUpdaterData* pUpdater = static_cast<res::ResFogUpdaterData*>(pAllocator->Alloc(sizeof(res::ResFogUpdaterData), 4));
    std::memset(pUpdater, 0, sizeof(res::ResFogUpdaterData));
    pResource->updater.Set(pUpdater);
    pResource->sampler.Set(pSampler);

    // the table: an image of 128 entries, filled by Update
    pSampler->typeInfo = res::ResLookupTable::TYPE_IMAGE;
    u32* pCommands = static_cast<u32*>(pAllocator->Alloc(FOG_TABLE_COMMAND_SIZE, 4));
    if (pCommands != nullptr) {
        std::memset(pCommands, 0, FOG_TABLE_COMMAND_SIZE);
    }
    pSampler->commandSize = FOG_TABLE_COMMAND_SIZE;
    pSampler->commands.Set(pCommands);
    return pResource;
}

// 0x004A4B1C | nintendogs:bytes [tier A]
void nw::gfx::Fog::DestroyResFog(os::IAllocator* pAllocator, res::ResFogData* pResource)
{
    if (pResource->name.ToPtr() != nullptr) {
        pAllocator->Free(pResource->name.ToPtr());
    }
    if (pResource->updater.ToPtr() != nullptr) {
        pAllocator->Free(pResource->updater.ToPtr());
    }
    res::ResImageLookupTableData* pSampler = pResource->sampler.To<res::ResImageLookupTableData>();
    if (pSampler != nullptr) {
        if (pSampler->commands.ToPtr() != nullptr) {
            pAllocator->Free(pSampler->commands.ToPtr());
        }
        pAllocator->Free(pResource->sampler.ToPtr());
    }
    pAllocator->Free(pResource);
}

// 0x004A4CB0 | nintendogs:bytes [tier A]
u32 nw::gfx::Fog::CreateAnimGroup(os::IAllocator* pAllocator)
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

    // only the members of the fog resource (the copy for the original values)
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(i);
        if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_FOG) {
            void* pObject = const_cast<res::ResSceneObjectData*>(GetResSceneObject().ptr());
            pAnimGroup->m_Targets[i] = pObject;
            pAnimGroup->m_TargetValues[i] = static_cast<u8*>(pObject) + member.ptr()->valueOffset;
            pAnimGroup->m_OriginalValues[i] = reinterpret_cast<u8*>(m_pOriginalValue) + member.ptr()->valueOffset;
        }
        pAnimGroup->m_TargetObjects[i] = nullptr;
    }
    m_AnimBinding->m_AnimGroups[0] = pAnimGroup;
    m_pAnimGroup = pAnimGroup;
    return result;
}

// 0x004A4E4C | nintendogs:callgraph [tier A]
void nw::gfx::Fog::SetupFogSampler(res::ResImageLookupTable lookupTable, res::ResFogUpdater updater, const nn::math::MTX44& inverseProjection)
{
    res::ResImageLookupTableData* pTable = const_cast<res::ResImageLookupTableData*>(lookupTable.ptr());
    pTable->commands.To<u32>()[0] = 0;
    pTable->commands.To<u32>()[1] = Header(REG_FOG_LUT_INDEX);
    pTable->commands.To<u32>()[3] = Header(REG_FOG_LUT_DATA0, 0xF, FOG_TABLE_SIZE - 1);

    const res::ResFogUpdaterData* pUpdater = updater.ptr();
    f32 previous = 0.0f;
    int entryCount = 0;
    for (int i = 0; i <= FOG_TABLE_SIZE; ++i) {
        // the depth in view space: from the depth buffer value (w buffer off) or between near and far
        f32 depth;
        if (m_WScale == 0.0f) {
            const f32 z = static_cast<f32>(i) * (-1.0f / FOG_TABLE_SIZE);
            depth = -(inverseProjection.m[2][3] + inverseProjection.m[2][2] * z) / (inverseProjection.m[3][3] + inverseProjection.m[3][2] * z);
        } else {
            depth = m_Near + static_cast<f32>(i) * (1.0f / FOG_TABLE_SIZE) * (m_Far - m_Near);
        }

        // how much of the color remains (1: no fog)
        f32 fog;
        if (depth <= pUpdater->start) {
            fog = 1.0f;
        } else if (depth > pUpdater->end) {
            fog = 0.0f;
        } else {
            fog = depth;
            switch (static_cast<u8>(pUpdater->fogType)) {
            case res::ResFogUpdaterData::FOG_TYPE_LINEAR:
                fog = (pUpdater->end - depth) / (pUpdater->end - pUpdater->start);
                break;
            case res::ResFogUpdaterData::FOG_TYPE_EXPONENT:
                fog = std::exp(-((depth - pUpdater->start) * pUpdater->density) / (pUpdater->end - pUpdater->start));
                break;
            case res::ResFogUpdaterData::FOG_TYPE_EXPONENT_SQUARE: {
                const f32 x = (depth - pUpdater->start) * pUpdater->density / (pUpdater->end - pUpdater->start);
                fog = std::exp(-(x * x));
                break;
            }
            default:
                break;
            }
        }

        // an entry: the value at the previous depth and the change to this one
        if (i > 0) {
            const u32 entry = FogDeltaToFix13(fog - previous) | (FogValueToFix11(previous) << 13);
            pTable->commands.To<u32>()[entryCount == 0 ? 2 : entryCount + 3] = entry;
            ++entryCount;
        }
        previous = fog;
    }
}

// 0x004A50E4 | nintendogs:bytes [tier A]
u32 nw::gfx::Fog::CreateOriginalValue(os::IAllocator* pAllocator)
{
    res::ResFogData* pCopy = static_cast<res::ResFogData*>(pAllocator->Alloc(sizeof(res::ResFogData), 4));
    if (pCopy != nullptr) {
        const res::ResFogData& resource = *static_cast<const res::ResFogData*>(GetResSceneObject().ptr());
        res::CopyResTransformNodeData(pCopy, resource);
        pCopy->color = resource.color;
        pCopy->sampler.m_Offset = resource.sampler.m_Offset;
        pCopy->updater.m_Offset = resource.updater.m_Offset;
    }
    m_pOriginalValue = pCopy;
    return RESULT_SUCCESS;
}

// 0x004A5188 | nintendogs:bytes [tier A]
void nw::gfx::Fog::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResFog resFog, Description description)
{
    pSize->Add(sizeof(Fog));
    TransformNode::GetMemorySizeForInitialize(pSize, resFog, description);
    pSize->Add(sizeof(res::ResFogUpdaterData));
    // CreateAnimGroup
    if (description.isAnimationEnabled && resFog.ptr()->animGroupCount > 0) {
        pSize->Add(sizeof(AnimGroup));
        AnimGroup::GetMemorySizeForInitialize(pSize, res::ResSceneObject(resFog.ptr()).GetAnimGroup(0), true);
    }
    // CreateOriginalValue
    pSize->Add(sizeof(res::ResFogData));
}

// 0x004A5298 slot 0x10 | nintendogs:bytes
void nw::gfx::Fog::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitFog(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004A52EC (name is ours)
nw::gfx::Fog* nw::gfx::Fog::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator)
{
    const res::ResFogData* pResource = res::DynamicCastData<res::ResFogData>(resObject.ptr(), res::ResFogData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(Fog), 4);
    Fog* pFog = nullptr;
    if (pMemory != nullptr) {
        pFog = new (pMemory) Fog(pAllocator, res::ResTransformNode(pResource), description, nullptr, nullptr);
    }
    pFog->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pFog);
    }
    return pFog;
}

// 0x004A550C | nintendogs:bytes [tier A]
void nw::gfx::Fog::Update(const Camera* pCamera)
{
    const res::ResFogData* pResource = static_cast<const res::ResFogData*>(GetResSceneObject().ptr());
    const res::ResFogUpdaterData* pUpdater = pResource->updater.To<res::ResFogUpdaterData>();
    const res::ResImageLookupTableData* pSampler = pResource->sampler.To<res::ResImageLookupTableData>();
    const res::ResCameraProjectionUpdaterData* pProjection =
        static_cast<const CameraProjectionUpdater*>(pCamera->m_ProjectionUpdater.Get())->GetResource().ptr();
    const f32 near = pProjection->near;
    const f32 far = pProjection->far;
    const f32 wScale = pCamera->m_WScale;

    // the table is made again only when the camera or the updater changed
    if (ut::FloatEqualsWeak(m_Near, near) && ut::FloatEqualsWeak(m_Far, far) && ut::FloatEqualsWeak(m_WScale, wScale) &&
        ut::FloatEqualsWeak(m_pLastUpdater->density, pUpdater->density) &&
        static_cast<u8>(pUpdater->fogType) == m_pLastUpdater->fogType && ut::FloatEqualsWeak(m_pLastUpdater->end, pUpdater->end) &&
        ut::FloatEqualsWeak(m_pLastUpdater->start, pUpdater->start)) {
        return;
    }
    m_Near = near;
    m_Far = far;
    m_WScale = wScale;
    m_pLastUpdater->density = pUpdater->density;
    m_pLastUpdater->fogType = static_cast<u8>(pUpdater->fogType);
    m_pLastUpdater->end = pUpdater->end;
    m_pLastUpdater->start = pUpdater->start;
    SetupFogSampler(res::ResImageLookupTable(pSampler), res::ResFogUpdater(pUpdater), pCamera->m_InverseProjectionMatrix);
}

// 0x004A5650 slot 0x00 | nintendogs:bytes-fuzzy
// 0x004A5644 (deleting dtor)
nw::gfx::Fog::~Fog()
{
    if (m_pAnimGroup != nullptr) {
        m_pAnimGroup->~AnimGroup();
        m_pAnimGroup->GetAllocator()->Free(m_pAnimGroup);
        m_pAnimGroup = nullptr;
    }
    if (m_pOriginalValue != nullptr) {
        GetAllocator()->Free(m_pOriginalValue);
        m_pOriginalValue = nullptr;
    }
    if (m_pLastUpdater != nullptr) {
        m_pLastUpdaterAllocator->Free(m_pLastUpdater);
    }
    if (m_pOwnResource != nullptr) {
        DestroyResFog(m_pResourceAllocator, m_pOwnResource);
    }
}

} // namespace gfx
} // namespace nw
