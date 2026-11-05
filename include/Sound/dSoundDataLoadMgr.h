#pragma once

#include "decomp.h"
#include "Sound/dSoundDataLoadRequest.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"

namespace sead {
class Heap;
} // namespace sead

// The sound heaps of the game and the loading of sound archive data into them (the class name is from the RTTI of
// its disposer, everything else is ours). A heap is freed back to a level (nw::snd::SoundHeap::LoadState); while a
// request of the heap is loading, that waits until the next Update.
class SoundDataLoadMgr
{
public:
    class SingletonDisposer_;

    enum
    {
        HEAP_NUM = 6,
        HEAP_OWN_NUM = 5,  // heaps 0..4 belong to the manager
        HEAP_PLAYER = 5,   // the sound heap of the audio player
    };

    static SoundDataLoadMgr* createInstance(sead::Heap* heap); // 0x0012392C

    DECOMP_ALWAYS_INLINE SoundDataLoadMgr();
    // (only the members are destroyed)
    ~SoundDataLoadMgr() {}

    // the audio player and its heap; loads GROUP_STATIC into it
    void Initialize(); // 0x001237B4
    void Finalize(); // 0x0013E404
    // heaps 0 and 1 in heap
    void CreateHeaps(sead::Heap* heap); // 0x001238D4
    // heaps 2..4 (4 only if !all) in HeapInit::s_pSoundDataHeap
    void CreateStageHeaps(bool all); // 0x002BD06C
    void DestroyStageHeaps(); // 0x002BD0DC
    // the pending frees
    void Update(); // 0x002BD12C

    // queues the data of the item for the heap; the level of the heap before it (-1: not queued)
    s32 Load(u32 itemId, u32 index); // 0x002BCF18
    // the heap back to the level (false: no heap or its request is busy)
    bool Unload(s32 level, u32 index); // 0x00136004
    // the same, but waits for a busy request
    bool UnloadOrDefer(s32 level, u32 index); // 0x0013E294
    // heaps 0 and 1 emptied
    void UnloadHeaps(); // 0x0013C374
    // the current level of the heap (-1: none)
    s32 GetLevel(u32 index); // 0x00139A70

    DECOMP_EMIT_COPY bool IsLoaded(u32 itemId, u32 loadFlag = 0xFFFFFFFF) const // 0x0071DF8C
    {
        return static_cast<nw::snd::internal::SoundArchiveLoader*>(m_pPlayer->m_pSoundDataMgr)->IsDataLoaded(itemId, loadFlag);
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsBusy(u32 index) const // 0x0071DF9C
    {
        if (index >= HEAP_NUM || m_pHeaps[index] == NULL) {
            return false;
        }
        return m_Requests[index].IsBusy();
    }
    DECOMP_EMIT_COPY bool HasHeap(u32 index) const // 0x0071DFCC
    {
        if (index >= HEAP_NUM) {
            return false;
        }
        return m_pHeaps[index] != NULL;
    }

    static SoundDataLoadMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    // the sizes of heaps 0..4 (0: all of the parent)
    static const u32 s_HeapSizes[HEAP_OWN_NUM];

    u32 m_SingletonDisposerBuffer[4];               // 0x00
    sead::AudioPlayerCtr* m_pPlayer;                // 0x10
    nw::snd::SoundHeap* m_pHeaps[HEAP_NUM];         // 0x14
    SoundDataLoadRequest m_Requests[HEAP_NUM];      // 0x2C
    u8 m_PendingMask;                               // 0xBC a bit per heap: free it when its request is done
    s32 m_PendingLevels[HEAP_NUM];                  // 0xC0

private:
    // loads at once (the level of the heap before it)
    s32 LoadNow(u32 itemId, u32 index);
};
ASSERT_SIZE(SoundDataLoadMgr, 0xD8);
