#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioPlayer.h"
#include "sead/hostio/seadNode.h"
#include "nw/snd/snd_SoundArchivePlayer.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadNew.h"
#include "sead/seadAudioSoundDataMgrCtr.h"
#include "sead/seadAudioSoundHeapCtr.h"

namespace sead {
// ctor candidate(s) 0x0012C814 (unverified)
sead::AudioPlayerCtr::AudioPlayerCtr()
{
}

// 0x00543978 slot 0x08
// 0x00543960 (deleting dtor)
// 0x00543970 (thunk)
// 0x00778358 (deleting thunk)
sead::AudioPlayerCtr::~AudioPlayerCtr()
{
    if (m_pSetupBuffer != nullptr) {
        delete[] m_pSetupBuffer;
        m_pSetupBuffer = nullptr;
    }
    if (m_pStreamBuffer != nullptr) {
        delete[] m_pStreamBuffer;
        m_pStreamBuffer = nullptr;
    }
    if (m_pStreamCacheBuffer != nullptr) {
        delete[] m_pStreamCacheBuffer;
        m_pStreamCacheBuffer = nullptr;
    }
    if (m_pSoundHeap != nullptr) {
        delete m_pSoundHeap;
        m_pSoundHeap = nullptr;
    }
    if (m_pSoundDataMgr != nullptr) {
        delete m_pSoundDataMgr;
        m_pSoundDataMgr = nullptr;
    }
}

// 0x00543294 slot 0x10 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x10()
{
}

// 0x00543854 slot 0x14 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x14()
{
}

// 0x005437B8 slot 0x18 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x18()
{
}

// 0x005432B8 slot 0x1C | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x1C()
{
}

// 0x00543298 slot 0x20 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x20()
{
}

// 0x00543940 slot 0x24 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x24()
{
}

// 0x00543920 slot 0x28 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x28()
{
}

// 0x0074BAB8 slot 0x2C | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x2C()
{
}

// 0x0074BA80 slot 0x30 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x30()
{
}

// 0x0074BA44 slot 0x34 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x34()
{
}

// 0x00543458 slot 0x38 | virtual slot, introduced by sead::AudioPlayerCtr
void sead::AudioPlayerCtr::vf_0x38()
{
}

} // namespace sead

// the type information of the classes below AudioPlayer
// 0x008289FC (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioPlayer>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
