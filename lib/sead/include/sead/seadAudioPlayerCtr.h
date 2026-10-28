#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchivePlayer.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioPlayer.h"
#include "sead/seadCriticalSection.h"

namespace sead {
class AudioSoundDataMgrCtr;
class AudioSoundHeapCtr;

// RTTI N4sead14AudioPlayerCtrE @ 0x008D1690
// vtable 0x00905678 (vptr 0x00905680), offset_to_top 0, 15 entries
// vtable 0x009056BC (vptr 0x009056C4), offset_to_top -4, 4 entries
// vtable 0x009056D4 (vptr 0x009056DC), offset_to_top -284, 1 entries
class AudioPlayerCtr : public ::sead::AudioPlayer, public ::nw::snd::SoundArchivePlayer, public ::sead::hostio::Node
{
public:
    AudioPlayerCtr(); // ctor candidate(s) 0x0012C814 (unverified)
    virtual void vf_0x00(); // 0x0074BB34 slot 0x00 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x04(); // 0x0074BAE8 slot 0x04 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual ~AudioPlayerCtr(); // 0x00543978 slot 0x08
    virtual void vf_0x10(); // 0x00543294 slot 0x10 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x14(); // 0x00543854 slot 0x14 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x18(); // 0x005437B8 slot 0x18 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x1C(); // 0x005432B8 slot 0x1C | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x20(); // 0x00543298 slot 0x20 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x24(); // 0x00543940 slot 0x24 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x28(); // 0x00543920 slot 0x28 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x2C(); // 0x0074BAB8 slot 0x2C | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x30(); // 0x0074BA80 slot 0x30 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x34(); // 0x0074BA44 slot 0x34 | virtual slot, introduced by sead::AudioPlayerCtr
    virtual void vf_0x38(); // 0x00543458 slot 0x38 | virtual slot, introduced by sead::AudioPlayerCtr

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
};
ASSERT_OFFSET(AudioPlayerCtr, m_pSetupBuffer, 0x120);
ASSERT_OFFSET(AudioPlayerCtr, m_CriticalSection, 0x150);
} // namespace sead
