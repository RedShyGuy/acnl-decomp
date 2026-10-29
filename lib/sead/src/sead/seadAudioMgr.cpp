#include "sead/hostio/seadNode.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioMgr_SingletonDisposer_.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioResetterCtr.h"
#include "sead/seadAudioResourceLoader.h"
#include "sead/seadAudioSettingParameter.h"
#include "sead/seadAudioSystemCtr.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"

#include <new>

namespace sead {
// 0x009762D8
AudioMgr* AudioMgr::s_pInstance = nullptr;
// 0x009762DC
AudioMgr::SingletonDisposer_* AudioMgr::s_pSingletonDisposer = nullptr;

// 0x00125F10 (name is ours)
sead::AudioMgr* sead::AudioMgr::createInstance(Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(AudioMgr)];
        s_pSingletonDisposer = new (buffer + offsetof(AudioMgr, mSingletonDisposerBuffer)) SingletonDisposer_(heap);
        s_pInstance = new (buffer) AudioMgr();
    }
    return s_pInstance;
}

// 0x00125FC4 (name is ours)
void sead::AudioMgr::initialize(AudioSettingParameter* param, Heap* heap)
{
    if (heap != nullptr) {
        mHeap = heap;
    } else {
        mHeap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    if (param != nullptr) {
        mSystem = param->mSystem;
        mResetter = param->mResetter;
        mPlayer = param->mPlayer;
        mResourceLoader = param->mResourceLoader;
        while (param->mSubsets.mCount != 0) {
            ListNode* node = param->mSubsets.popFront();
            mSubsets.pushBack(node != nullptr ? param->mSubsets.listNodeToObj(node) : nullptr);
        }
    }
    if (mSystem == nullptr) {
        mSystem = new (heap, 4) AudioSystemCtr();
        mIsSystemOwner = true;
    }
    if (mPlayer == nullptr) {
        mPlayer = new (heap, 4) AudioPlayerCtr();
        mIsPlayerOwner = true;
    }
    if (mResetter == nullptr) {
        mResetter = new (heap, 4) AudioResetterCtr();
        mIsResetterOwner = true;
    }
    if (mSystem != nullptr) {
        mSystem->initialize();
    }
    if (mPlayer != nullptr) {
        mPlayer->initialize();
    }
    if (mResetter != nullptr) {
        mResetter->initialize(this);
    }
    if (mResourceLoader != nullptr) {
        mResourceLoader->initialize(this);
    }
    if (mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mSubsets.front(); subset != mSubsets.endObj(); subset = mSubsets.next(subset)) {
            subset->initialize(this, heap);
        }
    }
    if (mResourceLoader != nullptr) {
        mResourceLoader->setupPlayer();
    }
    mIsInitialized = true;
}

// 0x0013C80C (name is ours)
void sead::AudioMgr::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == nullptr) {
        return;
    }
    s_pSingletonDisposer = nullptr;
    disposer->~SingletonDisposer_();
    delete s_pInstance;
    s_pInstance = nullptr;
}

// 0x0013C858 (name is ours)
void sead::AudioMgr::finalize()
{
    if (!mIsInitialized) {
        return;
    }
    if (mPlayer != nullptr) {
        mPlayer->finalize();
    }
    if (mResourceLoader != nullptr) {
        mResourceLoader->finalize();
    }
    if (mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mSubsets.front(); subset != mSubsets.endObj(); subset = mSubsets.next(subset)) {
            subset->finalize();
        }
    }
    if (mSystem != nullptr) {
        mSystem->finalize();
    }
    mIsInitialized = false;
}

// 0x00560AE8 (name is ours)
void sead::AudioMgr::calc()
{
    if (!mIsInitialized) {
        return;
    }
    if (mPlayer != nullptr) {
        mPlayer->calc();
    }
    if (mResetter != nullptr) {
        mResetter->calc();
    }
    if (mSubsets.mCount != 0) {
        for (AudioSubsetBase* subset = mSubsets.front(); subset != mSubsets.endObj(); subset = mSubsets.next(subset)) {
            subset->calc();
        }
    }
}

// 0x00560C0C slot 0x04
// 0x00560B78 (deleting dtor)
sead::AudioMgr::~AudioMgr()
{
    finalize();
    if (mIsSystemOwner && mSystem != nullptr) {
        delete mSystem;
        mSystem = nullptr;
    }
    if (mIsPlayerOwner && mPlayer != nullptr) {
        delete mPlayer;
        mPlayer = nullptr;
    }
    if (mIsResetterOwner && mResetter != nullptr) {
        delete mResetter;
        mResetter = nullptr;
    }
}
} // namespace sead
