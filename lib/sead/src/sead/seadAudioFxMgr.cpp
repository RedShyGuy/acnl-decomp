#include "sead/seadAudioFxHolder.h"
#include "sead/seadAudioFxMemoryMgr.h"
#include "sead/seadAudioFxMgr.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioSystem.h"

namespace sead {
// 0x0053E260 | nintendogs:callseq [tier A]
bool sead::AudioFxMgr::setup(AudioFx* fx, AudioFxMemoryMgr* memoryMgr, s32 alignment)
{
    if (mList.indexOf(mList.objToListNode(fx)) >= 0) {
        return true;
    }
    u32 size = fx->mHolder->getRequiredMemSize();
    void* buffer = memoryMgr->alloc(size, alignment);
    bool result = fx->mHolder->assignWorkBuffer(reinterpret_cast<uptr>(buffer), size);
    fx->mMemoryMgr = memoryMgr;
    mList.pushBack(fx);
    return result;
}

// 0x0053E2F4 | nintendogs:bytes-fuzzy [tier B]
bool sead::AudioFxMgr::append(AudioGlobal::AuxBus bus, AudioFx* fx)
{
    if (mList.indexOf(mList.objToListNode(fx)) < 0) {
        return false;
    }
    bool result = AudioMgr::s_pInstance->mSystem->appendEffect(bus, fx);
    if (result) {
        fx->mBus = bus;
    }
    return result;
}

// 0x0053E348 | nintendogs:callseq [tier A]
void sead::AudioFxMgr::dispose(AudioFxMemoryMgr* memoryMgr)
{
    u8 busMask = 0;
    for (AudioFx* fx = mList.front(); fx != mList.endObj(); fx = mList.next(fx)) {
        if (fx->mMemoryMgr == memoryMgr && fx->mBus >= 0) {
            busMask |= 1 << fx->mBus;
        }
    }
    for (s32 i = 0; i < AudioGlobal::AUX_BUS_NUM; i++) {
        if ((busMask & (1 << i)) == 0) {
            continue;
        }
        AudioGlobal::AuxBus bus = static_cast<AudioGlobal::AuxBus>(i);
        AudioMgr::s_pInstance->mSystem->clearEffect(bus, 0);
        for (AudioFx* fx = mList.front(); fx != mList.endObj(); fx = mList.next(fx)) {
            if (fx->mBus == bus) {
                fx->mBus = AudioGlobal::AUX_BUS_NONE;
            }
        }
    }
    AudioFx* fx = mList.front();
    while (fx != mList.endObj()) {
        AudioFx* next = mList.next(fx);
        if (fx->mMemoryMgr == memoryMgr) {
            fx->mHolder->releaseWorkBuffer();
            fx->mMemoryMgr = nullptr;
            mList.erase(fx);
        }
        fx = next;
    }
}
} // namespace sead
