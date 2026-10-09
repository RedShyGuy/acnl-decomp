#include "nn/snd/CTR/snd_DspFxManagerImpl.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// 0x00AF624C (name is ours)
DspFxManagerImpl s_DspFxManagerImpl;
} // namespace

// 0x00462A70 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::DspFxManagerImpl::Initialize()
{
    // all effects off
    for (s8 i = 0; i < AUX_BUS_COUNT; i++) {
        const AuxBusId bus = static_cast<AuxBusId>(i);
        DspFxDelayParams delay;
        delay.m_Flags = DspFxDelayParams::FLAG_ENABLE;
        delay.m_IsEnabled = 0;
        SetDspDelayEffect(bus, delay);
        DspFxReverbParams reverb;
        reverb.m_Flags = DspFxDelayParams::FLAG_ENABLE;
        reverb.m_IsEnabled = 0;
        SetDspReverbEffect(bus, reverb);
    }
}

// 0x00462BDC | nintendogs:callgraph [confirmed by fefates] [tier A]
DspFxManagerImpl* nn::snd::CTR::DspFxManagerImpl::GetInstance()
{
    return &s_DspFxManagerImpl;
}

// 0x00462BE8 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::DspFxManagerImpl::ForceUpdateParams()
{
    for (s8 i = 0; i < AUX_BUS_COUNT; i++) {
        const AuxBusId bus = static_cast<AuxBusId>(i);
        m_Delay[bus].m_Flags = 0xFFFF;
        g_Dspsnd.SetDspDelayEffect(bus, m_Delay[bus]);
        m_Reverb[bus].m_Flags = 0xFFFF;
        g_Dspsnd.SetDspReverbEffect(bus, m_Reverb[bus]);
    }
}

// 0x00462C7C | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManagerImpl::SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params)
{
    DspFxDelayParams* delay = &m_Delay[bus];
    if (params.m_Flags & DspFxDelayParams::FLAG_ENABLE) {
        delay->m_IsEnabled = params.m_IsEnabled;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_PARAM) {
        delay->m_ChannelCount = params.m_ChannelCount;
        delay->m_DelaySamples = params.m_DelaySamples;
        delay->m_Feedback = params.m_Feedback;
        delay->m_LpfB = params.m_LpfB;
        delay->m_LpfA = params.m_LpfA;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_BUFFER) {
        delay->m_BufferAddress = params.m_BufferAddress;
    }
    return g_Dspsnd.SetDspDelayEffect(bus, params);
}

// 0x00462D18 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManagerImpl::SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params)
{
    DspFxReverbParams* reverb = &m_Reverb[bus];
    if (params.m_Flags & DspFxDelayParams::FLAG_ENABLE) {
        reverb->m_IsEnabled = params.m_IsEnabled;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_PARAM) {
        reverb->m_ChannelCount = params.m_ChannelCount;
        reverb->m_EarlyReflectionSamples = params.m_EarlyReflectionSamples;
        reverb->m_FusedSamples = params.m_FusedSamples;
        reverb->m_FilterSize[0] = params.m_FilterSize[0];
        reverb->m_FilterSize[1] = params.m_FilterSize[1];
        reverb->m_FilterSize[2] = params.m_FilterSize[2];
        reverb->m_EarlyGain = params.m_EarlyGain;
        reverb->m_FusedGain = params.m_FusedGain;
        reverb->m_Coloration = params.m_Coloration;
        reverb->m_CombGain[0] = params.m_CombGain[0];
        reverb->m_CombGain[1] = params.m_CombGain[1];
        reverb->m_LpfB = params.m_LpfB;
        reverb->m_LpfA = params.m_LpfA;
    }
    if (params.m_Flags & DspFxDelayParams::FLAG_BUFFER) {
        for (s32 i = 0; i < 5; i++) {
            reverb->m_BufferAddress[i] = params.m_BufferAddress[i];
        }
    }
    return g_Dspsnd.SetDspReverbEffect(bus, params);
}

} // namespace CTR
} // namespace snd
} // namespace nn
