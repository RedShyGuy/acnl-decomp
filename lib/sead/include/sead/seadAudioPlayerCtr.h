#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "nw/snd/snd_SoundArchivePlayer.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioPlayer.h"
#include "sead/seadCriticalSection.h"

namespace sead {
class AudioSoundDataMgrCtr;
class AudioSoundHeapCtr;
class Heap;

// RTTI N4sead14AudioPlayerCtrE @ 0x008D1690
// vtable 0x00905678 (vptr 0x00905680), offset_to_top 0, 15 entries
// vtable 0x009056BC (vptr 0x009056C4), offset_to_top -4, 4 entries
// vtable 0x009056D4 (vptr 0x009056DC), offset_to_top -284, 1 entries
class AudioPlayerCtr : public ::sead::AudioPlayer, public ::nw::snd::SoundArchivePlayer, public ::sead::hostio::Node
{
public:
    AudioPlayerCtr(); // 0x0012C814
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BB34 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioPlayer::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BAE8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983454 (guard 0x00983450)
        static const RuntimeTypeInfo::Derive<AudioPlayer> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioPlayerCtr(); // 0x00543978 slot 0x08
    virtual void initialize(); // 0x00543294 slot 0x10
    // stops the sounds and frees the buffers, the sound heap and the data of the archive
    virtual void finalize(); // 0x00543854 slot 0x14
    // updates the archive player (under the lock with m_IsLockUpdate)
    virtual void calc(); // 0x005437B8 slot 0x18
    virtual bool startSound(nw::snd::SoundHandle* handle, u32 soundId); // 0x005432B8 slot 0x1C
    virtual bool startSound(nw::snd::SoundHandle* handle, const char* label); // 0x00543298 slot 0x20
    virtual bool holdSound(nw::snd::SoundHandle* handle, u32 soundId); // 0x00543940 slot 0x24
    virtual bool holdSound(nw::snd::SoundHandle* handle, const char* label); // 0x00543920 slot 0x28
    virtual u32 getSoundCount() const; // 0x0074BAB8 slot 0x2C
    virtual const char* getSoundLabel(u32 soundId) const; // 0x0074BA80 slot 0x30
    // (-1 without an archive)
    virtual u32 getSoundId(const char* label) const; // 0x0074BA44 slot 0x34
    // no new sounds while the audio is reset
    virtual StartResult detail_SetupSound(nw::snd::SoundHandle* pHandle, u32 soundId, bool isHold,
                                          const StartInfo* pStartInfo); // 0x00543458 slot 0x38

    // the arguments of setupBuffers_ (names are ours)
    struct SetupArg
    {
        f32 streamBufferScale;    // 0x0, of the required stream buffer size
        u32 streamCacheSize;      // 0x4
        u32 userParamSizePerSound; // 0x8
        Heap* heap;               // 0xC
    };

    // resumes the sounds of all players (names of these helpers are ours)
    void unpauseAll(int fadeFrames); // 0x005432D8
    // the sound archive player once the split initialization of the sound system is done
    void initializeAfterSoundSystem(); // 0x0054332C
    void setupBuffers(const SetupArg& arg); // 0x005434C8
    void setupBuffers(u32 extraStreamBufferSize, u32 streamCacheSize, u32 userParamSizePerSound, Heap* heap); // 0x00543534
    void setupBuffers_(nw::snd::SoundArchive* archive, u32 streamBufferSize, u32 streamCacheSize,
                       u32 userParamSizePerSound, Heap* heap); // 0x0054359C
    void stopAllSound(int fadeFrames); // 0x00543808
    // the sound heap for the data of the archive
    void createSoundHeap(u32 size, Heap* heap); // 0x0054A20C

    // (the members are ours; the buffers are the ones of SoundArchivePlayer::Initialize)
    u8* m_pSetupBuffer;                     // 0x120
    u32 m_SetupBufferSize;                  // 0x124
    u8* m_pStreamBuffer;                    // 0x128
    u32 m_StreamBufferSize;                 // 0x12C
    u32 m_RequiredStreamBufferSize;         // 0x130
    u32 m_Unknown134;                       // 0x134
    u8* m_pStreamCacheBuffer;               // 0x138
    u32 m_StreamCacheSize;                  // 0x13C
    u32 m_UserParamSizePerSound;            // 0x140
    AudioSoundDataMgrCtr* m_pSoundDataMgr;  // 0x144
    AudioSoundHeapCtr* m_pSoundHeap;        // 0x148
    bool m_IsPause;                         // 0x14C
    CriticalSection m_CriticalSection;      // 0x150
    bool m_IsLockUpdate;                    // 0x16C
};
ASSERT_OFFSET(AudioPlayerCtr, m_pSetupBuffer, 0x120);
ASSERT_OFFSET(AudioPlayerCtr, m_CriticalSection, 0x150);
} // namespace sead
