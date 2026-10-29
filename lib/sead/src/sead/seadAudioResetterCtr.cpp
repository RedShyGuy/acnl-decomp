#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioResetterCtr.h"

namespace sead {
namespace {
// the master volume of nw::snd now
inline f32 getMasterVolume()
{
    return nw::snd::internal::driver::HardwareManager::GetInstance().m_MasterVolume.GetValue();
}
} // namespace

// 0x0012CAA4
sead::AudioResetterCtr::AudioResetterCtr()
    : AudioResetter(), mResetState(STATE_NONE), mStopState(STATE_NONE), mMasterVolume(1.0f)
{
}

// 0x0054177C slot 0x08 (name is ours)
void sead::AudioResetterCtr::initialize(AudioMgr* mgr)
{
    AudioResetter::initialize(mgr);
}

// 0x0054662C slot 0x1C (name is ours)
void sead::AudioResetterCtr::endReset()
{
    if (mStopState != STATE_NONE) {
        return;
    }
    nw::snd::internal::driver::HardwareManager::GetInstance().SetMasterVolume(mMasterVolume, 0);
    AudioResetter::endReset();
    DynamicCast<AudioPlayerCtr>(mMgr->mPlayer)->unpauseAll(0);
    mResetState = STATE_NONE;
}

// 0x005466E4 slot 0x0C (name is ours)
void sead::AudioResetterCtr::calc()
{
    f32 volume = getMasterVolume();
    if (mStopState == STATE_FADE) {
        if (volume == 0.0f) {
            mStopState = STATE_DONE;
            return;
        }
    } else if (mStopState == STATE_DONE) {
        return;
    }
    if (mResetState == STATE_FADE && volume == 0.0f) {
        DynamicCast<AudioPlayerCtr>(mMgr->mPlayer)->stopAllSound(0);
        mResetState = STATE_DONE;
    }
}

// 0x00546804 slot 0x10 (name is ours)
void sead::AudioResetterCtr::startReset(int fadeFrames)
{
    if (mStopState != STATE_NONE || mResetState != STATE_NONE) {
        return;
    }
    mMasterVolume = getMasterVolume();
    nw::snd::internal::driver::HardwareManager::GetInstance().SetMasterVolume(0.0f, fadeFrames);
    AudioResetter::startReset(fadeFrames);
    mResetState = STATE_FADE;
}

// 0x0054689C slot 0x20 (name is ours)
void sead::AudioResetterCtr::startStop(int fadeFrames)
{
    if (mStopState != STATE_NONE) {
        return;
    }
    nw::snd::internal::driver::HardwareManager::GetInstance().SetMasterVolume(0.0f, fadeFrames);
    AudioResetter::startStop(fadeFrames);
    mStopState = STATE_FADE;
}

// 0x005468E4 slot 0x00
// 0x005468E0 (deleting dtor)
sead::AudioResetterCtr::~AudioResetterCtr()
{
    // empty (in the original too)
}

// 0x0074C780 slot 0x18 (name is ours)
bool sead::AudioResetterCtr::isResetReady()
{
    return AudioResetter::isResetReady() && mResetState == STATE_DONE;
}

// 0x0074C7AC slot 0x14
bool sead::AudioResetterCtr::isResetting() const
{
    return mResetState != STATE_NONE || AudioResetter::isResetting();
}

// 0x0074C7D0 slot 0x28 (name is ours)
bool sead::AudioResetterCtr::isStopReady()
{
    return AudioResetter::isStopReady() && mStopState == STATE_DONE;
}

// 0x0074C7FC slot 0x24 (name is ours)
bool sead::AudioResetterCtr::isStopping() const
{
    return mStopState != STATE_NONE || AudioResetter::isStopping();
}
} // namespace sead
