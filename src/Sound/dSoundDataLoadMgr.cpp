#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundDataLoadMgr_SingletonDisposer_.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Other/dHeapInit.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioSoundHeapCtr.h"
#include "sead/seadNew.h"

// 0x0094DD20
SoundDataLoadMgr* SoundDataLoadMgr::s_pInstance = NULL;
// 0x0094DD24
SoundDataLoadMgr::SingletonDisposer_* SoundDataLoadMgr::s_pSingletonDisposer = NULL;

// 0x0012392C
SoundDataLoadMgr* SoundDataLoadMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundDataLoadMgr)];
        SoundDataLoadMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundDataLoadMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

SoundDataLoadMgr::SoundDataLoadMgr() : m_pPlayer(NULL)
{
    m_PendingMask = 0;
    for (u32 i = 0; i < HEAP_NUM; i++) {
        m_pHeaps[i] = NULL;
        m_PendingLevels[i] = -1;
    }
}

inline s32 SoundDataLoadMgr::LoadNow(u32 itemId, u32 index)
{
    if (m_pHeaps[index] == NULL || IsLoaded(itemId)) {
        return -1;
    }
    s32 level = m_pHeaps[index]->GetCurrentLevel();
    if (m_pPlayer->m_pSoundDataMgr->loadData(itemId, 0xFFFFFFFF, 0, m_pHeaps[index])) {
        m_pHeaps[index]->SaveState();
    }
    return level;
}

// 0x001237B4
void SoundDataLoadMgr::Initialize()
{
    m_pPlayer = sead::DynamicCast<sead::AudioPlayerCtr>(sead::AudioMgr::s_pInstance->mPlayer);
    m_pHeaps[HEAP_PLAYER] = m_pPlayer->m_pSoundHeap;
    LoadNow(GROUP_STATIC, HEAP_PLAYER);
}

// 0x001238D4
void SoundDataLoadMgr::CreateHeaps(sead::Heap* heap)
{
    for (u32 i = 0; i < 2; i++) {
        if (m_pHeaps[i] == NULL) {
            m_pHeaps[i] = new (heap, 32) sead::AudioSoundHeapCtr(s_HeapSizes[i], heap);
        }
    }
}

// 0x002BD06C
void SoundDataLoadMgr::CreateStageHeaps(bool all)
{
    sead::Heap* heap = HeapInit::s_pSoundDataHeap;
    if (heap == NULL) {
        return;
    }
    for (u32 i = all ? 2 : 4; i <= 4; i++) {
        if (m_pHeaps[i] == NULL) {
            m_pHeaps[i] = new (heap, 32) sead::AudioSoundHeapCtr(s_HeapSizes[i], heap);
        }
    }
}

// 0x002BD0DC
void SoundDataLoadMgr::DestroyStageHeaps()
{
    for (s32 i = 4; i >= 2; i--) {
        if (m_pHeaps[i] != NULL) {
            m_pHeaps[i]->Clear();
            delete m_pHeaps[i];
            m_pHeaps[i] = NULL;
        }
    }
}

// 0x0013E404
void SoundDataLoadMgr::Finalize()
{
    for (u32 i = 0; i < HEAP_OWN_NUM; i++) {
        if (m_pHeaps[i] != NULL) {
            m_Requests[i].Cancel();
            m_pHeaps[i]->Destroy();
            delete m_pHeaps[i];
            m_pHeaps[i] = NULL;
        }
    }
    m_Requests[HEAP_PLAYER].Cancel();
}

// 0x002BD12C
void SoundDataLoadMgr::Update()
{
    if (SoundMgr::s_pInstance->IsPaused() || m_PendingMask == 0) {
        return;
    }
    for (u32 i = 0; i < HEAP_NUM; i++) {
        u32 bit = 1 << i;
        if ((m_PendingMask & bit) == 0) {
            continue;
        }
        if (m_pHeaps[i] != NULL) {
            if (m_Requests[i].IsBusy()) {
                continue;
            }
            m_pHeaps[i]->LoadState(m_PendingLevels[i]);
        }
        m_PendingMask ^= bit;
        m_PendingLevels[i] = -1;
    }
}

// 0x002BCF18
s32 SoundDataLoadMgr::Load(u32 itemId, u32 index)
{
    if (index >= HEAP_NUM || m_pHeaps[index] == NULL || IsLoaded(itemId)) {
        return -1;
    }
    s32 level = m_pHeaps[index]->GetCurrentLevel();
    m_Requests[index].Request(m_pHeaps[index], itemId);
    return level;
}

// 0x00136004
bool SoundDataLoadMgr::Unload(s32 level, u32 index)
{
    if (index >= HEAP_NUM || m_pHeaps[index] == NULL || m_Requests[index].IsBusy()) {
        return false;
    }
    if (m_pHeaps[index]->GetCurrentLevel() > level) {
        m_pHeaps[index]->LoadState(level);
    }
    return true;
}

// 0x0013E294
bool SoundDataLoadMgr::UnloadOrDefer(s32 level, u32 index)
{
    if (index >= HEAP_NUM) {
        return false;
    }
    if (m_pHeaps[index] == NULL) {
        return false;
    }
    if (m_Requests[index].IsBusy()) {
        m_PendingMask |= 1 << index;
        m_PendingLevels[index] = level;
        return true;
    }
    if (m_pHeaps[index]->GetCurrentLevel() > level) {
        m_pHeaps[index]->LoadState(level);
    }
    return true;
}

// 0x0013C374
void SoundDataLoadMgr::UnloadHeaps()
{
    for (u32 i = 0; i < 2; i++) {
        UnloadOrDefer(0, i);
    }
}

// 0x00139A70
s32 SoundDataLoadMgr::GetLevel(u32 index)
{
    if (index >= HEAP_NUM || m_pHeaps[index] == NULL) {
        return -1;
    }
    return m_pHeaps[index]->GetCurrentLevel();
}
