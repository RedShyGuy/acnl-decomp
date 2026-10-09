#include "nn/snd/CTR/snd_DspFxManager.h"
#include "nn/snd/CTR/snd_DspFxManagerImpl.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// the DSP cycles of an effect per channel
const u32 DELAY_CYCLES = 10000;
const u32 REVERB_CYCLES = 40000;

// 0x00AEB828 (name is ours)
DspFxManager s_DspFxManager;
} // namespace

// 0x0046143C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
DspFxManager* nn::snd::CTR::DspFxManager::GetInstance()
{
    return &s_DspFxManager;
}

// 0x00461448 | nintendogs:bytes [confirmed by fefates] [tier A]
u32 nn::snd::CTR::DspFxManager::GetDspCycles()
{
    u32 cycles = 0;
    for (s8 bus = 0; bus < AUX_BUS_COUNT; bus++) {
        cycles += (m_IsEnabled[DSP_EFFECT_TYPE_DELAY][bus] ? m_ChannelCount[DSP_EFFECT_TYPE_DELAY][bus] : 0) * DELAY_CYCLES;
        cycles += (m_IsEnabled[DSP_EFFECT_TYPE_REVERB][bus] ? m_ChannelCount[DSP_EFFECT_TYPE_REVERB][bus] : 0) * REVERB_CYCLES;
    }
    return cycles;
}

// 0x004614BC | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManager::Attach(DspEffectType type, AuxBusId bus)
{
    if (m_IsAttached[type][bus]) {
        return false;
    }
    m_IsAttached[type][bus] = true;
    return true;
}

// 0x004614DC | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManager::Detach(DspEffectType type, AuxBusId bus)
{
    m_IsAttached[type][bus] = false;
    return true;
}

// 0x00462A2C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::DspFxManager::Initialize()
{
    for (s32 bus = 0; bus < AUX_BUS_COUNT; bus++) {
        for (s32 type = 0; type < DSP_EFFECT_TYPE_COUNT; type++) {
            m_IsAttached[type][bus] = false;
            m_IsEnabled[type][bus] = false;
            m_ChannelCount[type][bus] = 0;
        }
    }
    DspFxManagerImpl::GetInstance()->Initialize();
}

// 0x00462C48 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManager::SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params)
{
    m_IsEnabled[DSP_EFFECT_TYPE_DELAY][bus] = params.m_IsEnabled != 0;
    return DspFxManagerImpl::GetInstance()->SetDspDelayEffect(bus, params);
}

// 0x00462CE4 | fefates:bytes [tier B]
bool nn::snd::CTR::DspFxManager::SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params)
{
    m_IsEnabled[DSP_EFFECT_TYPE_REVERB][bus] = params.m_IsEnabled != 0;
    return DspFxManagerImpl::GetInstance()->SetDspReverbEffect(bus, params);
}

// 0x00462DE8 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::DspFxManager::Finalize()
{
    DspFxManagerImpl::GetInstance()->Finalize();
}

} // namespace CTR
} // namespace snd
} // namespace nn
