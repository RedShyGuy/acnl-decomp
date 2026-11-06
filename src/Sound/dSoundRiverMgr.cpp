#include "Sound/dSoundRiverMgr.h"
#include "Sound/dSoundRiverMgr_SingletonDisposer_.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjRiver.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "sead/seadNew.h"

// 0x0094E550
SoundRiverMgr* SoundRiverMgr::s_pInstance = NULL;
// 0x0094E554
SoundRiverMgr::SingletonDisposer_* SoundRiverMgr::s_pSingletonDisposer = NULL;

SoundRiverMgr::SoundRiverMgr() : m_ObjNum(0)
{
    for (u32 i = 0; i < OBJ_NUM; i++) {
        m_pObjs[i] = NULL;
    }
}

// 0x00247274
SoundRiverMgr* SoundRiverMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundRiverMgr)];
        SoundRiverMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundRiverMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x001235B0
void SoundRiverMgr::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        if (s_pInstance != NULL) {
            s_pInstance->~SoundRiverMgr();
            ::operator delete(s_pInstance);
        }
        s_pInstance = NULL;
    }
}

// 0x002473B0
u32 SoundRiverMgr::Register(SoundObjRiver* obj)
{
    if (m_ObjNum < OBJ_NUM && obj != NULL) {
        for (u32 i = 0; i < OBJ_NUM; i++) {
            if (m_pObjs[i] == NULL) {
                m_pObjs[i] = obj;
                m_ObjNum++;
                return i;
            }
        }
    }
    return INVALID_INDEX;
}

// 0x00247408
void SoundRiverMgr::Update()
{
    f32 volumeSum = 0.0f;
    f32 maxVolume = 0.0f;
    f32 pan = 0.0f;
    f32 surroundPan = 0.0f;
    for (s32 i = 0; i < OBJ_NUM; i++) {
        SoundObjRiver* obj = m_pObjs[i];
        if (obj != NULL && obj->m_IsInRange) {
            f32 volume = obj->m_Volume;
            if (maxVolume < volume) {
                maxVolume = volume;
            }
            volumeSum += volume;
            pan += obj->m_Pan * volume;
            surroundPan += obj->m_SurroundPan * volume;
        }
    }
    if (0.0f < maxVolume) {
        pan /= volumeSum;
        surroundPan /= volumeSum;
        SoundMgr::s_pInstance->HoldSound(&m_Handle, SE_ENV_RIVER);
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetVolume(maxVolume, 0);
        }
        m_Handle.SetPan(pan);
        m_Handle.SetSurroundPan(surroundPan);
    }
}

// 0x002474F0
void SoundRiverMgr::Unregister(u32 index)
{
    if (index < OBJ_NUM && m_pObjs[index] != NULL) {
        m_pObjs[index] = NULL;
        m_ObjNum--;
    }
}
