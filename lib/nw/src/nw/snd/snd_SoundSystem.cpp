#include "nw/snd/snd_SoundSystem.h"

#include "nw/snd/internal/driver/snd_ChannelManager.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_CurveLfo.h"
#include "nw/snd/internal/snd_DriverCommand.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_TaskManager.h"
#include "nw/snd/internal/snd_TaskThread.h"
#include "nw/snd/internal/snd_ThreadStack.h"
#include "nw/snd/snd_FxBase.h"

namespace nw {
namespace snd {
// 0x00976194
bool nw::snd::SoundSystem::s_IsEnableSoundThread = false;
// 0x00976195
bool nw::snd::SoundSystem::s_IsInitialized = false;
// 0x00976196
bool nw::snd::SoundSystem::s_IsInitializedDriverCommandManager = false;
// 0x00976198
int nw::snd::SoundSystem::s_VoiceCount = 24;
// 0x0097619C
internal::ThreadStack nw::snd::SoundSystem::s_TaskThreadStack;

// sets up s_TaskThreadStack
// 0x0079A58C (static initializer)

// 0x004BFBEC | mk7dlp:callseq [tier A]
void nw::snd::SoundSystem::Initialize(const SoundSystemParam& param, uptr workMem, u32 workMemSize)
{
    if (s_IsInitialized) {
        return;
    }
    internal::driver::HardwareManager::GetInstance().Initialize();

    uptr p = workMem;
    if (!s_IsInitializedDriverCommandManager) {
        const uptr pTaskThreadCommandBuffer = p + param.soundThreadCommandBufferSize;
        const u32 commandBufferSize = param.soundThreadCommandBufferSize + param.taskThreadCommandBufferSize;
        internal::DriverCommandManager::GetInstance().Initialize(reinterpret_cast<void*>(p), param.soundThreadCommandBufferSize);
        internal::DriverCommandManager::GetInstanceForTaskThread().Initialize(reinterpret_cast<void*>(pTaskThreadCommandBuffer),
                                                                             param.taskThreadCommandBufferSize);
        p += commandBufferSize;
        s_IsInitializedDriverCommandManager = true;
    }

    // the stacks of the sound thread
    uptr soundThreadStack = 0;
    uptr userThreadStack = 0;
    if (param.enableSoundThread) {
        soundThreadStack = p;
        p += param.soundThreadStackSize;
        if (param.soundThreadCoreNo == 1 && param.enableUserThread) {
            userThreadStack = p;
            p += param.soundThreadStackSize;
        }
    }
    internal::driver::SoundThread::GetInstance().Initialize();

    // the task thread
    const uptr taskThreadStack = p;
    p += param.taskThreadStackSize;
    internal::TaskManager::GetInstance().Initialize();
    s_TaskThreadStack.m_pBuffer = reinterpret_cast<void*>(taskThreadStack);
    s_TaskThreadStack.m_Size = param.taskThreadStackSize;

    // the voices and channels
    const uptr channelMem = p + internal::driver::VoiceManager::GetInstance().GetRequiredMemSize(s_VoiceCount);
    internal::driver::VoiceManager::GetInstance().Initialize(
        reinterpret_cast<void*>(p), internal::driver::VoiceManager::GetInstance().GetRequiredMemSize(s_VoiceCount));
    p = channelMem + internal::driver::ChannelManager::GetInstance().GetRequiredMemSize(s_VoiceCount);
    internal::driver::ChannelManager::GetInstance().Initialize(
        reinterpret_cast<void*>(channelMem), internal::driver::ChannelManager::GetInstance().GetRequiredMemSize(s_VoiceCount));

    internal::driver::SequenceSoundPlayer::InitSequenceSoundPlayer();
    internal::TaskThread::GetInstance().Create(param.taskThreadPriority, s_TaskThreadStack);
    if (param.enableSoundThread) {
        internal::driver::SoundThread::GetInstance().CreateSoundThread(soundThreadStack, param.soundThreadStackSize,
                                                                      param.soundThreadPriority, userThreadStack,
                                                                      param.soundThreadStackSize, param.soundThreadPriority,
                                                                      param.soundThreadCoreNo, param.enableGetSoundThreadTick);
    }
    if (param.enableGetSoundThreadTick && param.soundThreadProfileCount > 0) {
        internal::driver::SoundThread::GetInstance().SetProfileBuffer(reinterpret_cast<SoundThreadProfile*>(p),
                                                                     param.soundThreadProfileCount);
    }
    internal::CurveLfo::InitializeCurveTable();
    s_IsEnableSoundThread = param.enableSoundThread;
    s_IsInitialized = true;
}

// 0x004BFE20 (name after the other AppendEffect)
bool nw::snd::SoundSystem::AppendEffect(AuxBus bus, nn::snd::CTR::DspFxDelay* pEffect,
                                        const nn::snd::CTR::DspFxDelay::Param& param)
{
    if (internal::driver::HardwareManager::GetInstance().m_DspEffects[bus][internal::driver::HardwareManager::DSP_EFFECT_DELAY].state !=
        internal::driver::HardwareManager::DspEffect::STATE_NONE) {
        return false;
    }
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffectDspDelay* pCommand = manager.AllocCommand<internal::DriverCommandEffectDspDelay>();
    pCommand->id = internal::DriverCommand::ID_APPEND_EFFECT_DSP_DELAY;
    pCommand->bus = bus;
    pCommand->effect = pEffect;
    pCommand->param = param;
    manager.PushCommand(pCommand);
    return true;
}

// 0x004BFE94 (name after the other AppendEffect)
bool nw::snd::SoundSystem::AppendEffect(AuxBus bus, nn::snd::CTR::DspFxReverb* pEffect,
                                        const nn::snd::CTR::DspFxReverb::Param& param)
{
    if (internal::driver::HardwareManager::GetInstance().m_DspEffects[bus][internal::driver::HardwareManager::DSP_EFFECT_REVERB].state !=
        internal::driver::HardwareManager::DspEffect::STATE_NONE) {
        return false;
    }
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffectDspReverb* pCommand = manager.AllocCommand<internal::DriverCommandEffectDspReverb>();
    pCommand->id = internal::DriverCommand::ID_APPEND_EFFECT_DSP_REVERB;
    pCommand->bus = bus;
    pCommand->effect = pEffect;
    pCommand->param = param;
    // the filter size goes with the command
    if (param.m_FilterSize != nullptr) {
        pCommand->filterSize = *param.m_FilterSize;
        pCommand->param.m_FilterSize = &pCommand->filterSize;
    }
    manager.PushCommand(pCommand);
    return true;
}

// 0x004BFF34 | fefates:bytes [tier B]
bool nw::snd::SoundSystem::AppendEffect(AuxBus bus, nn::snd::CTR::FxDelay* pEffect)
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffectFxDelay* pCommand = manager.AllocCommand<internal::DriverCommandEffectFxDelay>();
    pCommand->id = internal::DriverCommand::ID_APPEND_EFFECT_FX_DELAY;
    pCommand->bus = bus;
    pCommand->effect = pEffect;
    manager.PushCommand(pCommand);
    return true;
}

// 0x004BFF7C | fefates:bytes [tier B]
bool nw::snd::SoundSystem::AppendEffect(AuxBus bus, nn::snd::CTR::FxReverb* pEffect)
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffectFxReverb* pCommand = manager.AllocCommand<internal::DriverCommandEffectFxReverb>();
    pCommand->id = internal::DriverCommand::ID_APPEND_EFFECT_FX_REVERB;
    pCommand->bus = bus;
    pCommand->effect = pEffect;
    manager.PushCommand(pCommand);
    return true;
}

// 0x004BFFC4 (name after the other AppendEffect)
bool nw::snd::SoundSystem::AppendEffect(AuxBus bus, FxBase* pEffect)
{
    if (!pEffect->Initialize()) {
        return false;
    }
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffect* pCommand = manager.AllocCommand<internal::DriverCommandEffect>();
    pCommand->id = internal::DriverCommand::ID_APPEND_EFFECT;
    pCommand->bus = bus;
    pCommand->effect = pEffect;
    manager.PushCommand(pCommand);
    return true;
}

// 0x004C0024 | fefates:callgraph [tier C]
bool nw::snd::SoundSystem::IsInitialized()
{
    return s_IsInitialized;
}

// 0x004C0034 | nintendogs:callseq [tier A]
u32 nw::snd::SoundSystem::GetRequiredMemSize(const SoundSystemParam& param)
{
    const u32 threadMemSize = param.soundThreadCommandBufferSize + param.taskThreadCommandBufferSize + param.taskThreadStackSize;
    const u32 voiceMemSize = internal::driver::VoiceManager::GetInstance().GetRequiredMemSize(s_VoiceCount);
    const u32 channelMemSize = internal::driver::ChannelManager::GetInstance().GetRequiredMemSize(s_VoiceCount);
    u32 size = channelMemSize + voiceMemSize + threadMemSize;
    if (param.enableSoundThread == true) {
        size += param.soundThreadStackSize;
    }
    if (param.soundThreadCoreNo == 1 && param.enableUserThread) {
        size += param.soundThreadStackSize;
    }
    if (param.enableGetSoundThreadTick && param.soundThreadProfileCount > 0) {
        size += param.soundThreadProfileCount * sizeof(SoundThreadProfile);
    }
    return size;
}

// 0x004C00C4 (name is ours)
void nw::snd::SoundSystem::InitializeDriverCommandManager(const SoundSystemParam& param, uptr workMem, u32 /*workMemSize*/)
{
    if (s_IsInitializedDriverCommandManager) {
        return;
    }
    const uptr pTaskThreadCommandBuffer = workMem + param.soundThreadCommandBufferSize;
    internal::DriverCommandManager::GetInstance().Initialize(reinterpret_cast<void*>(workMem), param.soundThreadCommandBufferSize);
    internal::DriverCommandManager::GetInstanceForTaskThread().Initialize(reinterpret_cast<void*>(pTaskThreadCommandBuffer),
                                                                         param.taskThreadCommandBufferSize);
    s_IsInitializedDriverCommandManager = true;
}

// 0x004C0118 (name is ours)
u32 nw::snd::SoundSystem::GetRequiredDriverCommandManagerMemSize(const SoundSystemParam& param)
{
    return param.taskThreadCommandBufferSize + param.soundThreadCommandBufferSize;
}

// 0x004C0128 | nintendogs:callseq [tier A]
void nw::snd::SoundSystem::Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    internal::TaskThread::GetInstance().Destroy();
    internal::TaskManager::GetInstance().Finalize();
    internal::driver::SoundThread::GetInstance().Destroy();
    internal::driver::SoundThread::GetInstance().ClearProfileBuffer();
    internal::driver::ChannelManager::GetInstance().Finalize();
    internal::driver::VoiceManager::GetInstance().Finalize();
    internal::driver::HardwareManager::GetInstance().Finalize();
    s_TaskThreadStack.Finalize();
    internal::driver::SoundThread::GetInstance().Finalize();
    internal::DriverCommandManager::GetInstance().Finalize();
    internal::DriverCommandManager::GetInstanceForTaskThread().Finalize();
    s_IsInitializedDriverCommandManager = false;
    s_IsInitialized = false;
}

// 0x001417FC | nintendogs:callgraph [tier A]
void nw::snd::SoundSystem::ClearEffect(AuxBus bus, int fadeTimes)
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandEffectClear* pCommand = manager.AllocCommand<internal::DriverCommandEffectClear>();
    pCommand->id = internal::DriverCommand::ID_CLEAR_EFFECT;
    pCommand->bus = bus;
    pCommand->fadeTimes = fadeTimes;
    manager.PushCommand(pCommand);
    manager.WaitCommandReply(manager.FlushCommand(true));
}

} // namespace snd
} // namespace nw
