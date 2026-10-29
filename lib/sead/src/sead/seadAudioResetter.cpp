#include "sead/seadAudioMgr.h"
#include "sead/seadAudioResetter.h"
#include "sead/seadAudioSubsetBase.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x00132F80 (name is ours)
DECOMP_NOINLINE sead::AudioResetter::AudioResetter() : mMgr(nullptr)
{
}

// 0x00541780 slot 0x08 (name is ours)
void sead::AudioResetter::initialize(AudioMgr* mgr)
{
    mMgr = mgr;
}

// 0x00541788 slot 0x1C (name is ours)
void sead::AudioResetter::endReset()
{
    if (mMgr->mSubsets.mCount == 0) {
        return;
    }
    for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
         subset = mMgr->mSubsets.next(subset)) {
        subset->endReset();
    }
}

// 0x005417E8 slot 0x10 (name is ours)
void sead::AudioResetter::startReset(int fadeFrames)
{
    if (mMgr->mSubsets.mCount == 0) {
        return;
    }
    for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
         subset = mMgr->mSubsets.next(subset)) {
        subset->startReset(fadeFrames);
    }
}

// 0x00541850 slot 0x20 (name is ours)
void sead::AudioResetter::startStop(int fadeFrames)
{
    if (mMgr->mSubsets.mCount == 0) {
        return;
    }
    for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
         subset = mMgr->mSubsets.next(subset)) {
        subset->startStop(fadeFrames);
    }
}

// 0x005418BC slot 0x00
// 0x005418B8 (deleting dtor)
sead::AudioResetter::~AudioResetter()
{
    // empty (in the original too)
}

// 0x0074B0F8 slot 0x18 (name is ours)
bool sead::AudioResetter::isResetReady()
{
    if (mMgr->mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
             subset = mMgr->mSubsets.next(subset)) {
            if (!subset->isResetReady()) {
                return false;
            }
        }
    }
    return true;
}

// 0x0074B164 slot 0x14 | nintendogs
bool sead::AudioResetter::isResetting() const
{
    if (mMgr->mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
             subset = mMgr->mSubsets.next(subset)) {
            if (subset->isResetting()) {
                return true;
            }
        }
    }
    return false;
}

// 0x0074B1D8 slot 0x28 (name is ours)
bool sead::AudioResetter::isStopReady()
{
    if (mMgr->mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
             subset = mMgr->mSubsets.next(subset)) {
            if (!subset->isStopReady()) {
                return false;
            }
        }
    }
    return true;
}

// 0x0074B244 slot 0x24 (name is ours)
bool sead::AudioResetter::isStopping() const
{
    if (mMgr->mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mMgr->mSubsets.front(); subset != mMgr->mSubsets.endObj();
             subset = mMgr->mSubsets.next(subset)) {
            if (subset->isStopping()) {
                return true;
            }
        }
    }
    return false;
}
} // namespace sead
