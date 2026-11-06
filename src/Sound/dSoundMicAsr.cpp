#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundMicMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFormatFixedSafeString.h"
#include "sead/seadNew.h"

// 0x0094E0A8
bool SoundMicAsr::s_IsSoundFound;
// 0x0094E0AC
const char* SoundMicAsr::s_Directory = "Sound/asr";
// 0x0094E0B0
const char* SoundMicAsr::s_FileNames[FILE_NUM] = {"dtb.bin", "hmm.bin", "tree.bin", "phn.bin", "mdc.bin", "str.bin"};
// 0x0094E0C8
const char* SoundMicAsr::s_Languages[FILE_NUM] = {"JPN", "ENG", "FRA", "GER", "ITA", "SPA"};

namespace {
// is the sound SE_ACT_MEGAPHONE_SET
// 0x0033EE84
void CheckMegaphoneSound(nw::snd::SoundHandle& handle)
{
    if (handle.m_pSound != NULL && handle.m_pSound->m_Id == SE_ACT_MEGAPHONE_SET) {
        SoundMicAsr::s_IsSoundFound = true;
    }
}
} // namespace

// 0x00129038
SoundMicAsr::SoundMicAsr()
    : m_State(STATE_NONE), m_Progress(PROGRESS_NONE), m_pHeap(NULL), m_pWork(NULL), m_pParamData(NULL), m_ParamSize(0),
      m_Language(0), m_LanguageCode(1), m_Timer(0)
{
    for (s32 i = 0; i < RESULT_NUM; i++) {
        m_Results[i] = -1;
    }
}

// 0x001404A4
void SoundMicAsr::Stop()
{
    if (m_State != STATE_NONE) {
        if (m_State == STATE_WAIT_SOUND || m_State == STATE_WAIT_TIMER || m_State == STATE_RECOGNIZING) {
            Func142CFC(SoundMicMgr::s_pInstance->m_pSamples, 0);
            m_Timer = 0;
            m_State = STATE_READY;
            m_Progress = PROGRESS_NONE;
        }
        Func142BC0(0);
        for (s32 i = 0; i < FILE_NUM; i++) {
            m_Splits[i].Reset();
        }
        delete[] m_pWork;
        m_pHeap->destroy();
        m_pHeap = NULL;
    }
    m_State = STATE_NONE;
}

// 0x0033E834
void SoundMicAsr::Load(sead::Heap* heap)
{
    if (m_State != STATE_NONE) {
        return;
    }
    m_pHeap = sead::ExpHeap::create(0x55000, sead::SafeStringBase<char>("AsrHeap"), heap, sead::Heap::HEAP_DIRECTION_FORWARD,
                                    false);
    m_pWork = new (m_pHeap, 4) u8[Func151F00() & ~1];
    InitLanguage();
    for (s32 i = 0; i < FILE_NUM; i++) {
        m_Files[i].pData = NULL;
        m_Files[i].id = 0x7FFFFFFF;
        m_Files[i].size = 0;
        // (Japanese has no file 4)
        if (m_Language == 0 && s_FileIds[i] == 4) {
            continue;
        }
        sead::FormatFixedSafeString<32> path("%s/%s/%s", s_Directory, s_Languages[m_Language], s_FileNames[i]);
        m_Splits[i].Read(path, m_pHeap, 4);
    }
    m_State = STATE_LOADING;
}

// 0x0033E954
void SoundMicAsr::SetParam(const void* data, s32 size)
{
    if (m_State != STATE_RECOGNIZING) {
        m_pParamData = data;
        m_ParamSize = size;
    }
}

// 0x0033E968
void SoundMicAsr::InitLanguage()
{
    switch (GetRegion()) {
    case 0: // Japan
        m_Language = 0;
        m_LanguageCode = 1;
        break;
    case 1: // America, English
        m_Language = 1;
        m_LanguageCode = 3;
        break;
    case 2: // America, Spanish
        m_Language = 5;
        m_LanguageCode = 13;
        break;
    case 3: // America, French
        m_Language = 2;
        m_LanguageCode = 12;
        break;
    case 4: // Europe, English
        m_Language = 1;
        m_LanguageCode = 2;
        break;
    case 5: // Europe, Spanish
        m_Language = 5;
        m_LanguageCode = 5;
        break;
    case 6: // Europe, French
        m_Language = 2;
        m_LanguageCode = 4;
        break;
    case 7: // Europe, Italian
        m_Language = 4;
        m_LanguageCode = 7;
        break;
    case 8: // Europe, German
        m_Language = 3;
        m_LanguageCode = 6;
        break;
    default:
        m_Language = 0;
        m_LanguageCode = 1;
        break;
    }
}

// 0x0033EA64
void SoundMicAsr::UpdateNothing()
{
    // empty (in the original too)
}

// 0x0033EA68
void SoundMicAsr::UpdateWaitSound()
{
    s_IsSoundFound = false;
    SoundMgr::s_pInstance->GetSoundPlayer(6)->ForEachSound(CheckMegaphoneSound, false);
    if (!s_IsSoundFound) {
        m_State = STATE_RECOGNIZING;
    }
}

// 0x0033EAF4
bool SoundMicAsr::Start()
{
    if (m_State != STATE_READY || SoundMicMgr::s_pInstance->m_State != SoundMicMgr::STATE_SAMPLING) {
        return false;
    }
    for (s32 i = 0; i < RESULT_NUM; i++) {
        m_Results[i] = -1;
    }
    s16 config[8];
    if (Func151E64(config) != 0) {
        return false;
    }
    config[0] = 3;
    if (Func152608(config) != 0) {
        return false;
    }
    if (m_ParamSize <= 0 || m_pParamData == NULL) {
        return false;
    }
    if (Func152474(m_pParamData, static_cast<s16>(m_ParamSize)) != 0) {
        return false;
    }
    m_Progress = PROGRESS_NONE;
    if (Func1523C4() == 0) {
        m_State = STATE_WAIT_SOUND;
        return true;
    }
    m_State = STATE_READY;
    m_Progress = PROGRESS_ERROR;
    return false;
}

// 0x0033EBF8
void SoundMicAsr::Reset()
{
    if (m_State == STATE_WAIT_SOUND || m_State == STATE_WAIT_TIMER || m_State == STATE_RECOGNIZING) {
        Func142CFC(SoundMicMgr::s_pInstance->m_pSamples, 0);
        m_Timer = 0;
        m_State = STATE_READY;
        m_Progress = PROGRESS_NONE;
    }
}

// 0x0033EC44
void SoundMicAsr::UpdateWaitTimer()
{
    m_Timer--;
    if (m_Timer <= 0) {
        m_State = STATE_RECOGNIZING;
    }
}

// 0x0033EC60
void SoundMicAsr::UpdateRecognizing()
{
    SoundMicMgr* mic = SoundMicMgr::s_pInstance;
    s32 result = Func142CFC(mic->m_pSamples, static_cast<u16>(mic->m_SampleBytes >> 1));
    if (result == -7) {
        m_Progress = PROGRESS_ERROR_7;
    } else if (result <= -1) {
        m_Progress = PROGRESS_ERROR;
    } else if (result == 128) {
        m_Progress = PROGRESS_LISTENING;
        return;
    } else if (static_cast<u32>(result - 1) > 9) {
        return;
    } else if (Func151E0C(1) <= 0) {
        m_Progress = PROGRESS_ERROR;
    } else {
        for (s16 i = 0; i < result; i++) {
            m_Results[i] = Func151E0C(i + 1);
        }
        m_Progress = PROGRESS_RESULT;
    }
    m_State = STATE_READY;
}

// 0x0033ED1C
void SoundMicAsr::UpdateLoading()
{
    bool isLoaded = true;
    for (s32 i = 0; i < FILE_NUM; i++) {
        if (m_Language == 0 && s_FileIds[i] == 4) {
            m_Files[i].pData = NULL;
            m_Files[i].size = 0;
            m_Files[i].id = s_FileIds[i];
        } else if (!m_Splits[i].IsSucceeded()) {
            isLoaded = false;
        } else if (m_Files[i].id == 0x7FFFFFFF) {
            m_Files[i].pData = m_Splits[i].m_pData;
            m_Files[i].size = m_Splits[i].m_Size;
            m_Files[i].id = s_FileIds[i];
        }
    }
    if (!isLoaded) {
        return;
    }
    if (Func151F10(m_LanguageCode, m_Files, FILE_NUM, m_pWork) == 0) {
        m_State = STATE_READY;
    } else {
        Stop();
    }
}

// 0x0033EEB8
void SoundMicAsr::Update()
{
    (this->*s_UpdateFuncs[m_State])();
}
