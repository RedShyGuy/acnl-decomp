#include "Sound/dSoundMicMgr.h"

// 0x00128FBC
SoundMicUser1::SoundMicUser1() : m_IsActive(0), m_IsDetected(0), m_LastLoudCount(0), m_HitIndex(0)
{
    for (s32 i = 0; i < 10; i++) {
        m_Hits[i] = 0;
    }
}

// 0x0032CA30
void SoundMicUser1::Update()
{
    if (!m_IsActive) {
        return;
    }
    SoundMicMgr* mic = SoundMicMgr::s_pInstance;
    const s16* samples = mic->m_pSamples;
    u32 count = mic->m_SampleBytes >> 1;
    u32 rest = count & 127;
    u32 blocks = count >> 7;
    if (rest != 0) {
        blocks++;
    }
    u32 offset = 0;
    for (u32 block = 0; block < blocks; block++) {
        u32 size = 128;
        if (block == blocks - 1 && rest != 0) {
            // a short last block only counts with more than 96 samples
            if (rest <= 96) {
                break;
            }
            size = rest;
        }
        u32 sum = 0;
        s32 crossings = 0;
        s32 loud = 0;
        s32 veryLoud = 0;
        s32 last = 0;
        const s16* p = samples + offset;
        for (u32 i = 0; i < size; i++) {
            s32 sample = *p++;
            sum += sample < 0 ? 0 : sample;
            if (last < 0 && sample >= 0) {
                crossings++;
            }
            if (sample > 700) {
                loud++;
                if (sample > 10000) {
                    veryLoud++;
                }
            }
            last = sample;
        }
        u32 average = sum / size;
        s32 change = loud - m_LastLoudCount;
        if (change < 0) {
            change = -change;
        }
        bool isHit = average - 201 < 4799 && static_cast<u32>(crossings - 1) < 6 && static_cast<u32>(change - 11) < 89;
        m_LastLoudCount = loud;
        if (veryLoud > 10 && crossings < 10) {
            isHit = true;
        }
        m_Hits[m_HitIndex] = isHit;
        m_HitIndex++;
        if (m_HitIndex >= 10) {
            m_HitIndex = 0;
        }
        offset += size;
    }
    s32 hits = 0;
    for (s32 i = 0; i < 10; i++) {
        if (m_Hits[i] == 1) {
            hits++;
        }
    }
    m_IsDetected = hits > 3;
}

// 0x0032CC40
void SoundMicUser1::Stop()
{
    if (m_IsActive) {
        m_IsActive = 0;
        m_IsDetected = 0;
    }
}

// 0x0032CC5C
void SoundMicUser1::Start()
{
    if (m_IsActive) {
        return;
    }
    m_IsActive = 1;
    m_IsDetected = 0;
    m_LastLoudCount = 0;
    for (s32 i = 0; i < 10; i++) {
        m_Hits[i] = 0;
    }
    m_HitIndex = 0;
}
