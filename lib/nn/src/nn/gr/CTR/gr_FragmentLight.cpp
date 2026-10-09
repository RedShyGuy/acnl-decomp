#include "nn/gr/CTR/gr_FragmentLight.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

namespace {
inline u32* MakeResetTextureConfigCommand(u32* command)
{
    *command++ = 0;
    *command++ = CommandHeader(0x80, 0x0, 2);
    *command++ = 0;
    *command++ = 0;
    return command;
}
} // namespace

// 0x00349A28 | fefates:bytes [tier B]
u32* nn::gr::CTR::FragmentLight::MakeDisableCommand(u32* command, bool isResetTextureConfig)
{
    if (isResetTextureConfig) {
        command = MakeResetTextureConfigCommand(command);
    }
    *command++ = 0;
    *command++ = CommandHeader(0x8F);
    *command++ = 1;
    *command++ = CommandHeader(0x1C6);
    *command++ = 0;
    *command++ = CommandHeader(0x1C2);
    return command;
}

// 0x00349AE8 | nintendogs:bytes-fuzzy [confirmed by fefates] [tier A]
nn::gr::CTR::FragmentLight::FragmentLight()
    : m_GlobalAmbientR(0), m_GlobalAmbientG(0), m_GlobalAmbientB(0), m_Config(0), m_FresnelSelector(0),
      m_ShadowSelector(0), m_BumpMode(0), m_BumpSelector(0), m_IsShadowPrimary(false), m_IsShadowSecondary(false),
      m_IsShadowAlpha(false), m_IsInvertShadow(false), m_IsBumpRenorm(false), m_IsClampHighlights(true),
      m_IsEnableLutD0(false), m_IsEnableLutD1(false), m_IsEnableReflection(true)
{
    for (s32 i = 0; i < LUT_COUNT; ++i) {
        m_Luts[i].m_Input = 0;
        m_Luts[i].m_IsAbs = false;
        m_Luts[i].m_Scale = 0;
    }
    for (s32 i = 0; i < LIGHT_COUNT; ++i) {
        m_IsEnabled[i] = false;
        m_IsEnableSpot[i] = false;
        m_IsEnableDistanceAttenuation[i] = false;
        m_IsEnableShadow[i] = false;
        m_Sources[i].m_Id = i;
    }
}

// 0x007271F0 | nintendogs:bytes [tier B]
u32* nn::gr::CTR::FragmentLight::MakeAllCommand(u32* command, bool isResetTextureConfig) const
{
    command = MakeLutConfigCommand(command);
    command = MakeLightEnvCommand(command, isResetTextureConfig);
    for (s32 i = 0; i < LIGHT_COUNT; ++i) {
        if (m_IsEnabled[i]) {
            command = m_Sources[i].MakeAllCommand(command);
        }
    }
    return command;
}

// 0x0072724C | nintendogs:callgraph [confirmed by fefates] [tier A]
u32* nn::gr::CTR::FragmentLight::MakeLightEnvCommand(u32* command, bool isResetTextureConfig) const
{
    // the register holds "disable" bits
    u32 config1 = (!m_IsEnableLutD0 << 16) | (!m_IsEnableLutD1 << 17) | ((m_FresnelSelector == 0) << 19)
        | ((m_IsEnableReflection ? 0 : 7) << 20) | 0xFF04FFFF;
    u32 permutation = 0;
    s32 count = 0;
    for (s32 i = 0; i < LIGHT_COUNT; ++i) {
        if (!m_IsEnabled[i]) {
            continue;
        }
        if (m_IsEnableShadow[i]) {
            config1 &= ~(1 << i);
        }
        if (m_IsEnableSpot[i]) {
            config1 &= ~(1 << (i + 8));
        }
        if (m_IsEnableDistanceAttenuation[i]) {
            config1 &= ~(1 << (i + 24));
        }
        permutation |= i << (count * 4);
        ++count;
    }

    if (isResetTextureConfig) {
        command = MakeResetTextureConfigCommand(command);
    }
    *command++ = (count != 0) ? 1 : 0;
    *command++ = CommandHeader(0x8F);
    *command++ = (m_GlobalAmbientG << 10) | (m_GlobalAmbientR << 20) | m_GlobalAmbientB;
    *command++ = CommandHeader(0x1C0);
    *command++ = (count > 0) ? count - 1 : 0;
    *command++ = CommandHeader(0x1C2);
    *command++ = ((m_IsShadowPrimary | m_IsShadowSecondary | m_IsShadowAlpha) ? 1 : 0) | (m_FresnelSelector << 2)
        | (m_Config << 4) | ((m_IsShadowPrimary ? 1 : 0) << 16) | ((m_IsShadowSecondary ? 1 : 0) << 17)
        | ((m_IsInvertShadow ? 1 : 0) << 18) | ((m_IsShadowAlpha ? 1 : 0) << 19) | (m_BumpSelector << 22)
        | (m_ShadowSelector << 24) | ((m_IsClampHighlights ? 1 : 0) << 27) | (m_BumpMode << 28)
        | ((m_BumpMode != 0 && !m_IsBumpRenorm) ? 1 : 0) << 30 | 0x80000400;
    *command++ = CommandHeader(0x1C3);
    *command++ = config1;
    *command++ = CommandHeader(0x1C4);
    *command++ = (count == 0) ? 1 : 0;
    *command++ = CommandHeader(0x1C6);
    *command++ = permutation;
    *command++ = CommandHeader(0x1D9);
    return command;
}

// 0x007274BC | nintendogs:bytes [tier B]
u32* nn::gr::CTR::FragmentLight::MakeLutConfigCommand(u32* command) const
{
    command[0] = (!m_Luts[0].m_IsAbs << 1) | (!m_Luts[1].m_IsAbs << 5) | (!m_Luts[2].m_IsAbs << 9)
        | (!m_Luts[3].m_IsAbs << 13) | (!m_Luts[4].m_IsAbs << 17) | (!m_Luts[5].m_IsAbs << 21) | (!m_Luts[6].m_IsAbs << 25);
    command[1] = CommandHeader(0x1D0);
    command[2] = m_Luts[0].m_Input | (m_Luts[1].m_Input << 4) | (m_Luts[2].m_Input << 8) | (m_Luts[3].m_Input << 12)
        | (m_Luts[4].m_Input << 16) | (m_Luts[5].m_Input << 20) | (m_Luts[6].m_Input << 24);
    command[3] = CommandHeader(0x1D1);
    command[4] = m_Luts[0].m_Scale | (m_Luts[1].m_Scale << 4) | (m_Luts[2].m_Scale << 8) | (m_Luts[3].m_Scale << 12)
        | (m_Luts[4].m_Scale << 16) | (m_Luts[5].m_Scale << 20) | (m_Luts[6].m_Scale << 24);
    command[5] = CommandHeader(0x1D2);
    return command + 6;
}

} // namespace CTR
} // namespace gr
} // namespace nn
