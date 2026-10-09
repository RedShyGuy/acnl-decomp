#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// Fragment lighting: eight light sources and the lighting environment. Member names are ours;
// the fields are the PICA lighting registers on 3dbrew. Colors are 8 bit values written into the
// 10 bit register fields.
class FragmentLight
{
public:
    static const s32 LIGHT_COUNT = 8;
    static const s32 LUT_COUNT = 7; // D0, D1, SP, FR, RB, RG, RR

    class Source
    {
    public:
        Source(); // 0x00349A80 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]

        u32* MakeAllCommand(u32* command) const; // 0x007275FC | nintendogs:bytes [confirmed by fefates] [tier A]

        u8 m_Id;                        // 0x00, light index
        u8 m_IsTwoSideDiffuse;        // 0x01
        u8 m_IsGeometricFactor0;      // 0x02
        u8 m_IsGeometricFactor1;      // 0x03
        u8 m_DiffuseR;                  // 0x04
        u8 m_DiffuseG;                  // 0x05
        u8 m_DiffuseB;                  // 0x06
        u8 m_AmbientR;                  // 0x07
        u8 m_AmbientG;                  // 0x08
        u8 m_AmbientB;                  // 0x09
        u8 m_Specular0R;                // 0x0A
        u8 m_Specular0G;                // 0x0B
        u8 m_Specular0B;                // 0x0C
        u8 m_Specular1R;                // 0x0D
        u8 m_Specular1G;                // 0x0E
        u8 m_Specular1B;                // 0x0F
        u32 m_PositionXY;               // 0x10, two float16
        u32 m_PositionZ;                // 0x14
        u8 m_IsDirectional;             // 0x18
        u32 m_DistanceAttenuationBias;  // 0x1C, float20
        u32 m_DistanceAttenuationScale; // 0x20, float20
        u32 m_SpotDirectionXY;          // 0x24
        u32 m_SpotDirectionZ;           // 0x28
    };

    struct LutConfig
    {
        u8 m_Input;     // 0x0
        u8 m_IsAbs;     // 0x1
        u8 m_Scale;     // 0x2
        u8 m_Padding;   // 0x3
    };

    FragmentLight(); // 0x00349AE8 | nintendogs:bytes-fuzzy [confirmed by fefates] [tier A]

    static u32* MakeDisableCommand(u32* command, bool isResetTextureConfig); // 0x00349A28 | fefates:bytes [tier B]
    u32* MakeAllCommand(u32* command, bool isResetTextureConfig) const; // 0x007271F0 | nintendogs:bytes [tier B]
    DECOMP_NOINLINE u32* MakeLightEnvCommand(u32* command, bool isResetTextureConfig) const; // 0x0072724C | nintendogs:callgraph [confirmed by fefates] [tier A]
    DECOMP_NOINLINE u32* MakeLutConfigCommand(u32* command) const; // 0x007274BC | nintendogs:bytes [tier B]

    u8 m_GlobalAmbientR;                                // 0x000
    u8 m_GlobalAmbientG;                                // 0x001
    u8 m_GlobalAmbientB;                                // 0x002
    Source m_Sources[LIGHT_COUNT];                      // 0x004
    bool m_IsEnabled[LIGHT_COUNT];                      // 0x164
    bool m_IsEnableSpot[LIGHT_COUNT];                   // 0x16C
    bool m_IsEnableDistanceAttenuation[LIGHT_COUNT];    // 0x174
    bool m_IsEnableShadow[LIGHT_COUNT];                 // 0x17C
    u8 m_Config;                                        // 0x184
    u8 m_FresnelSelector;                               // 0x185, 0 = no fresnel
    u8 m_ShadowSelector;                                // 0x186, texture unit of the shadow
    u8 m_BumpMode;                                      // 0x187
    u8 m_BumpSelector;                                  // 0x188
    bool m_IsShadowPrimary;                             // 0x189
    bool m_IsShadowSecondary;                           // 0x18A
    bool m_IsShadowAlpha;                               // 0x18B
    u8 m_IsInvertShadow;                              // 0x18C
    bool m_IsBumpRenorm;                                // 0x18D
    u8 m_IsClampHighlights;                           // 0x18E
    u8 m_IsEnableLutD0;                               // 0x18F
    u8 m_IsEnableLutD1;                               // 0x190
    u8 m_IsEnableReflection;                          // 0x191
    u8 m_Padding192[2];                                 // 0x192
    LutConfig m_Luts[LUT_COUNT];                        // 0x194
};
ASSERT_SIZE(FragmentLight::Source, 0x2C);
ASSERT_SIZE(FragmentLight, 0x1B0);
} // namespace CTR
} // namespace gr
} // namespace nn
