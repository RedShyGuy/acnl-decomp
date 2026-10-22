#include <new>

#include "nw/gfx/gfx_SceneEnvironmentSetting.h"

namespace nw {
namespace gfx {
// 0x0097F498
const TypeInfo nw::gfx::SceneEnvironmentSetting::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FCF0 (static initializer)

// 0x004A208C | nintendogs:bytes [tier A]
void nw::gfx::SceneEnvironmentSetting::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResSceneEnvironmentSetting resSetting,
                                                             Description /*description*/)
{
    const res::ResSceneEnvironmentSettingData* pResource = resSetting.ptr();
    pSize->Add(sizeof(SceneEnvironmentSetting));
    pSize->Add(pResource->lightSets.count * sizeof(LightSetSlot));
    const ut::Offset* pBegin = pResource->lightSets.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + pResource->lightSets.count;
    for (const ut::Offset* pIt = pResource->lightSets.array.To<ut::Offset>(); pIt != pEnd; ++pIt) {
        pSize->Add(sizeof(LightSet));
        pSize->Add(pIt->To<res::ResLightSetData>()->lightCount * sizeof(Light*));
    }
    pSize->Add(pResource->cameras.count * sizeof(ReferenceSlot));
    pSize->Add(pResource->fogs.count * sizeof(ReferenceSlot));
}

// 0x004A21BC (name is ours)
void nw::gfx::SceneEnvironmentSetting::Setup(os::IAllocator* pAllocator, res::ResSceneEnvironmentSetting resSetting)
{
    const res::ResSceneEnvironmentSettingData* pResource = resSetting.ptr();
    if (pResource->lightSets.count != 0) {
        {
            void* pBuffer = pAllocator->Alloc(pResource->lightSets.count * sizeof(LightSetSlot), 4);
            ut::MoveArray<LightSetSlot> lightSets(pBuffer, pResource->lightSets.count, pAllocator);
            m_LightSets = lightSets;
        }
        m_LightSets.Resize(pResource->lightSets.count);
        const ut::Offset* pBegin = pResource->lightSets.array.To<ut::Offset>();
        const ut::Offset* pEnd = pBegin + pResource->lightSets.count;
        int index = 0;
        for (const ut::Offset* pIt = pResource->lightSets.array.To<ut::Offset>(); pIt != pEnd; ++pIt, ++index) {
            m_LightSets[index].lightSet = GfxPtr<LightSet>(LightSet::Create(res::ResLightSet(pIt->ToPtr()), pAllocator));
            m_LightSets[index].index = -1;
        }
    }
    if (pResource->cameras.count != 0) {
        void* pBuffer = pAllocator->Alloc(pResource->cameras.count * sizeof(ReferenceSlot), 4);
        ut::MoveArray<ReferenceSlot> cameras(pBuffer, pResource->cameras.count, pAllocator);
        m_Cameras = cameras;
    }
    if (pResource->fogs.count != 0) {
        void* pBuffer = pAllocator->Alloc(pResource->fogs.count * sizeof(ReferenceSlot), 4);
        ut::MoveArray<ReferenceSlot> fogs(pBuffer, pResource->fogs.count, pAllocator);
        m_Fogs = fogs;
    }
}

// 0x004A2888 (name is ours)
nw::gfx::SceneEnvironmentSetting* nw::gfx::SceneEnvironmentSetting::Create(res::ResSceneObject resObject, const Description& /*description*/,
                                                                           os::IAllocator* pAllocator)
{
    const void* pResource = (resObject.ptr() != nullptr && (resObject.ptr()->typeInfo & res::ResSceneEnvironmentSettingData::TYPE))
                                ? resObject.ptr()
                                : nullptr;
    void* pMemory = pAllocator->Alloc(sizeof(SceneEnvironmentSetting), 4);
    SceneEnvironmentSetting* pSetting =
        pMemory != nullptr ? new (pMemory) SceneEnvironmentSetting(pAllocator, res::ResSceneObject(pResource)) : nullptr;
    pSetting->Setup(pAllocator, res::ResSceneEnvironmentSetting(pResource));
    return pSetting;
}

// 0x004A2934 slot 0x00 (name is ours)
// 0x004A2928 (deleting dtor)
nw::gfx::SceneEnvironmentSetting::~SceneEnvironmentSetting()
{
    // empty (in the original too: only the arrays)
}

} // namespace gfx
} // namespace nw
