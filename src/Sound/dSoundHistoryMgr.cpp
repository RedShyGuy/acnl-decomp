#include "Sound/dSoundHistoryMgr.h"
#include "Sound/dSoundHistoryMgr_SingletonDisposer_.h"
#include "sead/seadNew.h"

// 0x0094DFFC
SoundHistoryMgr* SoundHistoryMgr::s_pInstance = NULL;
// 0x0094E000
SoundHistoryMgr::SingletonDisposer_* SoundHistoryMgr::s_pSingletonDisposer = NULL;

// 0x00123680
SoundHistoryMgr* SoundHistoryMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundHistoryMgr)];
        SoundHistoryMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundHistoryMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x002A3818
void SoundHistoryMgr::Add(u32 soundId, u8 frames)
{
    s32 oldest = -1;
    u32 fewest = 255;
    for (s32 i = 0; i < 10; i++) {
        if (m_Entries[i].soundId == 0xFFFFFFFF) {
            m_Entries[i].soundId = soundId;
            m_Entries[i].frames = frames;
            m_EntryCount++;
            return;
        }
        if (m_Entries[i].frames < fewest) {
            oldest = i;
            fewest = m_Entries[i].frames;
        }
    }
    m_Entries[oldest].soundId = soundId;
    m_Entries[oldest].frames = frames;
}

// 0x002A3944
void SoundHistoryMgr::Add(u32 soundId, u8 frames, f32 value)
{
    s32 oldest = -1;
    s32 free = -1;
    u32 fewest = 255;
    for (s32 i = 0; i < 5; i++) {
        if (m_ValueEntries[i].soundId == soundId) {
            m_ValueEntries[i].frames = frames;
            m_ValueEntries[i].value = value;
            return;
        }
        if (m_ValueEntries[i].soundId == 0xFFFFFFFF) {
            if (free == -1) {
                free = i;
            }
        } else if (m_ValueEntries[i].frames < fewest) {
            oldest = i;
            fewest = m_ValueEntries[i].frames;
        }
    }
    if (free != -1) {
        m_ValueEntries[free].soundId = soundId;
        m_ValueEntries[free].frames = frames;
        m_ValueEntries[free].value = value;
        m_ValueEntryCount++;
    } else {
        m_ValueEntries[oldest].soundId = soundId;
        m_ValueEntries[oldest].frames = frames;
        m_ValueEntries[oldest].value = value;
    }
}

// 0x002A3A90
void SoundHistoryMgr::Update()
{
    if (m_EntryCount != 0) {
        for (s32 i = 0; i < 10; i++) {
            if (m_Entries[i].frames != 0) {
                m_Entries[i].frames--;
                if (m_Entries[i].frames == 0) {
                    m_Entries[i].soundId = 0xFFFFFFFF;
                    m_EntryCount--;
                }
            }
        }
    }
    if (m_ValueEntryCount != 0) {
        for (s32 i = 0; i < 5; i++) {
            if (m_ValueEntries[i].frames != 0) {
                m_ValueEntries[i].frames--;
                if (m_ValueEntries[i].frames == 0) {
                    m_ValueEntries[i].soundId = 0xFFFFFFFF;
                    m_ValueEntryCount--;
                }
            }
        }
    }
}
