#pragma once

#include "decomp.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class AmbientLight;
class Camera;
class Fog;
class HemiSphereLight;
class Light;
class LightSet;

// The environment a scene is drawn in: the cameras, fogs and light sets, and the lights of the active
// light set (the class name, SetActiveLightSet and Reset are from the reference symbols, the rest is
// ours). The dirty flags tell the render context what to send again.
class SceneEnvironment
{
public:
    // the arrays move into the environment
    struct Description
    {
        ut::MoveArray<Camera*> cameras;       // 0x00
        ut::MoveArray<Fog*> fogs;             // 0x10
        ut::MoveArray<LightSet*> lightSets;   // 0x20
        ut::MoveArray<Light*> vertexLights;   // 0x30
    };
    // the fragment lights of the hardware
    static const int FRAGMENT_LIGHT_MAX = 8;

    explicit SceneEnvironment(const Description& description); // 0x0049C1A8 | nintendogs:bytes [tier B]

    // the lights of the light set (only the enabled ones); nothing if it is not there or active already
    void SetActiveLightSet(int lightSetIndex); // 0x0049C040 | nintendogs:bytes [tier B]
    // no lights, no fog, no camera
    void Reset(); // 0x0049C138 | nintendogs:bytes [tier B]

    AmbientLight* m_pAmbientLight;            // 0x00
    HemiSphereLight* m_pHemiSphereLight;      // 0x04
    ut::MoveArray<Light*> m_VertexLights;     // 0x08
    u32 m_Unknown0x18[2];                     // 0x18
    ut::MoveArray<Light*> m_FragmentLights;   // 0x20 in m_FragmentLightBuffer
    u32 m_Unknown0x30[4];                     // 0x30
    Light* m_FragmentLightBuffer[FRAGMENT_LIGHT_MAX]; // 0x40
    Fog* m_pFog;                              // 0x60
    Camera* m_pCamera;                        // 0x64
    ut::MoveArray<Camera*> m_Cameras;         // 0x68
    s32 m_ActiveCameraIndex;                  // 0x78
    ut::MoveArray<Fog*> m_Fogs;               // 0x7C
    ut::MoveArray<LightSet*> m_LightSets;     // 0x8C
    s32 m_ActiveLightSetIndex;                // 0x9C
    u32 m_Unknown0xA0;                        // 0xA0
    s32 m_FragmentLightCount;                 // 0xA4
    bool m_IsFragmentLightDirty;              // 0xA8
    bool m_IsAmbientLightDirty;               // 0xA9
    bool m_IsUnknown0xAADirty;                // 0xAA
    bool m_IsHemiSphereLightDirty;            // 0xAB
    bool m_IsFogDirty;                        // 0xAC
};
ASSERT_OFFSET(SceneEnvironment, m_Cameras, 0x68);
ASSERT_OFFSET(SceneEnvironment, m_FragmentLightCount, 0xA4);
} // namespace gfx
} // namespace nw
