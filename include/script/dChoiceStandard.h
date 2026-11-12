#pragma once

#include "decomp.h"
#include "script/dChoiceBase.h"
#include "script/dWordFix.h"

namespace script {
class IWord;
class Phrase;
class IRecept;
class SeqEngine;

// the data of a choice from a label of a message or a word (ChoiceItem::Set; names are ours)
struct ChoiceItemData
{
    static const u8 TYPE_WORD = 0;   // the text is pWord
    static const u8 TYPE_LABEL = 1;  // the text is the string number of label (StrMgr)

    u8 type;          // 0x00
    const IWord* pWord;  // 0x04 (TYPE_WORD)
    const char* label;   // 0x08 (TYPE_LABEL)
    s32 number;          // 0x0C (TYPE_LABEL)
    s32 value;           // 0x10 ChoiceItem::m_Value
    s32 sound;           // 0x14 ChoiceItem::m_Sound
    s32 data;            // 0x18 ChoiceItem::m_Data
    u8 kind;             // 0x1C ChoiceItem::m_Kind
    u8 isCapital;        // 0x1D the first character in upper case
    u8 isPeriod;         // 0x1E a "." is appended
    u8 isExclamation;    // 0x1F a "!" is appended

    // (game code; names are ours) no text / the text of the string number of label
    ChoiceItemData(); // 0x005E40AC
    void SetLabel(const char* label, s32 number, s32 value, s32 sound, s32 data, u8 kind); // 0x005E3F94
};
ASSERT_SIZE(ChoiceItemData, 0x20);

// the data of a choice from a label (ChoiceStandard::SetItems; names are ours)
struct ChoiceLabelData
{
    const char* label;  // 0x00 (NULL: no text)
    s32 number;         // 0x04 (negative: no text)
    s32 value;          // 0x08
    s32 sound;          // 0x0C
    u8 kind;            // 0x10
};
ASSERT_SIZE(ChoiceLabelData, 0x14);

// One choice: its text and what the game gets back when it is chosen (no RTTI; names are ours).
struct ChoiceItem
{
    // (inline; out of line for the arrays)
    DECOMP_EMIT_COPY DECOMP_NOINLINE ChoiceItem() : m_Value(-1), m_Sound(47), m_Kind(0), m_Data(0) {} // 0x005D59AC
    // (inline; out of line for the arrays)
    DECOMP_EMIT_COPY ~ChoiceItem() {} // 0x005D5A28

    // (inline) the empty choice
    void Clear()
    {
        m_Value = -1;
        m_Sound = 47;
        m_Word.Clear();
        m_Kind = 0;
        m_Data = 0;
    }
    // the text and the values of data
    void Set(const ChoiceItemData* data); // 0x005D57CC
    // the inserted words of the text are filled in (with the insert of phrase)
    void Insert(Phrase* phrase); // 0x005D5564
    // the tags of the text are processed (ChoiceSeq)
    void Process(Phrase* phrase, IRecept* recept, SeqEngine* seq); // 0x005D5634

    s32 m_Value;         // 0x00 the value the game gets (-1: none)
    s32 m_Sound;         // 0x04 (47 by default)
    WordFix<65> m_Word;  // 0x08 the text
    u8 m_Kind;           // 0xA4 (cancel item: 1)
    s32 m_Data;          // 0xA8

    static WordFix<65> s_ProcessBuffer;
};
ASSERT_SIZE(ChoiceItem, 0xAC);

// The choices of a message (the window or the list of the game derive from it): the items, an optional cancel
// choice, and a state machine with 5 states (PMF tables 0x00892800 enter / 0x00892828 update; names are ours).
// RTTI N6script14ChoiceStandardE @ 0x008D3894
// vtable 0x0090A1A4 (vptr 0x0090A1AC), offset_to_top 0, 15 entries
class ChoiceStandard : public ::script::ChoiceBase
{
public:
    enum State : u8
    {
        STATE_IDLE = 0,
        STATE_SETUP = 1,    // the texts are prepared, the derived class shows them
        STATE_SELECT = 2,
        STATE_DECIDE = 3,   // a choice was made; the derived class ends
        STATE_READY = 4,    // set up by the talk, waits for Begin
        STATE_NUM = 5,      // (m_NextState: none)
    };
    static const u8 CANCEL_NONE = 0;
    static const u8 CANCEL_INDEX = 2;  // m_CancelIndex is the cancel choice
    typedef void (ChoiceStandard::*StateFunc)();

    explicit ChoiceStandard(s32 num); // 0x005E56A4
    virtual ~ChoiceStandard() = 0; // slot 0x00 (pure)
    // slot 0x04 (deleting dtor)
    // the choices start with the phrase, the recept and the sequence of the message
    virtual void Begin(Phrase* phrase, IRecept* recept, SeqEngine* seq); // 0x005E5328 slot 0x08
    virtual bool IsIdle() const; // 0x0075C024 slot 0x0C
    virtual void Update() = 0; // slot 0x10
    // the item that was chosen (the cancel item when canceled)
    virtual const ChoiceItem* GetChosenItem(); // 0x0075C06C slot 0x14
    // the index of the choice under the cursor
    virtual s32 GetCursor() const = 0; // slot 0x18
    // whether a choice was made (not ended by RequestEnd)
    virtual bool IsChosen() const; // 0x0075C038 slot 0x1C
    // whether the choice was canceled (by the derived class)
    virtual bool IsCancelPressed() const = 0; // slot 0x20
    virtual s32 GetCancelValue() const = 0; // slot 0x24
    // asks to end (while setting up or selecting); true when it ends
    virtual bool RequestEnd(); // 0x005E533C slot 0x28
    virtual void SetHold(bool isHold); // 0x005E5320 slot 0x2C
    virtual void OnSetup(); // 0x005E56A0 slot 0x30
    virtual void OnDecide(); // 0x005E5090 slot 0x34
    // whether the derived class is done with the current state
    virtual bool IsReady() const; // 0x0075C064 slot 0x38

    // (the state functions)
    void EnterIdle(); // 0x005E52EC
    void UpdateIdle(); // 0x005E52E8
    void EnterSetup(); // 0x005E50BC
    void UpdateSetup(); // 0x005E5094
    void EnterSelect(); // 0x005E5310
    void UpdateSelect(); // 0x005E52F0
    void EnterDecide(); // 0x005E52CC
    void UpdateDecide(); // 0x005E5294

    // the texts of the items are filled in and processed
    void SetupTexts(); // 0x005E5120
    void RunStateMachine(); // 0x005E55B8
    // the cursor choice is made
    void Decide(); // 0x005E5314
    // the items from labels
    void SetItems(const ChoiceLabelData* data, s32 num, s32 cancel); // 0x005E5358
    DECOMP_EMIT_COPY void SetNum(s32 num, s32 cancel) // 0x005E5430
    {
        m_Num = num;
        m_State = STATE_IDLE;
        if (cancel >= 0 && num >= cancel) {
            m_CancelIndex = cancel;
            m_CancelMode = CANCEL_INDEX;
        } else {
            m_CancelMode = CANCEL_NONE;
        }
    }
    DECOMP_EMIT_COPY void Stop() { m_State = STATE_IDLE; } // 0x005E5458
    DECOMP_EMIT_COPY void SetItem(s32 index, const ChoiceItemData* data) // 0x005E5464
    {
        if (index >= 0 && m_ItemNum > index) {
            m_pItems[index].Set(data);
        }
    }
    DECOMP_EMIT_COPY void Start(s32 num, s32 cancel) // 0x005E5490
    {
        Reset();
        m_State = STATE_READY;
        m_Num = num;
        if (cancel >= 0 && num >= cancel) {
            m_CancelIndex = cancel;
            m_CancelMode = CANCEL_INDEX;
        } else {
            m_CancelMode = CANCEL_NONE;
        }
        m_IsSet = 1;
    }
    DECOMP_EMIT_COPY void Start() // 0x005E54D4
    {
        Reset();
        m_State = STATE_READY;
        m_IsSet = 1;
    }
    void Reset(); // 0x005E54F8
    DECOMP_EMIT_COPY ChoiceItem* GetItem(s32 index) // 0x005E5674
    {
        if (index >= 0 && m_Num > index) {
            return &m_pItems[index];
        }
        return NULL;
    }
    // (no caller in the binary)
    void Unused5E54F4(); // 0x005E54F4
    bool Unused5E55AC(); // 0x005E55AC
    void Unused5E55B4(); // 0x005E55B4

    DECOMP_EMIT_COPY s32 GetCancelIndex() const { return m_CancelIndex; } // 0x0075C040
    DECOMP_EMIT_COPY u8 GetKind() const { return m_Kind; } // 0x0075C048
    DECOMP_EMIT_COPY bool IsSelecting() const { return m_State == STATE_SELECT; } // 0x0075C050
    DECOMP_EMIT_COPY bool IsHold() const { return m_IsHold; } // 0x0075C158
    DECOMP_EMIT_COPY bool IsEndRequested() const { return m_IsEndRequested; } // 0x0075C160
    DECOMP_EMIT_COPY u8 IsFixed() const { return m_IsFixed; } // 0x0075C168
    DECOMP_EMIT_COPY s32 GetNum() const { return m_Num; } // 0x0075C170
    DECOMP_EMIT_COPY ChoiceItem* GetItems() const { return m_pItems; } // 0x0075C21C

    u8 m_State;              // 0x004
    u8 m_NextState;          // 0x005 (STATE_NUM: none)
    u8 m_IsFixed;            // 0x006 the choices of the tag 8/10 (fixed texts)
    s32 m_Num;               // 0x008
    ChoiceItem* m_pItems;    // 0x00C
    s32 m_ItemNum;           // 0x010 the size of m_pItems
    ChoiceItem m_CancelItem; // 0x014
    s32 m_CancelIndex;       // 0x0C0
    u8 m_CancelMode;         // 0x0C4
    u8 m_Kind;               // 0x0C5 (5 by default)
    bool m_IsChosen;         // 0x0C6
    bool m_IsEndRequested;   // 0x0C7
    bool m_IsHold;           // 0x0C8 the choice does not end yet
    u8 m_IsSet;              // 0x0C9
    Phrase* m_pPhrase;       // 0x0CC
    IRecept* m_pRecept;      // 0x0D0
    SeqEngine* m_pSeq;       // 0x0D4

    // the sound of a button per kind of choice
    static const u32 s_KindSounds[11];
    static const StateFunc s_EnterFuncs[STATE_NUM];
    static const StateFunc s_UpdateFuncs[STATE_NUM];
};
ASSERT_SIZE(ChoiceStandard, 0xD8);

// (inline)
inline ChoiceStandard::~ChoiceStandard()
{
    delete[] m_pItems;
}
} // namespace script
