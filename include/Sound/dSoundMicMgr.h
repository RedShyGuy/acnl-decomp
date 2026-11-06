#pragma once

#include "decomp.h"
#include "sead/seadCheckedArray.h"
#include "Other/ddLoadSplit.h"

namespace sead { class Heap; }

// The speech recognition on the microphone data (the name of its heap is "AsrHeap"; the data files are
// rom:/Sound/asr/<language>/*.bin; the recognizer itself is a library without symbols, Func142BC0.. in sound_Extern.h).
// All names are ours.
class SoundMicAsr
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_LOADING = 1,
        STATE_READY = 2,
        STATE_WAIT_SOUND = 3,  // waits for the end of SE_ACT_MEGAPHONE_SET
        STATE_WAIT_TIMER = 4,
        STATE_RECOGNIZING = 5,
    };
    // m_Progress
    enum Progress
    {
        PROGRESS_NONE = 0,
        PROGRESS_LISTENING = 1,
        PROGRESS_RESULT = 2,
        PROGRESS_ERROR = 3,
        PROGRESS_ERROR_7 = 4, // the recognizer returned -7
    };
    enum
    {
        FILE_NUM = 6,
        RESULT_NUM = 10,
    };

    // a data file for the recognizer
    struct FileEntry
    {
        s32 id;      // 0x0
        void* pData; // 0x4
        u32 size;    // 0x8
    };

    typedef void (SoundMicAsr::*UpdateFunc)();

    SoundMicAsr(); // 0x00129038
    void Stop(); // 0x001404A4
    void Load(sead::Heap* heap); // 0x0033E834
    void SetParam(const void* data, s32 size); // 0x0033E954
    void Update(); // 0x0033EEB8
    bool Start(); // 0x0033EAF4
    void Reset(); // 0x0033EBF8
    // the first result (-1: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetResult() const // 0x00726764
    {
        return m_Progress == PROGRESS_RESULT ? m_Results[0] : -1;
    }

    // the table of Update (0x0083C46C)
    void UpdateNothing(); // 0x0033EA64
    void UpdateLoading(); // 0x0033ED1C
    void UpdateWaitSound(); // 0x0033EA68
    void UpdateWaitTimer(); // 0x0033EC44
    void UpdateRecognizing(); // 0x0033EC60

    static const UpdateFunc s_UpdateFuncs[6];
    static const s32 s_FileIds[FILE_NUM];
    static const char* s_Directory;
    static const char* s_FileNames[FILE_NUM];
    static const char* s_Languages[FILE_NUM];
    static bool s_IsSoundFound;

    u8 m_State;                    // 0x000 State
    s32 m_Progress;                // 0x004 Progress
    sead::Heap* m_pHeap;           // 0x008
    dLoadSplit m_Splits[FILE_NUM]; // 0x00C
    u8* m_pWork;                   // 0x594
    FileEntry m_Files[FILE_NUM];   // 0x598
    const void* m_pParamData;      // 0x5E0
    s32 m_ParamSize;               // 0x5E4
    s32 m_Language;                // 0x5E8 an index of s_Languages
    u8 m_LanguageCode;             // 0x5EC of the recognizer
    s32 m_Timer;                   // 0x5F0
    s16 m_Results[RESULT_NUM];     // 0x5F4

private:
    void InitLanguage(); // 0x0033E968
};
ASSERT_OFFSET(SoundMicAsr, m_pWork, 0x594);
ASSERT_OFFSET(SoundMicAsr, m_Results, 0x5F4);
ASSERT_SIZE(SoundMicAsr, 0x608);

// The second user of the microphone data: looks at the samples in blocks of 128 (level, zero crossings, samples above 700)
// and reports when more than 3 of the last 10 blocks fit. What the game does with it is still open; names are ours.
class SoundMicUser1
{
public:
    SoundMicUser1(); // 0x00128FBC
    void Update(); // 0x0032CA30
    void Stop(); // 0x0032CC40
    void Start(); // 0x0032CC5C

    u8 m_IsActive;                     // 0x00
    s8 m_IsDetected;                   // 0x01
    s32 m_LastLoudCount;               // 0x04
    sead::CheckedArray<u8, 10> m_Hits; // 0x08
    s32 m_HitIndex;                    // 0x14
};
ASSERT_SIZE(SoundMicUser1, 0x18);

// The microphone: sampling into a shared buffer, a copy of the new samples each frame for the two users; two clients can
// hold the sampling open (the class name is from the RTTI of its disposer, everything else is ours).
class SoundMicMgr
{
public:
    class SingletonDisposer_;

    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_INITIALIZED = 1,
        STATE_SAMPLING = 2,
    };
    // m_User
    enum User
    {
        USER_0 = 0,
        USER_1 = 1,
        USER_NONE = 2,
    };

    SoundMicMgr();
    // 0x0013E1E8
    ~SoundMicMgr();
    static SoundMicMgr* createInstance(sead::Heap* heap); // 0x001233EC

    void Initialize(sead::Heap* heap); // 0x0012334C
    void Finalize(); // 0x0013ABFC
    void StopAsr(); // 0x001D9B08
    // the sampling for a client (false: not initialized or it fails)
    bool StartSampling(u32 client); // 0x001D9B34
    void StopSampling(u32 client); // 0x001D9BC8
    void LoadAsr(); // 0x001D9C4C
    void Update(); // 0x001D9D00
    void StopUser1(); // 0x001D9DE8
    bool StartUser1(); // 0x001D9E38
    bool StartAsr(); // 0x001D9EF0
    void ResetAsr(); // 0x001D9FA8
    // both clients and both users stop
    void Reset(); // 0x001D9FC4
    void SetAsrParam(const void* data, s32 size); // 0x0033E94C

    // the sampling rate of each client
    static const u8 s_Rates[2];
    static SoundMicMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00
    sead::Heap* m_pHeap;              // 0x10
    u8 m_State;                       // 0x14 State
    u8 m_User;                        // 0x15 User
    bool m_IsClient[2];               // 0x16
    u32 m_BufferSize;                 // 0x18
    u8* m_pBuffer;                    // 0x1C
    u8* m_pReadPos;                   // 0x20
    u8 m_Gain;                        // 0x24
    u8 m_Rate;                        // 0x25
    s16* m_pSamples;                  // 0x28 the copy of the new samples
    u32 m_SampleBytes;                // 0x2C
    u32 m_SamplesSize;                // 0x30
    SoundMicAsr m_Asr;            // 0x34
    SoundMicUser1 m_User1;            // 0x63C
};
ASSERT_OFFSET(SoundMicMgr, m_Asr, 0x34);
ASSERT_OFFSET(SoundMicMgr, m_User1, 0x63C);
ASSERT_SIZE(SoundMicMgr, 0x654);
