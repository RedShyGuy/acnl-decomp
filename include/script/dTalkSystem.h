#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "nn/fnd/fnd_DateTime.h"
#include "Sound/dSoundMgr.h"
#include "libms/libms.h"
#include "script/dScriptExtern.h"
#include "script/dLoader.h"
#include "script/dWordFix.h"
#include "Other/dRegion.h"

namespace sead {
class Heap;
} // namespace sead

namespace script {
class RenderMain;
class RenderRuby;
class TalkWin;
class TalkMeasure;
class TalkSeq;
class TalkInsert;
class TalkPhrase;
class ITalkRecept;
class ChoiceStandardWin;
class TalkSystem;
struct ChoiceItem;
struct ChoiceItemData;
struct Tag;

// the label of the message as a text of three digits ("%03d"; name is ours)
void FormatLabelNumber(sead::BufferedSafeStringBase<char>* label, s32 number); // 0x005EAC28

// the choices of a tag into the items (cancel: the cancel index, NULL: none; a, b: by the kind of the choice); the
// length of the tag with its texts in characters (the inline copies of the choice code; names are ours)
s32 ReadChoices8(const Tag* tag, ChoiceItem* items, s32* cancel, u32 num, u8 a, u8 b); // 0x0075ADC0
s32 ReadChoices15(const Tag* tag, ChoiceItem* items, s32* cancel, u32 num, u8 a, u8 b); // 0x0075B064
s32 ReadChoices8_10(const Tag* tag, ChoiceItem* items, u32 num); // 0x0075B3C4
s32 ReadChoices15_10(const Tag* tag, ChoiceItem* items, u32 num); // 0x0075B628

// The attribute of the message (the bytes libms gives for its text; names are ours): [0] the kind of the speaker,
// [2] the number of the next message, [3] a value at the start (ITalkRecept::OnBegin), [4] one after each page
// (ITalkRecept::OnPageEnd).
struct TalkAttribute
{
    static const u8 SPEAKER_NUM = 39;  // (kind: the speaker of the attribute)

    DECOMP_EMIT_COPY TalkAttribute() : pData(NULL), speaker(SPEAKER_NUM) {} // 0x005EA860
    // the attribute of the message of the index in the message file of the talk
    DECOMP_EMIT_COPY TalkAttribute(TalkSystem* talk, s32 index); // 0x005EA834
    DECOMP_EMIT_COPY void SetSpeaker(u8 kind) { speaker = kind; } // 0x005EA82C
    // (inline)
    void Clear()
    {
        pData = NULL;
        speaker = SPEAKER_NUM;
    }

    // (inline) the kind of the speaker (from the attribute unless kind is set; none: 0)
    u8 GetSpeaker() const
    {
        u8 speaker = this->speaker;
        if (speaker >= SPEAKER_NUM) {
            speaker = pData != NULL ? pData[0] : 0;
        }
        return speaker;
    }

    const u8* pData;  // 0x0
    u8 speaker;       // 0x4 (SPEAKER_NUM: the one of the attribute)
};

// The state of the speech sounds of the talk: flags and a callback for every written character (names are ours).
struct TalkState
{
    typedef void (TalkState::*CharFunc)(u32 c, const char16* const* cur, const char16* const* end);
    static const u8 FLAG_INSTANT = 0x1;   // the speaker writes at once (no speech sound)
    static const u8 FLAG_TAG7_2 = 0x2;    // the tags 7/2 and 7/3
    static const u8 FLAG_FIXED_WINDOW = 0x4;  // the window of the speaker gives one speech sound for all
    static const u8 FLAG_TAG0_3 = 0x8;    // the tag 0/3 with the color 5
    static const u8 FLAG_SILENT = 0x63;   // no speech sound
    static const u8 FLAG_FIXED = 0x1C;    // the same speech sound for every character

    // (inline; the period ends sentences in the Western regions)
    explicit TalkState(TalkSystem* pOwner) : pOwner(pOwner), flags(0), isOpenSound(0), isSentenceEnd(0), pCharFunc(NULL)
    {
        s32 region = GetRegion();
        if (region == 1 || region == 2 || region == 3 || region == 4 || region == 5 || region == 6 || region == 7 ||
            region == 8) {
            pCharFunc = &TalkState::OnPeriod;
        }
    }

    // the voices of the recept, the flags after the speaker
    void SetSpeaker(const TalkAttribute* attribute); // 0x0060526C
    // (Western regions) a period ends the speech sounds of a sentence (6)
    void OnPeriod(u32 c, const char16* const* cur, const char16* const* end); // 0x00605020

    TalkSystem* pOwner;   // 0x0
    u8 flags;             // 0x4
    u8 isOpenSound;       // 0x5 the window plays its speech sound when the page is done
    u8 isSentenceEnd;     // 0x6 the speech sound 165 becomes 158
    CharFunc pCharFunc;   // 0x8
};
ASSERT_SIZE(TalkState, 0x10);

// the speech sound of a character (218: none; kind is not used; names are ours)
u16 CharToSpeechSound(u32 c, u32 kind); // 0x006050C8
// (the call of the above was removed by armlink: falls into it)
u16 CharToSpeechSound(u32 c); // 0x006050C0
// the speech sounds of the other characters (name is ours)
struct SpeechSoundEntry
{
    u16 c;
    u16 sound;
};
extern const SpeechSoundEntry s_SpeechSounds[44];
// (inline) the speech sound for a character of a talk (none while the state is silent; level: the volume in percent;
// name is ours)
inline void QueueTalkSpeech(const TalkState* state, u32 c, s32 level)
{
    if ((state->flags & TalkState::FLAG_SILENT) == 0) {
        u32 sound;
        if ((state->flags & TalkState::FLAG_FIXED) != 0) {
            sound = 159;
        } else {
            sound = CharToSpeechSound(c, 0);
            if (state->isSentenceEnd && sound == 165) {
                sound = 158;
            }
        }
        SoundMgr::SQueueSpeechChar(sound, level);
    }
}

// A message of a talk (names are ours): the file (Script/Talk/<file>.umsbt) and the label of a text, given as the index
// of the text, the number of its label or the label itself ("%03d"), with the name of the speaker.
struct TalkLabel
{
    static const u32 FLAG_INDEX = 1;
    static const u32 FLAG_NUMBER = 2;
    static const u32 FLAG_LABEL = 4;

    // (inline)
    TalkLabel() : m_Flags(0), m_Index(-1), m_Number(-1), m_NameMode(3), m_IsUnknown0x79(0) {}
    // (inline)
    void Clear()
    {
        m_Flags = 0;
        m_Index = -1;
        m_Number = -1;
        m_Label.clear();
        m_File.clear();
        static_cast<IWord&>(m_Name).Clear();
        m_NameMode = 3;
        m_IsUnknown0x79 = 0;
    }
    // (inline)
    void Set(const TalkLabel& src)
    {
        m_Flags = src.m_Flags;
        m_Index = src.m_Index;
        m_Number = src.m_Number;
        m_Label.copy(src.m_Label);
        m_File.copy(src.m_File);
        static_cast<IWord&>(m_Name).Copy(src.m_Name);
        m_NameMode = src.m_NameMode;
        m_IsUnknown0x79 = src.m_IsUnknown0x79;
    }

    void SetFile(const char* file); // 0x005E2444
    void SetIndex(s32 index); // 0x005E2478
    void SetName(const IWord& name, u8 mode); // 0x005E2488
    void ClearName(); // 0x005E24AC
    void SetNumber(s32 number); // 0x005E24D0
    // the index, the number and the label from each other (in message); false if there is no text
    bool Resolve(libms::Message* message); // 0x005E24E0
    // whether the label ends the talk (the number 0 or the label "000", or no label at all)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsEnd() const // 0x0075BD34
    {
        if ((m_Flags & FLAG_NUMBER) != 0 && m_Number != -1) {
            return m_Number == 0;
        }
        if ((m_Flags & FLAG_LABEL) != 0 && !m_Label.isEmpty()) {
            return m_Label.isEqual(s_EndLabel);
        }
        if ((m_Flags & FLAG_INDEX) != 0 && m_Index != -1) {
            return false;
        }
        return true;
    }

    u32 m_Flags;                     // 0x00
    s32 m_Index;                     // 0x04 the index of the text
    s32 m_Number;                    // 0x08 the label as a number (-1: none)
    sead::FixedSafeString<8> m_Label;  // 0x0C
    sead::FixedSafeString<32> m_File;  // 0x20
    WordFix<9> m_Name;               // 0x4C the name of the speaker
    u8 m_NameMode;                   // 0x78 (3: none)
    u8 m_IsUnknown0x79;              // 0x79

    // (inline) the number of a label of digits (-1: none, not a number or not below 1000)
    static s32 ParseNumber(const sead::BufferedSafeStringBase<char>& label)
    {
        s32 number = 0;
        for (s32 i = 0; i < label.getBufferSize(); i++) {
            char c = label.at(i);
            if (c == 0) {
                break;
            }
            if (static_cast<u32>(c - '0') > 9) {
                return -1;
            }
            number = number * 10 + c - '0';
        }
        if (static_cast<u32>(number) >= 1000) {
            return -1;
        }
        return number;
    }

    static sead::FixedSafeString<8> s_EndLabel;
    // the buffer of the labels libms gives
    static sead::FixedSafeString<256> s_LabelBuffer;
};
ASSERT_SIZE(TalkLabel, 0x7C);

// a message file of the talk (names are ours)
struct TalkFile
{
    // (inline; out of line for the arrays)
    DECOMP_EMIT_COPY TalkFile() : m_Loader(false), m_pHeap(NULL) {} // 0x005FE108
    // (inline; out of line for the arrays)
    DECOMP_EMIT_COPY ~TalkFile() {} // 0x005FE134

    // (inline)
    void Clear()
    {
        m_Loader.Unload();
        m_Name.clear();
        m_Path.clear();
        m_pHeap = NULL;
    }
    // (inline) the file name, its path and its heap
    void Set(const sead::SafeStringBase<char>& name, sead::Heap* heap)
    {
        GetRegion();  // (the result is not used)
        m_Name.copy(name, -1);
        sead::StringFormatter formatter(&m_Path);
        formatter.append("Script/Talk/%s.umsbt").appendValue(name).flush();
        m_pHeap = heap;
    }
    // (inline) the message data of the language of the console (NULL: not loaded)
    const void* GetMessageData() const
    {
        const u8* data = static_cast<const u8*>(m_Loader.GetData());
        if (data == NULL) {
            return NULL;
        }
        s32 language = GetRegion();
        language = static_cast<u32>(language) < 10 ? s_RegionLanguages[language] : 0;
        return data + reinterpret_cast<const u32*>(data)[language * 2];
    }

    Loader m_Loader;                  // 0x000
    sead::FixedSafeString<32> m_Name; // 0x100
    sead::FixedSafeString<64> m_Path; // 0x12C
    sead::Heap* m_pHeap;              // 0x178

    // the language of the file per region (the files hold one message binary per language)
    static const u8 s_RegionLanguages[10];
};
ASSERT_SIZE(TalkFile, 0x17C);

// the message files of the talk: the current one and the ones that stay loaded (names are ours)
struct TalkFiles
{
    static const s32 FILE_NUM = 1;

    // (inline)
    TalkFiles() : m_pCurrent(&m_Main), m_Num(0) {}

    // the file name (a file that stays loaded, else the main file)
    void Select(const sead::SafeStringBase<char>& name, sead::Heap* heap); // 0x005E45E8
    // the files that stay loaded go on loading; true when all are done
    bool Load(); // 0x005E474C
    // a file that stays loaded (if there is room)
    void Add(const sead::SafeStringBase<char>& name, sead::Heap* heap); // 0x005E47A8
    // (inline) the main file is freed, or the current one becomes the main file
    void Release()
    {
        TalkFile* file = m_pCurrent;
        if (file != &m_Main) {
            m_pCurrent = &m_Main;
            return;
        }
        file->Clear();
    }
    DECOMP_EMIT_COPY TalkFiles* Get() { return this; }

    TalkFile* m_pCurrent;         // 0x000
    TalkFile m_Main;              // 0x004
    s32 m_Num;                    // 0x180
    TalkFile m_Files[FILE_NUM];   // 0x184
};
ASSERT_SIZE(TalkFiles, 0x300);

// The barcarolle of Kapp'n during a talk (names are ours): the phrases of the song (SoundBarcarollePlayer) lead the
// pages of the talk; a state machine over the phrases (PMF table 0x00892964). The game starts the song (isPlaying).
struct TalkSong
{
    enum State : u8
    {
        STATE_IDLE = 0,    // waits for the first phrase
        STATE_SING = 1,    // a phrase is sung
        STATE_NEXT = 2,    // the page of the next phrase
        STATE_REPEAT = 3,  // waits for the repeated phrase
        STATE_END = 4,
        STATE_NUM = 5,
    };
    typedef void (TalkSong::*StateFunc)();

    // (inline)
    explicit TalkSong(TalkSystem* pOwner)
        : pOwner(pOwner), isPlaying(0), state(STATE_IDLE), phrase(-1), curPhrase(-1), prevPhrase(-1), note(-1),
          prevNote(-1), noteNum(0), isBetween(0), isSinging(0), isActive(0), restNum(0), _unknown0x20(0), isWaiting(0)
    {
    }
    // (inline)
    ~TalkSong()
    {
        if (isPlaying) {
            SoundMgr::SDestroyBarcarolle();
            isPlaying = 0;
        }
    }

    // the state of the song is read, the state machine runs
    void Update(); // 0x005D5424
    // (the state functions)
    void UpdateIdle(); // 0x005D52E8
    void UpdateSing(); // 0x005D5314
    void UpdateNext(); // 0x005D5340
    void UpdateRepeat(); // 0x005D5274
    void UpdateEnd(); // 0x005D5270

    TalkSystem* pOwner;  // 0x00
    u8 isPlaying;        // 0x04
    u8 state;            // 0x05
    s16 phrase;          // 0x06 the phrase of the song (-1: none)
    s16 curPhrase;       // 0x08 the phrase of the talk (-1: none)
    s16 prevPhrase;      // 0x0A
    s32 note;            // 0x0C the note of the song
    s32 prevNote;        // 0x10
    s32 noteNum;         // 0x14 the frames of the current note (0: the song went back)
    u8 isBetween;        // 0x18 between two phrases
    u8 isSinging;        // 0x19
    u8 isActive;         // 0x1A
    s32 restNum;         // 0x1C the lines of the song still to come (one per note)
    u8 _unknown0x20;     // 0x20
    u8 isWaiting;        // 0x21 the wait icon of the talk stays until the next note (phrase 2: the next pause)

    static const StateFunc s_StateFuncs[STATE_NUM];
};
ASSERT_SIZE(TalkSong, 0x24);

// The controller of the conversations (script::Mgr +0x14; no RTTI, the name is ours): the message files, the
// renderers, the window, the measure, the sequence, the choices, the insert and the phrase of the talk messages, and a
// state machine with 9 states (PMF tables 0x00891FD0 enter / 0x00892018 update / 0x00892060 leave).
class TalkSystem
{
public:
    enum State : u8
    {
        STATE_IDLE = 0,
        STATE_LOAD = 1,      // the file of the message is loaded
        STATE_OPEN = 2,      // the window opens
        STATE_PAGE = 3,      // waits at the end of a page
        STATE_WRITE = 4,     // the text is written
        STATE_CHOICE = 5,
        STATE_CLOSE = 6,     // the window closes
        STATE_CLOSED = 7,    // the message is done, the window is closed
        STATE_KEEP = 8,      // the message is done, the window stays open
        STATE_NUM = 9,       // (m_NextState: none)
    };
    static const u32 FLAG_CHOICE = 1;
    static const u8 QUIT_NONE = 5;
    typedef void (TalkSystem::*StateFunc)();

    explicit TalkSystem(nn::fnd::DateTimeParameters* pDate); // 0x00604A2C
    ~TalkSystem(); // 0x00604EA0

    // the state machine
    void Update(); // 0x00604950
    // the talk starts with recept (or stays with it)
    void SetRecept(ITalkRecept* recept); // 0x00602C14
    // the recept is let go
    void ReleaseRecept(); // 0x00602CA0
    // the next message (closeAtEnd: the window closes when it ends)
    void Next(bool isCloseAtEnd); // 0x00603408
    // a closed message goes on with the jump
    bool Continue(); // 0x00603FD0
    // the flow continues at the label of file with the name (negative label: none)
    DECOMP_NOINLINE void Jump(s32 label, const char* file, const IWord* name, u8 mode); // 0x00603FEC
    // the text of the numbered label (label "%03d") of the current message file into the word; true: found
    bool GetTextByNumber(IWord* word, s32 number); // 0x0075DDF8
    // quits the message with mode (only while it is written or waits)
    void RequestQuit(u8 mode, bool isForced, bool isNext); // 0x0060440C
    void SetWaitIcon(u8 mode); // 0x006047C4
    void SetWaitIcon(u8 mode, s32 time); // 0x00602E20
    void HideWaitIcon(bool isKept); // 0x006029F0
    void SetTimer(s32 time, bool isAuto); // 0x006030D4
    void SetJumped(); // 0x00604934
    // the sounds of the window (TalkWin)
    void SetOpenSound(u8 sound); // 0x0060443C
    void SetCloseSound(u8 sound); // 0x006044A8
    // the choices
    void SetChoiceNum(s32 num, s32 cancel); // 0x006044BC
    void StopChoice(); // 0x006044E8
    void SetChoiceItem(s32 index, const ChoiceItemData* data); // 0x006044F8
    void StartChoice(s32 num, s32 cancel); // 0x00604528
    void StartChoice(); // 0x00604580
    // everything is freed (files, choices, window)
    void Release(); // 0x006045B4
    // back to the start (no message)
    void Reset(); // 0x006047D4
    DECOMP_EMIT_COPY TalkAttribute* GetAttribute() { return m_pAttribute; } // 0x006044B4
    DECOMP_EMIT_COPY ChoiceStandardWin* GetChoice() { return m_pChoice; } // 0x00604940
    DECOMP_EMIT_COPY TalkPhrase* GetPhrase() { return m_pPhrase; } // 0x00604948
    DECOMP_EMIT_COPY TalkFiles* GetFiles() { return &m_Files; } // 0x00603E7C

    // (the state functions)
    void EnterIdle(); // 0x006030E8
    void UpdateIdle(); // 0x00602E30
    void LeaveIdle(); // 0x00603554
    void EnterLoad(); // 0x006030EC
    void UpdateLoad(); // 0x00602E34
    void LeaveLoad(); // 0x00603558
    void EnterOpen(); // 0x00602B88
    void UpdateOpen(); // 0x00602B64
    void LeaveOpen(); // 0x00602D60
    void EnterPage(); // 0x00603D68
    void UpdatePage(); // 0x006039D4
    void LeavePage(); // 0x00603E78
    void EnterWrite(); // 0x00603D2C
    void UpdateWrite(); // 0x0060383C
    void LeaveWrite(); // 0x00603E74
    void EnterChoice(); // 0x00603CD4
    void UpdateChoice(); // 0x00603604
    void LeaveChoice(); // 0x00603E70
    void EnterClose(); // 0x00602D64
    void UpdateClose(); // 0x00602D04
    void LeaveClose(); // 0x006030D0
    void EnterClosed(); // 0x00603E84
    void UpdateClosed(); // 0x00603E2C
    void LeaveClosed(); // 0x00603FCC
    void EnterKeep(); // 0x0060355C
    void UpdateKeep(); // 0x00603464
    void LeaveKeep(); // 0x00603CD0

    // the message file is freed
    void ReleaseMessage(); // 0x0060296C
    // the text of the message into the renderers
    void SetupText(); // 0x00602A14
    // the text is reset (resetSeq: the sequence too)
    void ResetText(bool isResetSeq); // 0x00603560
    // the message is done: the next one, the window stays or closes
    void EndMessage(); // 0x00603E88
    // the message is quit: the window closes
    void Quit(); // 0x00604448
    void ClearJump(); // 0x00603DCC
    // the net play may hold the talk (the timer or an error)
    void CheckNet(u8 state); // 0x00604068
    void Finish(); // 0x0060342C
    // the state (copies in the shared area; names are ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsWaitIconActive() const // 0x0075DD68
    {
        return m_WaitIconTime != 0;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsIdle() const // 0x0075DD78
    {
        return m_State == STATE_IDLE;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsKept() const // 0x0075DD8C
    {
        return m_State == STATE_KEEP;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsPageWait() const // 0x0075DDB4
    {
        return m_State == STATE_PAGE;
    }
    // the talk is over: idle and the next message is the end (both are evaluated; name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsFinished() const // 0x0075E10C
    {
        return (m_State == STATE_IDLE) & m_pJump->IsEnd();
    }
    // the number of the label of the current message
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetMessageNumber() const // 0x0075DF98
    {
        return m_pMessage->m_Number;
    }
    // whether the message may be quit now
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool CanQuit(bool isForced) const // 0x0075DFA4
    {
        if (isForced) {
            return (m_State == STATE_WRITE || m_State == STATE_PAGE || m_State == STATE_KEEP) && m_Flags == 0;
        }
        return m_State == STATE_WRITE || m_State == STATE_PAGE || m_State == STATE_CHOICE || m_State == STATE_KEEP;
    }
    // (inline) whether the jump goes on to another message (not the end, or the end of another file)
    bool IsJumpPending() const
    {
        return !m_pJump->IsEnd() || (!m_pJump->m_File.isEmpty() &&
                                     m_pMessage->m_File.compare(m_pJump->m_File, sead::SafeStringBase<char>::MAX_LENGTH) != 0);
    }
    // (inline) the end of the page is forgotten
    void ClearPage();
    // (inline) the flags of the next message are reset
    void ClearMessageFlags()
    {
        m_IsInputLocked = 0;
        m_ChoiceKind = 5;
        m_IsChoice = 0;
        m_Flags &= ~FLAG_CHOICE;
    }

    nn::fnd::DateTimeParameters* m_pDate; // 0x000 the date of the talk (script::Mgr +0x10)
    u8 m_State;                   // 0x004
    u8 m_NextState;               // 0x005 (STATE_NUM: none)
    u8 m_QuitMode;                // 0x006 (QUIT_NONE: none)
    u8 m_IsQuitForced;            // 0x007
    u8 m_IsQuitNext;              // 0x008 after the quit, the jump goes on
    u32 m_Flags;                  // 0x00C
    s32 m_StateTime;              // 0x010 the frames in the state
    u8 m_WaitIconMode;            // 0x014 (0: none; 2, 3: the sounds of the icon)
    s32 m_WaitIconTime;           // 0x018
    s8 m_IsJumped;                // 0x01C a branch tag jumped
    u8 m_IsEnd;                   // 0x01D the talk ends after closing
    u8 m_IsTextStopped;           // 0x01E the sequence is not given more text
    u8 m_IsInputLocked;           // 0x01F
    u8 m_IsCloseAtEnd;            // 0x020
    u8 m_IsChoice;                // 0x021
    s32 m_ChoiceTime;             // 0x024
    u8 m_ChoiceKind;              // 0x028 (< 5)
    TalkFiles m_Files;            // 0x02C
    libms::Message m_Message;     // 0x32C
    TalkAttribute* m_pAttribute;  // 0x334
    RenderMain* m_pMain;          // 0x338
    RenderRuby* m_pRuby;          // 0x33C
    TalkWin* m_pWin;              // 0x340
    TalkMeasure* m_pMeasure;      // 0x344
    TalkSeq* m_pSeq;              // 0x348
    ChoiceStandardWin* m_pChoice; // 0x34C
    TalkInsert* m_pInsert;        // 0x350
    TalkPhrase* m_pPhrase;        // 0x354
    TalkState* m_pState;          // 0x358
    TalkLabel* m_pMessage;        // 0x35C the current message
    TalkLabel* m_pJump;           // 0x360 the next message
    ITalkRecept* m_pRecept;       // 0x364
    s8 m_IsAutoOnly;              // 0x368 (tags 7/8, 7/9) the page goes on by itself only
    s32 m_AutoTime;               // 0x36C the frames until the page goes on by itself (-1: none)
    s32 m_NetTime;                // 0x370 the frames the net play holds the page (-1: none)
    s32 m_NetTimeMax;             // 0x374 (-1: none)
    u8 m_IsNetAuto;               // 0x378
    u32 m_PageTime;               // 0x37C the frames at the end of the page
    u8 m_IsLoading;               // 0x380 the message file is being loaded
    TalkSong* m_pSong;            // 0x384

    static const StateFunc s_EnterFuncs[STATE_NUM];
    static const StateFunc s_UpdateFuncs[STATE_NUM];
    static const StateFunc s_LeaveFuncs[STATE_NUM];
    // the color of the name per name mode
    static const u8 s_NameColors[3];
    // the color of the window per window kind (TalkWin::m_WindowKind)
    static const u8 s_WindowColors[11];
    static const SeID s_OpenSounds[5];
    static const SeID s_CloseSounds[3];
};
ASSERT_SIZE(TalkSystem, 0x388);

inline TalkAttribute::TalkAttribute(TalkSystem* talk, s32 index) : pData(NULL), speaker(SPEAKER_NUM)
{
    pData = static_cast<const u8*>(talk->m_Message.GetAttribute(index));
}
} // namespace script
