#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadListImpl.h"

namespace sead {
class AudioPlayer;
class AudioResetter;
class AudioResourceLoader;
class AudioSettingParameter;
class AudioSystem;
class Heap;

// RTTI N4sead8AudioMgrE @ 0x008D217C
// vtable 0x00906CB0 (vptr 0x00906CB8), offset_to_top 0, 3 entries
// The audio of the program (a singleton): the sound system, the player, the resetter, the resource
// loader and the subsets of the program. What the setting parameter does not give, it makes itself
// and deletes again. Member and function names are ours.
class AudioMgr : public ::sead::hostio::Node
{
public:
    class SingletonDisposer_;

    AudioMgr()
        : hostio::Node(), mSystem(nullptr), mResetter(nullptr), mPlayer(nullptr), mResourceLoader(nullptr), mSubsets(),
          mHeap(nullptr), mIsInitialized(false), mIsSystemOwner(false), mIsResetterOwner(false), mIsPlayerOwner(false)
    {
        mSubsets.initOffset(offsetof(AudioSubsetBase, mNode));
    }
    virtual ~AudioMgr(); // 0x00560C0C slot 0x04
    // 0x00560B78 slot 0x08 (deleting dtor)

    static AudioMgr* createInstance(Heap* heap); // 0x00125F10
    static void deleteInstance(); // 0x0013C80C
    void initialize(AudioSettingParameter* param, Heap* heap); // 0x00125FC4
    void finalize(); // 0x0013C858
    void calc(); // 0x00560AE8

    static AudioMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4];       // 0x04, the SingletonDisposer_
    AudioSystem* mSystem;                  // 0x14
    AudioResetter* mResetter;              // 0x18
    AudioPlayer* mPlayer;                  // 0x1C
    AudioResourceLoader* mResourceLoader;  // 0x20
    OffsetList<AudioSubsetBase> mSubsets;  // 0x24
    Heap* mHeap;                           // 0x34
    bool mIsInitialized;                   // 0x38
    bool mIsSystemOwner;                   // 0x39
    bool mIsResetterOwner;                 // 0x3A
    bool mIsPlayerOwner;                   // 0x3B
};
ASSERT_SIZE(AudioMgr, 0x3C);
} // namespace sead
