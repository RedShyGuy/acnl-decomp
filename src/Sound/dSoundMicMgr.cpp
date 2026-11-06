#include "Sound/dSoundMicMgr.h"
#include "Sound/dSoundMicMgr_SingletonDisposer_.h"
#include "nn/mic/CTR/CTR_Api.h"
#include "nn/mic/CTR/detail/mic_Mic.h"
#include "nn/nstd/nstd_String.h"
#include "sead/seadNew.h"

namespace {
// the buffer shared with the mic module (page aligned in the original)
// 0x00984000
u8 s_MicBuffer[0x1000] DECOMP_ALIGN(0x1000);
} // namespace

// 0x0094E0A0
SoundMicMgr* SoundMicMgr::s_pInstance = NULL;
// 0x0094E0A4
SoundMicMgr::SingletonDisposer_* SoundMicMgr::s_pSingletonDisposer = NULL;

// 0x0083C468
const u8 SoundMicMgr::s_Rates[2] = {nn::mic::CTR::SAMPLING_RATE_10910, nn::mic::CTR::SAMPLING_RATE_8180};

SoundMicMgr::SoundMicMgr()
    : m_pHeap(NULL), m_State(STATE_NONE), m_User(USER_NONE), m_BufferSize(0), m_pBuffer(NULL), m_pReadPos(NULL),
      m_Gain(43), m_Rate(nn::mic::CTR::SAMPLING_RATE_10910), m_pSamples(NULL), m_SampleBytes(0), m_SamplesSize(0)
{
    m_IsClient[0] = false;
    m_IsClient[1] = false;
}

// 0x001233EC
SoundMicMgr* SoundMicMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundMicMgr)];
        SoundMicMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundMicMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012334C
void SoundMicMgr::Initialize(sead::Heap* heap)
{
    if (m_State != STATE_NONE) {
        return;
    }
    if (nn::mic::CTR::Initialize().IsFailure()) {
        return;
    }
    if (nn::mic::CTR::SetBuffer(s_MicBuffer, sizeof(s_MicBuffer)).IsFailure()) {
        return;
    }
    m_pBuffer = s_MicBuffer;
    size_t size;
    if (nn::mic::CTR::GetSamplingBufferSize(&size).IsFailure()) {
        return;
    }
    m_BufferSize = size;
    if (nn::mic::CTR::detail::Mic::GetPGAB(&m_Gain).IsFailure()) {
        return;
    }
    m_SamplesSize = m_BufferSize;
    m_pSamples = new (heap, 4) s16[m_BufferSize >> 1];
    m_User = USER_NONE;
    m_State = STATE_INITIALIZED;
    m_pHeap = heap;
}

// 0x0013ABFC
void SoundMicMgr::Finalize()
{
    if (m_State == STATE_NONE) {
        return;
    }
    m_Asr.Stop();
    if (m_State == STATE_SAMPLING) {
        if (nn::mic::CTR::detail::Mic::StopSampling().IsSuccess()) {
            m_SampleBytes = 0;
            m_State = STATE_INITIALIZED;
        }
        m_IsClient[0] = false;
        m_IsClient[1] = false;
    }
    nn::mic::CTR::detail::Mic::SetMicBiasEntry(false);
    nn::mic::CTR::ResetBuffer();
    nn::mic::CTR::Finalize();
    if (m_pSamples != NULL) {
        delete[] m_pSamples;
        m_pSamples = NULL;
    }
    m_User = USER_NONE;
    m_State = STATE_NONE;
}

// 0x0013E1E8
SoundMicMgr::~SoundMicMgr()
{
    Finalize();
}

// 0x001D9B08
void SoundMicMgr::StopAsr()
{
    if (m_User == USER_NONE || m_User == USER_0) {
        m_Asr.Stop();
        m_User = USER_NONE;
    }
}

// 0x001D9B34
bool SoundMicMgr::StartSampling(u32 client)
{
    if (m_State == STATE_SAMPLING || m_State == STATE_NONE) {
        return false;
    }
    if (nn::mic::CTR::detail::Mic::SetMicBiasEntry(true).IsFailure()) {
        return false;
    }
    m_pReadPos = s_MicBuffer;
    if (nn::mic::CTR::StartSampling(nn::mic::CTR::SAMPLING_TYPE_SIGNED_16BIT,
                                    static_cast<nn::mic::CTR::SamplingRate>(s_Rates[client]), 0, m_BufferSize, true)
            .IsFailure()) {
        return false;
    }
    m_State = STATE_SAMPLING;
    m_Rate = s_Rates[client];
    if (client < 2) {
        m_IsClient[client] = true;
    } else {
        m_IsClient[0] = true;
    }
    return true;
}

// 0x001D9BC8
void SoundMicMgr::StopSampling(u32 client)
{
    if (client < 2) {
        m_IsClient[client] = false;
    } else {
        m_IsClient[0] = false;
    }
    bool isUsed = false;
    for (s32 i = 0; i < 2; i++) {
        if (m_IsClient[i]) {
            isUsed = true;
        }
    }
    if (!isUsed && m_State != STATE_NONE && m_State == STATE_SAMPLING &&
        nn::mic::CTR::detail::Mic::StopSampling().IsSuccess()) {
        m_SampleBytes = 0;
        m_State = STATE_INITIALIZED;
    }
}

// 0x001D9C4C
void SoundMicMgr::LoadAsr()
{
    if (m_User == USER_NONE) {
        m_Asr.Load(m_pHeap);
    }
}

// 0x001D9D00
void SoundMicMgr::Update()
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (m_State == STATE_SAMPLING) {
        uptr last = 0;
        if (nn::mic::CTR::GetLastSamplingAddress(&last).IsFailure()) {
            return;
        }
        u8* read = m_pReadPos;
        if (reinterpret_cast<uptr>(read) < last) {
            m_SampleBytes = last - reinterpret_cast<uptr>(read);
            if (m_SampleBytes <= m_SamplesSize) {
                nnnstdMemCpy(m_pSamples, read, m_SampleBytes);
            }
        } else {
            u32 tail = m_BufferSize - (read - m_pBuffer);
            u32 head = last - reinterpret_cast<uptr>(m_pBuffer);
            m_SampleBytes = tail + head;
            if (m_SampleBytes <= m_SamplesSize) {
                nnnstdMemCpy(m_pSamples, read, tail);
                nnnstdMemCpy(m_pSamples + (tail >> 1), s_MicBuffer, head);
            }
        }
        m_pReadPos = reinterpret_cast<u8*>(last);
    }
    m_Asr.Update();
    if (m_Asr.m_Progress == 2 || m_Asr.m_Progress == 3) {
        m_User = USER_NONE;
    }
    m_User1.Update();
}

// 0x001D9DE8
void SoundMicMgr::StopUser1()
{
    m_User1.Stop();
    if (m_Rate != s_Rates[0] && nn::mic::CTR::AdjustSampling(static_cast<nn::mic::CTR::SamplingRate>(s_Rates[0])).IsSuccess()) {
        m_Rate = s_Rates[0];
    }
    m_User = USER_NONE;
}

namespace {
const u8 MIC_GAIN = 43;
} // namespace

// 0x001D9E38
bool SoundMicMgr::StartUser1()
{
    if (m_User == USER_1) {
        return true;
    }
    if (m_User != USER_NONE) {
        return false;
    }
    if (m_State != STATE_SAMPLING && !StartSampling(1)) {
        return false;
    }
    if (m_Rate != s_Rates[1]) {
        if (nn::mic::CTR::AdjustSampling(static_cast<nn::mic::CTR::SamplingRate>(s_Rates[1])).IsFailure()) {
            return false;
        }
        m_Rate = s_Rates[1];
    }
    if (m_Gain != MIC_GAIN && nn::mic::CTR::detail::Mic::SetPGABEntry(MIC_GAIN).IsSuccess()) {
        m_Gain = MIC_GAIN;
    }
    m_User1.Start();
    m_User = USER_1;
    return true;
}

// 0x001D9EF0
bool SoundMicMgr::StartAsr()
{
    if (m_User == USER_0) {
        return true;
    }
    if (m_User != USER_NONE) {
        return false;
    }
    if (m_State != STATE_SAMPLING && !StartSampling(0)) {
        return false;
    }
    if (m_Rate != s_Rates[0]) {
        if (nn::mic::CTR::AdjustSampling(static_cast<nn::mic::CTR::SamplingRate>(s_Rates[0])).IsFailure()) {
            return false;
        }
        m_Rate = s_Rates[0];
    }
    if (m_Gain != MIC_GAIN && nn::mic::CTR::detail::Mic::SetPGABEntry(MIC_GAIN).IsSuccess()) {
        m_Gain = MIC_GAIN;
    }
    bool isStarted = m_Asr.Start();
    if (isStarted) {
        m_User = USER_0;
    }
    return isStarted;
}

// 0x001D9FA8
void SoundMicMgr::ResetAsr()
{
    m_Asr.Reset();
    m_User = USER_NONE;
}

// 0x001D9FC4
void SoundMicMgr::Reset()
{
    if (m_User1.m_IsActive) {
        m_User1.Stop();
    }
    StopSampling(1);
    if (m_Asr.m_State >= 2) {
        m_Asr.Stop();
    }
    StopSampling(0);
}

// 0x0033E94C
void SoundMicMgr::SetAsrParam(const void* data, s32 size)
{
    m_Asr.SetParam(data, size);
}
