#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_LightSet.h"
#include "nw/gfx/gfx_SceneEnvironment.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {
namespace {
// (inline) whether the resource of the light enables it (name is ours)
inline bool IsLightEnabled(const Light* pLight)
{
    return static_cast<const res::ResLightData*>(pLight->GetResSceneObject().ptr())->isLightEnabled;
}
} // namespace

// 0x0049C040 | nintendogs:bytes [tier B]
void nw::gfx::SceneEnvironment::SetActiveLightSet(int lightSetIndex)
{
    LightSet* pLightSet = m_LightSets[lightSetIndex];
    if (pLightSet == nullptr || m_ActiveLightSetIndex == lightSetIndex) {
        return;
    }
    m_ActiveLightSetIndex = lightSetIndex;

    if (m_pAmbientLight != pLightSet->m_pAmbientLight) {
        m_IsAmbientLightDirty = true;
        AmbientLight* pAmbientLight = pLightSet->m_pAmbientLight;
        m_pAmbientLight = (pAmbientLight != nullptr && IsLightEnabled(pAmbientLight)) ? pAmbientLight : nullptr;
    }
    if (m_pHemiSphereLight != pLightSet->m_pHemiSphereLight) {
        m_IsHemiSphereLightDirty = true;
        HemiSphereLight* pHemiSphereLight = pLightSet->m_pHemiSphereLight;
        m_pHemiSphereLight = (pHemiSphereLight != nullptr && IsLightEnabled(pHemiSphereLight)) ? pHemiSphereLight : nullptr;
    }

    // the enabled fragment lights, as many as the hardware has
    m_FragmentLightCount = 0;
    for (Light** ppLight = pLightSet->m_FragmentLights.begin(); ppLight != pLightSet->m_FragmentLights.end(); ++ppLight) {
        m_IsFragmentLightDirty = true;
        if (m_FragmentLightCount == FRAGMENT_LIGHT_MAX) {
            break;
        }
        if (IsLightEnabled(*ppLight)) {
            m_FragmentLights[m_FragmentLightCount] = *ppLight;
            ++m_FragmentLightCount;
        }
    }
}

// 0x0049C138 | nintendogs:bytes [tier B]
void nw::gfx::SceneEnvironment::Reset()
{
    m_ActiveLightSetIndex = -1;
    m_pAmbientLight = nullptr;
    m_IsAmbientLightDirty = true;
    m_pHemiSphereLight = nullptr;
    m_IsHemiSphereLightDirty = true;
    for (int i = 0; i < m_FragmentLightCount; ++i) {
        m_FragmentLights[i] = nullptr;
    }
    m_FragmentLightCount = 0;
    m_IsFragmentLightDirty = true;
    m_pFog = nullptr;
    m_IsFogDirty = true;
    m_pCamera = nullptr;
    m_ActiveCameraIndex = -1;
}

// 0x0049C1A8 | nintendogs:bytes [tier B]
nw::gfx::SceneEnvironment::SceneEnvironment(const Description& description)
    : m_pAmbientLight(nullptr), m_pHemiSphereLight(nullptr), m_VertexLights(const_cast<Description&>(description).vertexLights),
      m_FragmentLights(m_FragmentLightBuffer, FRAGMENT_LIGHT_MAX, nullptr, ut::MoveArray<Light*>::KIND_INLINE), m_pFog(nullptr),
      m_pCamera(nullptr), m_Cameras(const_cast<Description&>(description).cameras), m_ActiveCameraIndex(-1),
      m_Fogs(const_cast<Description&>(description).fogs), m_LightSets(const_cast<Description&>(description).lightSets),
      m_ActiveLightSetIndex(-1), m_Unknown0xA0(0), m_FragmentLightCount(0), m_IsFragmentLightDirty(true), m_IsAmbientLightDirty(true),
      m_IsUnknown0xAADirty(true), m_IsHemiSphereLightDirty(true), m_IsFogDirty(true)
{
    // all slots there; the cameras, fogs and light sets empty
    m_FragmentLights.Resize(m_FragmentLights.capacity());
    m_VertexLights.Resize(m_VertexLights.capacity());
    m_Cameras.Resize(m_Cameras.capacity());
    for (Camera** ppCamera = m_Cameras.begin(); ppCamera != m_Cameras.end(); ++ppCamera) {
        *ppCamera = nullptr;
    }
    m_Fogs.Resize(m_Fogs.capacity());
    for (Fog** ppFog = m_Fogs.begin(); ppFog != m_Fogs.end(); ++ppFog) {
        *ppFog = nullptr;
    }
    m_LightSets.Resize(m_LightSets.capacity());
    for (LightSet** ppLightSet = m_LightSets.begin(); ppLightSet != m_LightSets.end(); ++ppLightSet) {
        *ppLightSet = nullptr;
    }
}

} // namespace gfx
} // namespace nw
