#include "sead/seadAudioFxSet.h"
#include "sead/seadAudioFxHolder.h"
#include "sead/seadAudioFxMemoryMgrCtr.h"
#include "sead/seadAudioFxMgr.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioSoundHeapCtr.h"

namespace sead {
// 0x00549328 (name is ours)
void sead::AudioFxSet::reset(s32 fadeFrames)
{
    field_0x28 = false;
    for (s32 bus = 0; bus < 2; bus++) {
        if (mNum[bus] > 0) {
            mFxMgr->clearBus(static_cast<AudioGlobal::AuxBus>(bus), fadeFrames);
        }
    }
    for (s32 bus = 0; bus < 2; bus++) {
        for (s32 i = 0; i < FX_NUM_PER_BUS; i++) {
            mFx[bus][i] = nullptr;
        }
        mNum[bus] = 0;
    }
}

// 0x005493CC (name is ours)
bool sead::AudioFxSet::setup(nw::snd::SoundHeap* soundHeap)
{
    if (soundHeap == nullptr) {
        soundHeap = DynamicCast<AudioPlayerCtr>(AudioMgr::s_pInstance->mPlayer)->m_pSoundHeap;
    }
    // the work buffers with 16 bytes each (the header of their block in the ExpHeap)
    u32 size = 0;
    for (s32 bus = 0; bus < 2; bus++) {
        for (s32 i = 0; i < mNum[bus]; i++) {
            if (mFx[bus][i] != nullptr) {
                size += mFx[bus][i]->mHolder->getRequiredMemSize() + 16;
            }
        }
    }
    field_0x28 = false;
    // 32 bytes more for the alignment of the work buffers
    mMemoryMgr->allocFromSoundHeap(mFxMgr, soundHeap, size + 32);
    bool result = true;
    for (s32 bus = 0; bus < 2; bus++) {
        for (s32 i = 0; i < mNum[bus]; i++) {
            if (mFx[bus][i] != nullptr) {
                mFxMgr->setup(mFx[bus][i], mMemoryMgr, 32);
                if (!mFxMgr->append(static_cast<AudioGlobal::AuxBus>(bus), mFx[bus][i])) {
                    result = false;
                }
            }
        }
    }
    return result;
}

// 0x00549604 (name is ours)
bool sead::AudioFxSet::append(s32 bus, AudioFx* fx)
{
    if (mNum[bus] >= FX_NUM_PER_BUS) {
        return false;
    }
    if (!canAppend(bus, fx)) {
        return false;
    }
    mFx[bus][mNum[bus]] = fx;
    mNum[bus]++;
    return true;
}

// 0x00141BD8 (name is ours)
sead::AudioFxSet::~AudioFxSet()
{
    reset(0);
    if (mFxMgr != nullptr) {
        delete mFxMgr;
        mFxMgr = nullptr;
    }
    if (mMemoryMgr != nullptr) {
        delete mMemoryMgr;
        mMemoryMgr = nullptr;
    }
}
} // namespace sead
