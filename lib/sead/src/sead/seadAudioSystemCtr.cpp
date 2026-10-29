#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/os/os_Atomic.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/snd_SoundSystem.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioFxCtr.h"
#include "sead/seadAudioFxHolder.h"
#include "sead/seadAudioFxObject.h"
#include "sead/seadAudioSystem.h"
#include "sead/seadAudioSystemCtr.h"
#include "sead/seadNew.h"

namespace sead {
namespace {
// the bus of nw::snd (B for AUX_BUS_B, else A)
inline nw::snd::AuxBus toNwAuxBus(AudioGlobal::AuxBus bus)
{
    switch (bus) {
    case AudioGlobal::AUX_BUS_A:
        return nw::snd::AUX_BUS_A;
    case AudioGlobal::AUX_BUS_B:
        return nw::snd::AUX_BUS_B;
    default:
        return nw::snd::AUX_BUS_A;
    }
}
} // namespace

// 0x0012C8A0 (name is ours)
sead::AudioSystemCtr::AudioSystemCtr()
    : AudioSystem(), hostio::Node(), mSoundThreadPriority(4), mTaskThreadPriority(3), mSoundThreadStackSize(16384),
      mTaskThreadStackSize(16384), mHeap(nullptr), mWorkMem(nullptr), mWorkMemSize(0), mSoundThreadCoreNo(0),
      mEnableSoundThread(true), mEnableGetSoundThreadTick(false), mSoundThreadProfileCount(-1),
      mEnableUserThread(true), mParam(), mIsInitialized(false), mDriverCommandMem(nullptr),
      mIsSplitInitialize(false), mIsSoundSystemInitialized(false), mFrameCallbacks(), mCriticalSection()
{
    mFrameCallbacks.initOffset(offsetof(FrameCallback, mNode));
}

// 0x00543A30 slot 0x10 (name is ours)
void sead::AudioSystemCtr::initialize()
{
    mParam.soundThreadPriority = mSoundThreadPriority;
    mParam.soundThreadCoreNo = mSoundThreadCoreNo;
    mParam.soundThreadStackSize = mSoundThreadStackSize;
    mParam.taskThreadPriority = mTaskThreadPriority;
    mParam.taskThreadStackSize = mTaskThreadStackSize;
    mParam.enableSoundThread = mEnableSoundThread;
    mParam.enableGetSoundThreadTick = mEnableGetSoundThreadTick;
    mParam.enableUserThread = mEnableUserThread;
    mParam.soundThreadProfileCount = mSoundThreadProfileCount;
    mIsInitialized = true;
    if (mIsSplitInitialize) {
        Heap* heap = mHeap;
        u32 commandMemSize = nw::snd::SoundSystem::GetRequiredDriverCommandManagerMemSize(mParam);
        mDriverCommandMem = new (heap, 32) u8[commandMemSize];
        nw::snd::SoundSystem::InitializeDriverCommandManager(mParam, reinterpret_cast<uptr>(mDriverCommandMem),
                                                             commandMemSize);
        // (the command buffers are in mDriverCommandMem)
        nw::snd::SoundSystem::SoundSystemParam param = mParam;
        param.soundThreadCommandBufferSize = 0;
        param.taskThreadCommandBufferSize = 0;
        mWorkMemSize = nw::snd::SoundSystem::GetRequiredMemSize(param);
        mWorkMem = new (mHeap, 32) u8[mWorkMemSize];
        return;
    }
    initializeDsp_();
    mWorkMemSize = nw::snd::SoundSystem::GetRequiredMemSize(mParam);
    mWorkMem = new (mHeap, 32) u8[mWorkMemSize];
    nw::snd::SoundSystem::Initialize(mParam, reinterpret_cast<uptr>(mWorkMem), mWorkMemSize);
    mIsSoundSystemInitialized = true;
}

// 0x00543B4C slot 0x28 | nintendogs:bytes
void sead::AudioSystemCtr::clearEffect(AudioGlobal::AuxBus bus, int fadeFrames)
{
    nw::snd::SoundSystem::ClearEffect(toNwAuxBus(bus), fadeFrames);
}

// 0x00543B68 slot 0x20
bool sead::AudioSystemCtr::appendEffect(AudioGlobal::AuxBus bus, AudioFx* fx)
{
    AudioFxCtr* fxCtr = DynamicCast<AudioFxCtr>(fx);
    nw::snd::AuxBus nwBus = toNwAuxBus(bus);
    void* effect = fx->mHolder->getFx();
    switch (fxCtr->mType) {
    case AudioFxCtr::TYPE_FX_REVERB:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nn::snd::CTR::FxReverb*>(effect));
    case AudioFxCtr::TYPE_FX_DELAY:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nn::snd::CTR::FxDelay*>(effect));
    case AudioFxCtr::TYPE_DSP_FX_REVERB:
        return nw::snd::SoundSystem::AppendEffect(
            nwBus, static_cast<nn::snd::CTR::DspFxReverb*>(effect),
            *static_cast<const nn::snd::CTR::DspFxReverb::Param*>(fxCtr->mDspParam));
    case AudioFxCtr::TYPE_DSP_FX_DELAY:
        return nw::snd::SoundSystem::AppendEffect(
            nwBus, static_cast<nn::snd::CTR::DspFxDelay*>(effect),
            *static_cast<const nn::snd::CTR::DspFxDelay::Param*>(fxCtr->mDspParam));
    default:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nw::snd::FxBase*>(effect));
    }
}

// 0x00543C80 slot 0x18 (name is ours)
bool sead::AudioSystemCtr::setOutputMode(AudioGlobal::OutputMode mode)
{
    nw::snd::OutputMode nwMode;
    switch (mode) {
    case AudioGlobal::OUTPUT_MODE_STEREO:
        nwMode = nw::snd::OUTPUT_MODE_STEREO;
        break;
    case AudioGlobal::OUTPUT_MODE_MONO:
        nwMode = nw::snd::OUTPUT_MODE_MONO;
        break;
    case AudioGlobal::OUTPUT_MODE_SURROUND:
        nwMode = nw::snd::OUTPUT_MODE_SURROUND;
        break;
    default:
        return false;
    }
    nw::snd::internal::driver::HardwareManager::GetInstance().SetOutputMode(nwMode);
    return true;
}

// 0x00543CC0 slot 0x24
bool sead::AudioSystemCtr::appendEffect(AudioGlobal::AuxBus bus, AudioFxObject* fx)
{
    nw::snd::AuxBus nwBus = toNwAuxBus(bus);
    u8 type = DynamicCast<AudioFxObjectCtr>(fx)->mType;
    void* effect = fx->mHolder->getFx();
    switch (type) {
    case AudioFxCtr::TYPE_FX_REVERB:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nn::snd::CTR::FxReverb*>(effect));
    case AudioFxCtr::TYPE_FX_DELAY:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nn::snd::CTR::FxDelay*>(effect));
    case AudioFxCtr::TYPE_DSP_FX_REVERB:
        return nw::snd::SoundSystem::AppendEffect(
            nwBus, static_cast<nn::snd::CTR::DspFxReverb*>(effect),
            *static_cast<const nn::snd::CTR::DspFxReverb::Param*>(fx->mHolder->getParam()));
    case AudioFxCtr::TYPE_DSP_FX_DELAY:
        return nw::snd::SoundSystem::AppendEffect(
            nwBus, static_cast<nn::snd::CTR::DspFxDelay*>(effect),
            *static_cast<const nn::snd::CTR::DspFxDelay::Param*>(fx->mHolder->getParam()));
    default:
        return nw::snd::SoundSystem::AppendEffect(nwBus, static_cast<nw::snd::FxBase*>(effect));
    }
}

// 0x00543DFC (name is ours)
void sead::AudioSystemCtr::initializeSoundSystem_()
{
    initializeDsp_();
    nw::snd::SoundSystem::SoundSystemParam param = mParam;
    param.soundThreadCommandBufferSize = 0;
    param.taskThreadCommandBufferSize = 0;
    nw::snd::SoundSystem::Initialize(param, reinterpret_cast<uptr>(mWorkMem), mWorkMemSize);
    mIsSoundSystemInitialized = true;
}

// 0x00543E58 (name is ours)
void sead::AudioSystemCtr::soundFrameCallback_(uptr arg)
{
    AudioSystemCtr* system = reinterpret_cast<AudioSystemCtr*>(arg);
    system->mCriticalSection.lock();
    if (system->mFrameCallbacks.mCount != 0) {
        for (FrameCallback* callback = system->mFrameCallbacks.front(); callback != system->mFrameCallbacks.endObj();
             callback = system->mFrameCallbacks.next(callback)) {
            callback->invoke();
        }
    }
    system->mCriticalSection.unlock();
}

// 0x00543EC0 slot 0x2C (name is ours)
bool sead::AudioSystemCtr::isEffectFinalized(AudioGlobal::AuxBus bus)
{
    return nw::snd::internal::driver::HardwareManager::GetInstance().IsEffectFinalized(toNwAuxBus(bus));
}

// 0x00543EE0 slot 0x38 (name is ours)
void sead::AudioSystemCtr::clearFrameCallbacks()
{
    mCriticalSection.lock();
    mFrameCallbacks.clear();
    mCriticalSection.unlock();
    nw::snd::internal::driver::SoundThread& thread = nw::snd::internal::driver::SoundThread::GetInstance();
    thread.m_pUserCallback = nullptr;
    nn::os::detail::DataMemoryBarrier();
    thread.m_UserCallbackArg = 0;
}

// 0x00543F1C slot 0x30 (name is ours)
void sead::AudioSystemCtr::addFrameCallback(FrameCallback* callback)
{
    if (mFrameCallbacks.mCount == 0) {
        nw::snd::internal::driver::SoundThread& thread = nw::snd::internal::driver::SoundThread::GetInstance();
        thread.m_UserCallbackArg = reinterpret_cast<uptr>(this);
        nn::os::detail::DataMemoryBarrier();
        thread.m_pUserCallback = &soundFrameCallback_;
    }
    mCriticalSection.lock();
    if (mFrameCallbacks.indexOf(mFrameCallbacks.objToListNode(callback)) == -1) {
        mFrameCallbacks.pushBack(callback);
    }
    mCriticalSection.unlock();
}

// 0x00543FA4 slot 0x34 (name is ours)
void sead::AudioSystemCtr::removeFrameCallback(FrameCallback* callback)
{
    mCriticalSection.lock();
    if (mFrameCallbacks.indexOf(mFrameCallbacks.objToListNode(callback)) >= 0) {
        mFrameCallbacks.erase(callback);
    }
    mCriticalSection.unlock();
    if (mFrameCallbacks.mCount == 0) {
        nw::snd::internal::driver::SoundThread& thread = nw::snd::internal::driver::SoundThread::GetInstance();
        thread.m_pUserCallback = nullptr;
        nn::os::detail::DataMemoryBarrier();
        thread.m_UserCallbackArg = 0;
    }
}

// 0x00544018 slot 0x14 (name is ours)
void sead::AudioSystemCtr::finalize()
{
    nw::snd::SoundSystem::Finalize();
    finalizeDsp_();
    if (mWorkMem != nullptr) {
        delete[] static_cast<u8*>(mWorkMem);
        mWorkMem = nullptr;
    }
    if (mDriverCommandMem != nullptr) {
        delete[] static_cast<u8*>(mDriverCommandMem);
        mDriverCommandMem = nullptr;
    }
    mIsInitialized = false;
    mIsSoundSystemInitialized = false;
}

// 0x00544094 slot 0x08
// 0x0054406C (deleting dtor)
sead::AudioSystemCtr::~AudioSystemCtr()
{
    // (only the members are destroyed)
}

// 0x00351A3C slot 0x40
void sead::AudioSystemCtr::finalizeDsp_()
{
    nn::snd::CTR::Finalize();
    nn::dsp::CTR::UnloadComponent();
    nn::dsp::CTR::Finalize();
}

// 0x00463584 slot 0x3C | nintendogs:bytes
void sead::AudioSystemCtr::initializeDsp_()
{
    nn::dsp::CTR::Initialize();
    nn::dsp::CTR::LoadDefaultComponent();
    nn::snd::CTR::Initialize();
    nn::snd::CTR::SetOutputBufferCount(3);
}

// 0x0074BBE8 slot 0x1C (name is ours)
sead::AudioGlobal::OutputMode sead::AudioSystemCtr::getOutputMode() const
{
    switch (nw::snd::internal::driver::HardwareManager::GetInstance().GetOutputMode()) {
    case nw::snd::OUTPUT_MODE_MONO:
        return AudioGlobal::OUTPUT_MODE_MONO;
    case nw::snd::OUTPUT_MODE_STEREO:
        return AudioGlobal::OUTPUT_MODE_STEREO;
    case nw::snd::OUTPUT_MODE_SURROUND:
        return AudioGlobal::OUTPUT_MODE_SURROUND;
    default:
        return AudioGlobal::OUTPUT_MODE_INVALID;
    }
}
} // namespace sead

// the type information of the classes below AudioSystem
// 0x00828A70 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioSystem>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
// the type information of the classes below AudioFxObject
// 0x00828B58 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioFxObject>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
