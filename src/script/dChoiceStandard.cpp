#include "script/dChoiceStandard.h"
#include "script/dCapitalTopEngine.h"
#include "script/dInsertEngine.h"
#include "script/dPhrase.h"
#include "script/dStrMgr.h"
#include "script/dWordRes.h"
#include "sead/seadNew.h"

namespace script {
// 0x005D57CC (name is ours)
void ChoiceItem::Set(const ChoiceItemData* data)
{
    bool isWord = data->type == ChoiceItemData::TYPE_WORD;
    bool isLabel = data->type == ChoiceItemData::TYPE_LABEL;
    IWord& word = m_Word;
    m_Value = data->value;
    m_Sound = data->sound;
    m_Kind = data->kind;
    m_Data = data->data;
    if (isWord) {
        if (data->pWord != NULL) {
            word.Copy(*data->pWord);
        } else {
            word.Clear();
        }
    } else if (isLabel) {
        if (data->label == NULL || data->number < 0 || !StrMgr::s_pInstance->GetString(&word, data->label, data->number)) {
            word.Clear();
        }
    }
    if (data->isCapital) {
        CapitalTopEngine capital;
        char16* text = word.GetWord();
        if (text != NULL && word.GetSize() != 0 && text[0] != 0) {
            capital.SetText(text);
            capital.Run(&MsgEngineModify::s_pNoStop);
        }
    }
    if (data->isPeriod) {
        word.Append(WordRes(u"."));
    }
    if (data->isExclamation) {
        word.Append(WordRes(u"!"));
    }
}

// 0x005D5564 (name is ours)
void ChoiceItem::Insert(Phrase* phrase)
{
    IWord& word = m_Word;
    const char16* text = word.GetWord();
    if (text != NULL && word.GetSize() != 0 && text[0] != 0) {
        InsertEngine insert(phrase->m_pInsert, false);
        text = word.GetWord();
        if (insert.m_pInsert != NULL && text != NULL) {
            insert.Run(text);
        }
    }
}

// 0x005E56A4 (name is ours)
ChoiceStandard::ChoiceStandard(s32 num)
    : m_State(STATE_IDLE), m_NextState(STATE_NUM), m_IsFixed(0), m_Num(0), m_pItems(new (std::nothrow) ChoiceItem[num]),
      m_ItemNum(num), m_CancelIndex(-1), m_CancelMode(CANCEL_NONE), m_Kind(5), m_IsChosen(false),
      m_IsEndRequested(false), m_IsHold(false), m_IsSet(0), m_pPhrase(NULL), m_pRecept(NULL), m_pSeq(NULL)
{
}

// 0x005E5328 slot 0x08 (name is ours)
void ChoiceStandard::Begin(Phrase* phrase, IRecept* recept, SeqEngine* seq)
{
    m_pPhrase = phrase;
    m_pRecept = recept;
    m_pSeq = seq;
    m_NextState = STATE_SETUP;
}

// 0x0075C024 slot 0x0C (name is ours)
bool ChoiceStandard::IsIdle() const
{
    return m_State == STATE_IDLE;
}

// 0x0075C06C slot 0x14 (name is ours)
const ChoiceItem* ChoiceStandard::GetChosenItem()
{
    // 0x00AD67C8 (guard 0x0095EF2C)
    static ChoiceItem s_EmptyItem;
    if (IsCancelPressed()) {
        const ChoiceItem* item;
        if (m_CancelMode == CANCEL_INDEX) {
            item = m_CancelIndex >= 0 && m_ItemNum > m_CancelIndex ? &m_pItems[m_CancelIndex] : &s_EmptyItem;
        } else {
            item = &m_CancelItem;
        }
        if (item != NULL) {
            return item;
        }
    }
    s32 cursor = GetCursor();
    if (cursor >= 0 && m_Num > cursor) {
        return &m_pItems[cursor];
    }
    return m_pItems;
}

// 0x0075C038 slot 0x1C (name is ours)
bool ChoiceStandard::IsChosen() const
{
    return m_IsChosen;
}

// 0x005E533C slot 0x28 (name is ours)
bool ChoiceStandard::RequestEnd()
{
    if (m_State == STATE_SELECT || m_State == STATE_SETUP) {
        m_IsEndRequested = true;
    }
    return m_IsEndRequested;
}

// 0x005E5320 slot 0x2C (name is ours)
void ChoiceStandard::SetHold(bool isHold)
{
    m_IsHold = isHold;
}

// 0x005E56A0 slot 0x30 (name is ours)
void ChoiceStandard::OnSetup()
{
    // empty (in the original too)
}

// 0x005E5090 slot 0x34 (name is ours)
void ChoiceStandard::OnDecide()
{
    // empty (in the original too)
}

// 0x0075C064 slot 0x38 (name is ours)
bool ChoiceStandard::IsReady() const
{
    return true;
}

// 0x005E52EC (name is ours)
void ChoiceStandard::EnterIdle()
{
    // empty (in the original too)
}

// 0x005E52E8 (name is ours)
void ChoiceStandard::UpdateIdle()
{
    // empty (in the original too)
}

// 0x005E50BC (name is ours)
void ChoiceStandard::EnterSetup()
{
    if (m_CancelMode == CANCEL_INDEX) {
        ChoiceItem* item = GetItem(m_CancelIndex);
        if (item != NULL && item->m_Kind == 0) {
            item->m_Kind = 1;
        }
    }
    SetupTexts();
    OnSetup();
}

// 0x005E5094 (name is ours)
void ChoiceStandard::UpdateSetup()
{
    if (IsReady()) {
        m_NextState = STATE_SELECT;
    }
}

// 0x005E5310 (name is ours)
void ChoiceStandard::EnterSelect()
{
    // empty (in the original too)
}

// 0x005E52F0 (name is ours)
void ChoiceStandard::UpdateSelect()
{
    if (!m_IsHold && m_IsEndRequested) {
        m_NextState = STATE_DECIDE;
    }
}

// 0x005E52CC (name is ours)
void ChoiceStandard::EnterDecide()
{
    if (!m_IsEndRequested) {
        m_IsChosen = true;
    }
    OnDecide();
}

// 0x005E5294 (name is ours)
void ChoiceStandard::UpdateDecide()
{
    if (IsReady()) {
        m_pPhrase = NULL;
        m_pRecept = NULL;
        m_pSeq = NULL;
        m_NextState = STATE_IDLE;
    }
}

// 0x005E5120 (name is ours)
void ChoiceStandard::SetupTexts()
{
    if (m_pPhrase == NULL) {
        return;
    }
    if (m_IsSet) {
        for (s32 i = 0; i < m_Num; i++) {
            m_pItems[i].Insert(m_pPhrase);
        }
        m_CancelItem.Insert(m_pPhrase);
    }
    for (s32 i = 0; i < m_Num; i++) {
        m_pItems[i].Process(m_pPhrase, m_pRecept, m_pSeq);
    }
    m_CancelItem.Process(m_pPhrase, m_pRecept, m_pSeq);
}

// 0x005E55B8 (name is ours)
void ChoiceStandard::RunStateMachine()
{
    StateFunc update = s_UpdateFuncs[m_State];
    if (update != NULL) {
        (this->*update)();
    }
    if (m_NextState != STATE_NUM) {
        u8 state = m_NextState;
        m_State = state;
        m_NextState = STATE_NUM;
        StateFunc enter = s_EnterFuncs[state];
        if (enter != NULL) {
            (this->*enter)();
        }
    }
}

// 0x005E5314 (name is ours)
void ChoiceStandard::Decide()
{
    m_NextState = STATE_DECIDE;
}

// 0x005E5358 (name is ours)
void ChoiceStandard::SetItems(const ChoiceLabelData* data, s32 num, s32 cancel)
{
    Reset();
    for (s32 i = 0; i < num; i++) {
        ChoiceItem* item = &m_pItems[i];
        const ChoiceLabelData* src = &data[i];
        if (src->label == NULL || src->number < 0 ||
            !StrMgr::s_pInstance->GetString(&item->m_Word, src->label, src->number)) {
            static_cast<IWord&>(item->m_Word).Clear();
        }
        item->m_Value = src->value;
        item->m_Sound = src->sound;
        item->m_Kind = src->kind;
        item->m_Data = 0;
    }
    m_Num = num;
    if (cancel >= 0 && num > cancel) {
        m_CancelIndex = cancel;
        m_CancelMode = CANCEL_INDEX;
    } else {
        m_CancelMode = CANCEL_NONE;
    }
    m_IsSet = 1;
}

// 0x005E54F4 (name is ours)
void ChoiceStandard::Unused5E54F4()
{
    // empty (in the original too)
}

// 0x005E54F8 (name is ours)
void ChoiceStandard::Reset()
{
    m_State = STATE_IDLE;
    m_NextState = STATE_NUM;
    m_IsFixed = 0;
    m_Num = 0;
    for (s32 i = 0; i < m_ItemNum; i++) {
        ChoiceItem* item = &m_pItems[i];
        item->m_Value = -1;
        item->m_Sound = 47;
        static_cast<IWord&>(item->m_Word).Clear();
        item->m_Kind = 0;
        item->m_Data = 0;
    }
    m_CancelItem.m_Value = -1;
    m_CancelItem.m_Sound = 47;
    static_cast<IWord&>(m_CancelItem.m_Word).Clear();
    m_CancelItem.m_Kind = 0;
    m_CancelItem.m_Data = 0;
    m_CancelIndex = -1;
    m_CancelMode = CANCEL_NONE;
    m_Kind = 5;
    m_IsChosen = false;
    m_IsEndRequested = false;
    m_IsHold = false;
    m_IsSet = 0;
}

// 0x005E55AC (name is ours)
bool ChoiceStandard::Unused5E55AC()
{
    return true;
}

// 0x005E55B4 (name is ours)
void ChoiceStandard::Unused5E55B4()
{
    // empty (in the original too)
}
} // namespace script
