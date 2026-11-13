#include "script/dTalkSeq.h"
#include "script/dChoiceStandardWin.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Other/dRegion.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "event/event_Extern.h"
#include "nn/nstd/nstd_String.h"
#include "script/dChip.h"
#include "script/dITalkRecept.h"
#include "script/dInsert.h"
#include "script/dKappeiNoteEngine.h"
#include "script/dRenderBase.h"
#include "script/dRenderTag.h"
#include "script/dScriptExtern.h"
#include "script/dTalkMeasure.h"
#include "script/dTalkWin.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"

namespace script {
// 0x005FC7F0
TalkSeq::TalkSeq(TalkSystem* pOwner, RenderBase* pMain, RenderBase* pRuby, TalkWin* pWin, TalkMeasure* pMeasure)
    : m_pOwner(pOwner), m_pMain(pMain), m_pRuby(pRuby), m_pWin(pWin), m_pMeasure(pMeasure), m_LineMax(3), m_LineNum(0),
      m_PauseCount(0), m_Delay(0), m_IsFastRequested(0), m_IsFastLocked(0), m_IsInstant(0), m_IsCenterPending(0),
      m_IsStylePending(0), m_HasText(0), m_Scale(100), m_Color(0), m_State(STATE_NONE), m_RubyText(pOwner, pMain),
      m_Ruby(pOwner, pRuby), m_Song(this), m_RandomBranch(pOwner, this)
{
    m_Timer.count = 0;
    m_Timer.time = 0.0f;
    m_Timer.speed = 1.0f;
    m_Timer.waitNormal = 0.0f;
    m_Timer.waitFast = 0.0f;
    m_Timer.override = -1.0f;
    // the speed of the text by region
    u32 region = GetRegion();
    if (region >= 10) {
        region = 0;
    }
    f32 waitFast = s_WaitsFast[region];
    f32 waitNormal = s_WaitsNormal[region];
    m_Timer.waitFast = waitFast;
    m_Timer.waitNormal = waitNormal;
    m_Timer.override = -1.0f;
    m_RubyText.m_Timer.waitFast = waitFast;
    m_RubyText.m_Timer.waitNormal = waitNormal;
    m_RubyText.m_Timer.override = -1.0f;
    m_Ruby.m_Timer.waitFast = waitFast;
    m_Ruby.m_Timer.waitNormal = waitNormal;
    m_Ruby.m_Timer.override = -1.0f;
}

// 0x005FCA44 slot 0x00
// 0x005FC9F8 (deleting dtor)
TalkSeq::~TalkSeq()
{
    // (only the members are destroyed)
}

// 0x005F8030 slot 0x14
void TalkSeq::OnChar(const CharInfo* info)
{
    char16 c;
    if (ProcessCapital(&c, info)) {
        m_pMain->Put(c);
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
    } else {
        m_pMain->Put(info);
        if (info->pText != NULL) {
            char16 ch = *info->pText;
            if (ch != ' ' && ch != 0x3000 && ch != '\n') {
                m_Frame.lastChar = ch;
            }
        }
    }
    bool isNewLine = info->units == '\n';
    bool isSpace = info->units == ' ' || info->units == 0x3000;
    if (isNewLine) {
        m_pRuby->Put(info);
        m_IsCenterPending = 1;
        m_LineNum++;
        if (m_LineMax <= m_LineNum) {
            m_State = STATE_FULL;
        }
    }
    if (!(isNewLine || isSpace)) {
        m_Timer.count++;
        m_HasText = 1;
    }
    // the Japanese particle は sounds as わ before a punctuation mark or a space
    u32 sound = static_cast<u16>(info->units);
    if (info->units == 0x306F) {
        char16 next = *m_pCur;
        for (u32 i = 0; i < 14; i++) {
            if (s_WaParticleEnds[i] == next) {
                sound = 0x308F;
                break;
            }
        }
    }
    TalkState* state = m_pOwner->m_pState;
    const char16* cur = m_pCur;
    const char16* end = m_pEnd;
    if (state->pCharFunc != NULL) {
        (state->*state->pCharFunc)(sound, &cur, &end);
    }
    QueueTalkSpeech(m_pOwner->m_pState, sound, m_Scale);
}

// 0x005FC364 slot 0x0C
void TalkSeq::End()
{
    m_State = STATE_END;
}

// 0x005FC370 slot 0x18
void TalkSeq::OnTag(const Tag* tag)
{
    if (static_cast<u32>(tag->group) < 21) {
        TagFunc func = s_GroupFuncs[tag->group];
        (this->*func)(tag);
    } else if (tag->group == 0xFF && static_cast<u32>(tag->index) < 7) {
        TagFunc func = s_GenericFuncs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FC408 slot 0x08
void TalkSeq::Begin()
{
    m_State = STATE_WRITE;
    m_LineNum = 0;
    u8 kind = m_pOwner->m_pAttribute->GetSpeaker();
    s32 lineMax = 3;
    if (kind < TalkAttribute::SPEAKER_NUM) {
        u8 window = s_SpeakerWindows[kind];
        if (window == 9 || window == 10) {
            lineMax = 2;
        }
    }
    m_LineMax = lineMax;
    m_Timer.count = 0;
    m_Timer.time = 0.0f;
    m_Timer.override = -1.0f;
    m_PauseCount = 0;
    m_Delay = 0;
    m_IsFastRequested = 0;
    m_IsFastLocked = 0;
    m_IsInstant = 0;
    m_IsCenterPending = 1;
    m_IsStylePending = 1;
    m_HasText = 0;
    m_Scale = 100;
    m_Color = 0;
    m_pMain->Clear();
    m_pRuby->Clear();
    m_Song.isDone = 0;
    m_Song.isStopped = 0;
    m_pOwner->m_pState->flags &= ~TalkState::FLAG_TAG7_2;
    m_pOwner->m_pState->flags &= ~TalkState::FLAG_TAG0_3;
    m_Frame.pBegin = m_pCur;
}

// 0x005FC504 slot 0x10
bool TalkSeq::IsContinue()
{
    bool result = false;
    u8 state = m_State;
    if (state == STATE_FULL || state == STATE_HOLD) {
        m_Timer.time = 0.0f;
        m_Timer.count = 0;
        m_PauseCount = 0;
        if (state == STATE_HOLD) {
            m_State = STATE_WRITE;
        }
    } else if (state == STATE_STOP) {
        m_Timer.count = 0;
        m_Timer.time = 0.0f;
    } else if (state == STATE_WRITE) {
        if (m_IsCenterPending) {
            m_IsCenterPending = 0;
            const char16* cur = m_pCur;
            u8 kind = m_pOwner->m_pAttribute->GetSpeaker();
            if (kind < TalkAttribute::SPEAKER_NUM && s_SpeakerCenter[kind] == 1) {
                s_CursorTagText[4] = static_cast<s32>(m_pMeasure->MeasureCenterOffset(&cur, m_Scale) * 32.0f);
                const char16* text = s_CursorTagText;
                // 0x00AD6730 (guard 0x0095EF18)
                static Tag s_CenterTag = Tag::GetTag(&text);
                m_pMain->Put(&s_CenterTag);
            }
        }
        if (m_IsStylePending) {
            m_IsStylePending = 0;
            if (m_Scale != 100) {
                s_ScaleTagText[4] = m_Scale;
                const char16* text = s_ScaleTagText;
                PushText(&text, &s_pNoStop);
            }
            if (m_Color != 0) {
                const char16* text = m_Color < 7 ? s_ColorTags[m_Color] : s_DefaultColorTag;
                PushText(&text, &s_pNoStop);
            }
        }
        if (Advance()) {
            result = true;
        }
    }
    return result;
}

// 0x005FC6F0
void TalkSeq::Reset()
{
    SeqEngine::Reset();
    m_LineMax = 3;
    m_LineNum = 0;
    m_Timer.count = 0;
    m_Timer.time = 0.0f;
    m_IsCenterPending = 1;
    m_IsStylePending = 1;
    m_HasText = 0;
    m_RubyText.Reset();
    m_Ruby.Reset();
    m_pMain->Clear();
    m_pRuby->Clear();
}

namespace {
// (inline) a ruby goes on at the time of TalkSeq (from the first character on)
DECOMP_ALWAYS_INLINE void ContinueRuby(TalkRuby* ruby, const TalkTimer& timer, bool isFast)
{
    if (ruby->m_CharNum == 0) {
        ruby->m_Timer.count = timer.count;
        ruby->m_Timer.time = timer.time;
        ruby->m_CharNum = 1;
    }
    ruby->m_IsFast = isFast;
    ruby->SetText2(&ruby->m_pStop);
}
} // namespace

// 0x005F9980
bool TalkSeq::Advance()
{
    u8 kind = m_pOwner->m_pAttribute->GetSpeaker();
    u8 instant = kind < TalkAttribute::SPEAKER_NUM ? s_SpeakerInstant[kind] : 0;
    bool isInstant;
    bool isFast;
    if (m_IsInstant || instant == 1) {
        isInstant = true;
        isFast = false;
    } else if (m_Song.isDone || m_Song.isStopped) {
        isInstant = true;
        isFast = false;
    } else {
        isInstant = false;
        isFast = (!m_IsFastLocked && instant == 0) ? m_IsFastRequested != 0 : false;
    }
    bool isDue = true;
    if (m_Delay > 0) {
        m_Delay--;
        isDue = false;
    }
    if (isInstant) {
        // the whole text at once
        m_PauseCount = 0;
        m_Timer.count = 0;
        m_Timer.time = 0.0f;
        while (!m_RubyText.m_IsDone || !m_Ruby.m_IsDone) {
            ContinueRuby(&m_RubyText, m_Timer, isFast);
            ContinueRuby(&m_Ruby, m_Timer, isFast);
            if (m_RubyText.m_IsDone && m_Ruby.m_IsDone) {
                m_Timer.count = m_RubyText.m_Timer.count;
                m_Timer.time = m_RubyText.m_Timer.time;
            }
        }
        return isDue;
    }
    if (!isDue) {
        return false;
    }
    if (isFast) {
        m_PauseCount = 0;
    } else if (m_PauseCount > 0) {
        s32 count = m_PauseCount - 2;
        m_PauseCount = count;
        if (count <= 0) {
            count = 0;
        }
        m_PauseCount = count;
        return false;
    }
    if (m_RubyText.m_IsDone && m_Ruby.m_IsDone) {
        return m_Timer.Step(isFast);
    }
    ContinueRuby(&m_RubyText, m_Timer, isFast);
    ContinueRuby(&m_Ruby, m_Timer, isFast);
    if (m_RubyText.m_IsDone && m_Ruby.m_IsDone) {
        m_Timer.count = m_RubyText.m_Timer.count;
        m_Timer.time = m_RubyText.m_Timer.time;
    }
    m_HasText = 1;
    return false;
}

// 0x005F8674
void TalkSeq::SetChoice8(const Tag* tag, u32 num, u32 hasCancel)
{
    ChoiceStandardWin* choice = m_pOwner->m_pChoice;
    u8 kind = m_pOwner->m_ChoiceKind;
    choice->Reset();
    u8 a = 0;
    u8 b = 1;
    if (kind < 5) {
        a = s_ChoiceKindsA[kind];
        b = s_ChoiceKindsB[kind];
    }
    s32 length = ReadChoices8(tag, choice->m_pItems, hasCancel ? &choice->m_CancelIndex : NULL, num, a, b);
    choice->m_Num = num;
    choice->m_CancelMode = hasCancel ? 2 : 0;
    choice->m_Kind = kind;
    StartChoice(length);
}

// 0x005FAA58
void TalkSeq::SetChoice15(const Tag* tag, u32 num, u32 hasCancel)
{
    ChoiceStandardWin* choice = m_pOwner->m_pChoice;
    u8 kind = m_pOwner->m_ChoiceKind;
    choice->Reset();
    u8 a = 0;
    u8 b = 1;
    if (kind < 5) {
        a = s_ChoiceKindsA[kind];
        b = s_ChoiceKindsB[kind];
    }
    s32 length = ReadChoices15(tag, choice->m_pItems, hasCancel ? &choice->m_CancelIndex : NULL, num, a, b);
    choice->m_Num = num;
    choice->m_CancelMode = hasCancel ? 2 : 0;
    choice->m_Kind = kind;
    StartChoice(length);
}

// 0x006027D0
void TalkSeq::Song::Process(const Tag* tag, bool isFast)
{
    TalkSeq* seq = pSeq;
    TalkSong* song = seq->m_pOwner->m_pSong;
    if (!isActive) {
        isActive = 1;
        lineNum++;
    }
    if (song->restNum > 1 || isStopped) {
        seq->m_Timer.override = 1.0f;
        isDone = 1;
        if (song->restNum > 0) {
            song->restNum--;
        }
        isActive = 0;
        return;
    }
    if (song->restNum != 1) {
        seq->HoldAtTag(tag);
        seq->m_Timer.override = 1.0f;
        isDone = 1;
        return;
    }
    // the notes of the line set the speed
    KappeiNoteEngine engine;
    engine.Run(seq->m_pCur);
    s32 noteNum = song->isPlaying ? song->noteNum : 0;
    if (engine.m_Count > 0 && noteNum > 0 && !isFast) {
        seq->m_Timer.override = static_cast<f32>(engine.m_Count) / static_cast<f32>(noteNum);
        isDone = 0;
    } else {
        seq->m_Timer.override = 1.0f;
        isDone = 1;
    }
    if (song->restNum > 0) {
        song->restNum--;
    }
    isActive = 0;
}

// 0x005F7ED8
void TalkSeq::RandomBranch::Branch(const Tag* tag)
{
    u32 index = tag->index;
    if (index >= 7) {
        return;
    }
    u32 num = s_LabelNums[index];
    u16 labels[10];
    GetTagArgs(tag, labels, num);
    const sead::SafeStringBase<char>& message = pOwner->m_pMessage->m_File;
    Entry& entry = entries[index];
    s32 label;
    for (s32 i = 0; i < 5; i++) {
        label = labels[GetRandom(num)];
        if (!message.isEqual(entry.message) || entry.label != label) {
            break;
        }
    }
    entry.message.copy(message, -1);
    entry.label = label;
    pSeq->Branch(label);
}

// the tag handlers

// 0x005FA37C
void TalkSeq::TagRuby(const Tag* tag)
{
    // the parameters: the size of the main text and of the ruby in bytes, the ruby; the main text follows the tag
    const u16* param = reinterpret_cast<const u16*>(tag->args);
    const char16* rubyEnd = NULL;
    const char16* rubyBegin = NULL;
    const char16* textEnd = NULL;
    const char16* text = NULL;
    text = tag->data + (tag->length & ~1u) / sizeof(char16);
    textEnd = text;
    textEnd = reinterpret_cast<const char16*>(reinterpret_cast<const u8*>(text) + (param[0] & ~1u));
    rubyBegin = reinterpret_cast<const char16*>(param + 2);
    rubyEnd = rubyBegin;
    rubyEnd = reinterpret_cast<const char16*>(reinterpret_cast<const u8*>(rubyBegin) + (param[1] & ~1u));
    s32 textLength = textEnd - text;
    s32 rubyLength = rubyEnd - rubyBegin;
    f32 speed = 1.0f;
    if (textLength > 0 && rubyLength > 0) {
        speed = static_cast<f32>(rubyLength) / static_cast<f32>(textLength);
    }
    // the main text and the ruby are written side by side, the ruby at the speed of the main text
    m_RubyText.SetText(&text, &s_pNoStop);
    m_RubyText.m_pStop = textEnd;
    m_RubyText.m_Mode = TalkRuby::MODE_TEXT;
    m_RubyText.m_IsDone = 0;
    m_RubyText.m_IsFast = 0;
    m_RubyText.m_CharNum = 0;
    m_RubyText.m_Timer.speed = 1.0f;
    m_Ruby.SetText(&rubyBegin, &s_pNoStop);
    m_Ruby.m_pStop = rubyEnd;
    m_Ruby.m_Mode = TalkRuby::MODE_RUBY;
    m_Ruby.m_IsDone = 0;
    m_Ruby.m_IsFast = 0;
    m_Ruby.m_CharNum = 0;
    m_Ruby.m_Timer.speed = speed;
    // the space for the wider of the two
    f32 textWidth = Chip::MeasureWidth(m_pWin->GetTextBox(), &text, &textEnd);
    f32 rubyWidth = 0.0f;
    if (m_pWin->m_Window.m_pRubyTextBox != NULL) {
        rubyWidth = Chip::MeasureWidth(m_pWin->m_Window.m_pRubyTextBox, &rubyBegin, &rubyEnd);
    }
    Tag store = GetStoreTag(textWidth);
    m_pMain->Put(&store);
    Tag rubyStore = GetStoreTag(rubyWidth);
    m_pRuby->Put(&rubyStore);
    m_pCur = textEnd;
}

// 0x005FA378
void TalkSeq::TagFont(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FA594
void TalkSeq::TagScale(const Tag* tag)
{
    m_pMain->Put(tag);
    m_Scale = *reinterpret_cast<const u16*>(tag->args);
}

// 0x005FA9BC
void TalkSeq::TagSetColor(const Tag* tag)
{
    u16 color = *reinterpret_cast<const u16*>(tag->args);
    TalkState* state = m_pOwner->m_pState;
    u16 value = 0xFFFF;
    if (color < 7) {
        value = color;
        if (color == 5) {
            state->flags |= TalkState::FLAG_TAG0_3;
        } else {
            state->flags &= ~TalkState::FLAG_TAG0_3;
        }
    } else {
        state->flags &= ~TalkState::FLAG_TAG0_3;
    }
    m_Color = value;
    m_pMain->Put(tag);
    m_pRuby->Put(tag);
}

// 0x005FB8D0
void TalkSeq::TagPageBreak(const Tag* tag)
{
    m_State = 2;
}

// 0x005F92A8
void TalkSeq::OnTagGroup0(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group0Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F8314
void TalkSeq::Tag1(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FAB54
void TalkSeq::Tag2(const Tag* tag)
{
    if (!m_pOwner->m_pRecept->ProcessTag2(tag)) {
        HoldAtTag(tag);
    }
}

// 0x005F9C98
void TalkSeq::Tag3(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (static_cast<u32>(tag->index) < 47 && !recept->ProcessTag3(tag)) {
        HoldAtTag(tag);
    }
}

// 0x005FA7F8
void TalkSeq::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005FACE8
void TalkSeq::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005F8EEC
void TalkSeq::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005FA7C0
void TalkSeq::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005FA1AC
void TalkSeq::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005FA1E4
void TalkSeq::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005F9688
void TalkSeq::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F96D4
void TalkSeq::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F9720
void TalkSeq::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F976C
void TalkSeq::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F97B8
void TalkSeq::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F9804
void TalkSeq::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F9850
void TalkSeq::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FA87C
void TalkSeq::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005FAD20
void TalkSeq::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005FA21C
void TalkSeq::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005FA830
void TalkSeq::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005FA268
void TalkSeq::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005FA174
void TalkSeq::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005FA774
void TalkSeq::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005FB1A4
void TalkSeq::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005F828C
void TalkSeq::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F9148
void TalkSeq::Tag5_0(const Tag* tag)
{
    m_Frame.color = 1;
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005F90BC
void TalkSeq::Tag5_1(const Tag* tag)
{
    m_Frame.color = 4;
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005FAE20
void TalkSeq::Tag5_2(const Tag* tag)
{
    m_Frame.color = 4;
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005FADD0
void TalkSeq::Tag5_3(const Tag* tag)
{
    m_Frame.color = 4;
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005FB660
void TalkSeq::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005F8F60
void TalkSeq::Tag5_5(const Tag* tag)
{
    m_Frame.color = 1;
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005F9188
void TalkSeq::Tag5_6(const Tag* tag)
{
    m_Frame.color = 1;
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005F8FEC
void TalkSeq::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005FA95C
void TalkSeq::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005F8630
void TalkSeq::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005FB614
void TalkSeq::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x005FBE84
void TalkSeq::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x005FBE4C
void TalkSeq::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005FAE10
void TalkSeq::TagGenderText(const Tag* tag)
{
    InsertGenderText(m_pOwner->m_pRecept, tag);
}

// 0x005F8FA0
void TalkSeq::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005F98E8
void TalkSeq::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005F9214
void TalkSeq::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005F91C8
void TalkSeq::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005F9024
void TalkSeq::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005F85E4
void TalkSeq::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005F9070
void TalkSeq::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005FA970
void TalkSeq::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005FB5DC
void TalkSeq::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x005FBF74
void TalkSeq::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pOwner->m_pPhrase, tag);
}

// 0x005FA32C
void TalkSeq::Tag5_24(const Tag* tag)
{
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005FB674
void TalkSeq::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pOwner->m_pPhrase, tag);
}

// 0x005FB250
void TalkSeq::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FBEBC
void TalkSeq::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FBE38
void TalkSeq::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005F8598
void TalkSeq::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005F9934
void TalkSeq::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005F90FC
void TalkSeq::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005FAE60
void TalkSeq::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005F989C
void TalkSeq::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005FADBC
void TalkSeq::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FB898
void TalkSeq::Tag5_35(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x005FBC40
void TalkSeq::Tag5_36(const Tag* tag)
{
    m_Frame.color = 3;
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005FB684
void TalkSeq::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005F82D0
void TalkSeq::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FA5F4
void TalkSeq::Tag6_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FB6D8
void TalkSeq::TagPause(const Tag* tag)
{
    s32 value = *reinterpret_cast<const s32*>(tag->args);
    if (value >= 0) {
        m_PauseCount = value;
    }
}

// 0x005FBD38
void TalkSeq::TagWaitButton(const Tag* tag)
{
    m_State = 4;
}

// 0x005FBF84
void TalkSeq::TagInstantBegin(const Tag* tag)
{
    m_IsInstant = 1;
    m_pOwner->m_pState->flags |= TalkState::FLAG_TAG7_2;
}

// 0x005FBD44
void TalkSeq::TagInstantEnd(const Tag* tag)
{
    m_IsInstant = 0;
    m_pOwner->m_pState->flags &= ~TalkState::FLAG_TAG7_2;
}

// 0x005FBA0C
void TalkSeq::TagNoFastBegin(const Tag* tag)
{
    m_IsFastLocked = 1;
}

// 0x005FB28C
void TalkSeq::TagNoFastEnd(const Tag* tag)
{
    m_IsFastLocked = 0;
}

// 0x005FBF00
void TalkSeq::TagWordRangeBegin(const Tag* tag)
{
    InsertWord2B0Range(m_pOwner->m_pPhrase, tag);
}

// 0x005FBA18
void TalkSeq::TagWordRangeEnd(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FB6BC
void TalkSeq::TagAutoNext(const Tag* tag)
{
    s32 value = *reinterpret_cast<const s32*>(tag->args);
    m_pOwner->m_AutoTime = value;
    m_pOwner->m_IsAutoOnly = 0;
}

// 0x005FBEE4
void TalkSeq::TagAutoNextOnly(const Tag* tag)
{
    s32 value = *reinterpret_cast<const s32*>(tag->args);
    m_pOwner->m_AutoTime = value;
    m_pOwner->m_IsAutoOnly = 1;
}

// 0x005FB2C8
void TalkSeq::TagShake0(const Tag* tag)
{
    TalkWin::Shake& shake = m_pWin->m_Shake;
    shake.state = 0;
    shake.velocity = TalkWin::s_ShakeParams[0].impulse;
}

// 0x005FBA1C
void TalkSeq::TagShake1(const Tag* tag)
{
    TalkWin::Shake& shake = m_pWin->m_Shake;
    shake.state = 1;
    shake.velocity = TalkWin::s_ShakeParams[1].impulse;
}

// 0x005FB298
void TalkSeq::TagShake2(const Tag* tag)
{
    TalkWin::Shake& shake = m_pWin->m_Shake;
    shake.state = 2;
    shake.velocity = TalkWin::s_ShakeParams[2].impulse;
}

// 0x005FB91C
void TalkSeq::Tag7_13(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag7(13);
    }
}

// 0x005FB940
void TalkSeq::Tag7_14(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag7(14);
    }
}

// 0x005FB964
void TalkSeq::Tag7_15(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag7(15);
    }
}

// 0x005FB988
void TalkSeq::Tag7_16(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag7(16);
    }
}

// 0x005FB9AC
void TalkSeq::Tag7_17(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag7(17);
    }
}

// 0x005FB9D0
void TalkSeq::Tag7_18(const Tag* tag)
{
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u8 a = param[0];
    u8 b = param[1];
    u16 c = *reinterpret_cast<const u16*>(param + 2);
    ITalkRecept* recept = m_pOwner->m_pRecept;
    u16 d = *reinterpret_cast<const u16*>(param + 4);
    if (recept != NULL) {
        recept->OnTag7_18(a, b, c, d);
    }
}

// 0x005F9C54
void TalkSeq::OnTagGroup7(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group7Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F8668
void TalkSeq::TagChoiceJump2(const Tag* tag)
{
    SetChoice8(tag, 2, 0);
}

// 0x005FA5F8
void TalkSeq::TagChoiceJump3(const Tag* tag)
{
    SetChoice8(tag, 3, 0);
}

// 0x005FA604
void TalkSeq::TagChoiceJump4(const Tag* tag)
{
    SetChoice8(tag, 4, 0);
}

// 0x005FA610
void TalkSeq::TagChoiceJump5(const Tag* tag)
{
    SetChoice8(tag, 5, 0);
}

// 0x005FA61C
void TalkSeq::TagChoiceJump6(const Tag* tag)
{
    SetChoice8(tag, 6, 0);
}

// 0x005FBC80
void TalkSeq::TagChoiceJump2Cancel(const Tag* tag)
{
    SetChoice8(tag, 2, 1);
}

// 0x005FBC8C
void TalkSeq::TagChoiceJump3Cancel(const Tag* tag)
{
    SetChoice8(tag, 3, 1);
}

// 0x005FBC98
void TalkSeq::TagChoiceJump4Cancel(const Tag* tag)
{
    SetChoice8(tag, 4, 1);
}

// 0x005FBCA4
void TalkSeq::TagChoiceJump5Cancel(const Tag* tag)
{
    SetChoice8(tag, 5, 1);
}

// 0x005FBCB0
void TalkSeq::TagChoiceJump6Cancel(const Tag* tag)
{
    SetChoice8(tag, 6, 1);
}

// 0x005FABA0
void TalkSeq::TagChoiceJumpList(const Tag* tag)
{
    ChoiceStandardWin* choice = m_pOwner->m_pChoice;
    choice->Reset();
    choice->m_IsFixed = 1;
    s32 length = ReadChoices8_10(tag, choice->m_pItems, ChoiceStandardWin::ITEM_NUM);
    choice->m_Num = ChoiceStandardWin::ITEM_NUM;
    choice->m_CancelMode = 0;
    StartChoice(length);
}

// 0x005F9260
void TalkSeq::OnTagGroup8(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group8Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F8380
void TalkSeq::Tag9(const Tag* tag)
{
    if (m_RandomBranch.isNoRepeat) {
        m_RandomBranch.Branch(tag);
        return;
    }
    if (static_cast<u32>(tag->index) < 7) {
        u32 num = s_RandomBranchNums[tag->index];
        u16 labels[10];
        GetTagArgs(tag, labels, num);
        Branch(labels[GetRandom(num)]);
    }
}

// 0x005FB320
void TalkSeq::Tag10_0(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        Branch(recept->GetBranch10_0());
    }
}

// 0x005F9D2C
void TalkSeq::Tag10_1(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    u32 index = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        index = (player->playerInfo.playerData.gender & 1) != 0 ? 1 : 0;
    }
    Branch(labels[index]);
}

// 0x005F9370
void TalkSeq::Tag10_2(const Tag* tag)
{
    u16 labels[10];
    GetTagArgs(tag, labels, 10);
    s32 index = 0;
    if (Func5C7BDC()) {
        index = Func717120();
        if (static_cast<u32>(index) >= 10 || index < 0) {
            index = 0;
        }
    }
    Branch(labels[index]);
}

// 0x005F9DD0
void TalkSeq::Tag10_3(const Tag* tag)
{
    u16 labels[12];
    GetTagArgs(tag, labels, 12);
    u32 index = PercentToIndex(36);
    Branch(labels[index]);
}

// 0x005F886C
void TalkSeq::Tag10_4(const Tag* tag)
{
    u16 labels[12];
    GetTagArgs(tag, labels, 12);
    u32 index = PercentToIndex(39);
    Branch(labels[index]);
}

// 0x005FB380
void TalkSeq::Tag10_5(const Tag* tag)
{
    u16 labels[6];
    GetTagArgs(tag, labels, 6);
    u32 value = event::Func118C8C(GetGardenPlus()->data.npcData.npcs);
    if (value < 5) {
        value = 5;
    } else if (value > 10) {
        value = 10;
    }
    u32 index = value - 5;
    Branch(labels[index]);
}

// 0x005F8D80
void TalkSeq::TagBranchHour(const Tag* tag)
{
    u16 labels[4];
    GetTagArgs(tag, labels, 4);
    s32 from = labels[0];
    s32 to = labels[1];
    s32 hour = m_pOwner->m_pDate->hour;
    bool isInRange;
    if (to < from) {
        isInRange = hour >= from || hour < to;
    } else {
        isInRange = hour >= from && hour < to;
    }
    u32 index = isInRange ? 2 : 3;
    Branch(labels[index]);
}

// 0x005FAC74
void TalkSeq::TagBranchPartOfDay(const Tag* tag)
{
    u16 labels[6];
    GetTagArgs(tag, labels, 6);
    u32 index = time::GetPartOfDay(*m_pOwner->m_pDate);
    Branch(labels[index]);
}

// 0x005FA6CC
void TalkSeq::Tag10_8(const Tag* tag)
{
    u16 labels[8];
    GetTagArgs(tag, labels, 8);
    u32 index;
    if (Func630C3C()) {
        index = 7;
    } else {
        index = Func62FB14();
        if (!(index == 0 || index == 1 || index == 2 || index == 3 || index == 4 || index == 5 || index == 6)) {
            index = 0;
        }
    }
    Branch(labels[index]);
}

// 0x005F8E30
void TalkSeq::TagBranchWeekday(const Tag* tag)
{
    u16 labels[7];
    GetTagArgs(tag, labels, 7);
    u32 index;
    switch (m_pOwner->m_pDate->dayOfWeek) {
    case 1:
        index = 0;
        break;
    case 2:
        index = 1;
        break;
    case 3:
        index = 2;
        break;
    case 4:
        index = 3;
        break;
    case 5:
        index = 4;
        break;
    case 6:
        index = 5;
        break;
    default:
        index = 6;
        break;
    }
    Branch(labels[index]);
}

// 0x005FA080
void TalkSeq::Tag10_10(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    u32 index = SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(netgame::GetDefaultSaveNo(4))) ? 0 : 1;
    Branch(labels[index]);
}

// 0x005F8C94
void TalkSeq::Tag10_11(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        Branch(recept->GetBranch10_11());
    }
}

// 0x005F94C0
void TalkSeq::Tag10_12(const Tag* tag)
{
    // the parameters: a sum of bells, then the labels
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 bells;
    nnnstdMemCpy(&bells, param, sizeof(bells));
    u16 labels[2];
    param += sizeof(bells);
    for (u32 i = 0; i < 2; i++) {
        nnnstdMemCpy(&labels[i], param, sizeof(u16));
        param += sizeof(u16);
    }
    u32 index = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid() && bells > Func71E70C(&player->inventory)) {
        index = 1;
    }
    Branch(labels[index]);
}

// 0x005FA628
void TalkSeq::Tag10_13(const Tag* tag)
{
    u16 labels[8];
    GetTagArgs(tag, labels, 8);
    s32 index = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        index = Func713798(&player->playerAppearance);
        if (index >= 8) {
            index = 7;
        } else if (index < 0) {
            index = 0;
        }
    }
    Branch(labels[index]);
}

// 0x005F8A58
void TalkSeq::Tag10_14(const Tag* tag)
{
    u16 labels[3];
    GetTagArgs(tag, labels, 3);
    u32 index;
    if (netgame::IsMultiPlayer()) {
        index = 0;
    } else {
        netgame::ConnectMgr& connect = netgame::NetGameMgr::s_pInstance->m_ConnectMgr;
        if (connect.IsConnected(netgame::ConnectMgr::CONNECTION_RANDOM_MATCH) ||
            connect.IsConnected(netgame::ConnectMgr::CONNECTION_BEST_FRIEND)) {
            index = 2;
        } else {
            index = 1;
        }
    }
    Branch(labels[index]);
}

// 0x005F8C0C
void TalkSeq::Tag10_15(const Tag* tag)
{
    u16 labels[4];
    GetTagArgs(tag, labels, 4);
    u32 index = netgame::GetPlayerCount();
    if (index != 0) {
        index = index > 4 ? 0 : index - 1;
    }
    Branch(labels[index]);
}

// 0x005FA100
void TalkSeq::TagBranchSeason(const Tag* tag)
{
    u16 labels[4];
    GetTagArgs(tag, labels, 4);
    u32 index = time::GetSeason(*m_pOwner->m_pDate);
    Branch(labels[index]);
}

// 0x005F95A4
void TalkSeq::TagBranchMonth(const Tag* tag)
{
    u16 labels[12];
    GetTagArgs(tag, labels, 12);
    u32 index = 0;
    u32 month = m_pOwner->m_pDate->month - 1;
    if (month < 12) {
        index = month;
    }
    Branch(labels[index]);
}

// 0x005F8CF4
void TalkSeq::Tag10_18(const Tag* tag)
{
    u16 labels[5];
    GetTagArgs(tag, labels, 5);
    u32 index = GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka.nooklingState;
    if (index >= 5) {
        index = 0;
    }
    Branch(labels[index]);
}

// 0x005F84F4
void TalkSeq::Tag10_19(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    u32 index = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid() && !Func76D45C(Func71B33C(&player->playerAppearance.playerOutfit))) {
        index = 1;
    }
    Branch(labels[index]);
}

// 0x005F8450
void TalkSeq::Tag10_20(const Tag* tag)
{
    u16 labels[3];
    GetTagArgs(tag, labels, 3);
    u32 index = 2;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->playerFlags.hasBeeSting) {
        index = Func713404(player) ? 1 : 0;
    }
    Branch(labels[index]);
}

// 0x005F92EC
void TalkSeq::TagBranchOutdoor(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    u32 index = stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) ? 0 : 1;
    Branch(labels[index]);
}

// 0x005FAFAC
void TalkSeq::Tag10_22(const Tag* tag)
{
    // the parameters: the kind of the item, then the labels
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 kind = *param++;
    u16 labels[2];
    for (u32 i = 0; i < 2; i++) {
        nnnstdMemCpy(&labels[i], param, sizeof(u16));
        param += sizeof(u16);
    }
    u32 index = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        const SvFgName* item = Func71B394(&player->playerAppearance.playerOutfit);
        if (Func76BA8C(item)) {
            item::Kind itemKind = item->GetKind();
            if (kind < 14) {
                u8 expected = s_ItemKinds10_22[kind];
                if (expected == ITEM_KIND_SPECIAL) {
                    if (kind == 12) {
                        if (item->IsName(0x295B)) {
                            index = 0;
                        }
                    } else if (kind == 4) {
                        if (Func311840(item) == 8) {
                            index = 0;
                        }
                    }
                } else if (itemKind == expected) {
                    index = 0;
                }
            }
        }
    }
    Branch(labels[index]);
}

// 0x005FAF4C
void TalkSeq::Tag10_23(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        Branch(recept->GetBranch10_23());
    }
}

// 0x005F8B18
void TalkSeq::Tag10_24(const Tag* tag)
{
    u16 labels[4];
    GetTagArgs(tag, labels, 4);
    u32 mode = labels[0];
    u32 value = labels[1];
    u32 index = 3;
    SvPlayer* player = SvGetCurrentPlayer();
    bool isValue = value < 17;
    if (player != NULL && player->playerInfo.IsValid() && isValue && Func76A518(Func71B33C(&player->playerAppearance.playerOutfit))) {
        s32 current = Func713768(&player->playerAppearance);
        if (mode == 0) {
            if (static_cast<s32>(value) == current) {
                index = 2;
            }
        } else if (mode == 1) {
            if (static_cast<s32>(value) == current - 17) {
                index = 2;
            }
        }
    }
    Branch(labels[index]);
}

// 0x005F87B4
void TalkSeq::Tag10_25(const Tag* tag)
{
    // the parameters: a value, then the labels
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 value;
    nnnstdMemCpy(&value, param, sizeof(value));
    u16 labels[3];
    param += sizeof(value);
    for (u32 i = 0; i < 3; i++) {
        nnnstdMemCpy(&labels[i], param, sizeof(u16));
        param += sizeof(u16);
    }
    s32 result = Func240734(value);
    u32 index;
    if (result == 2) {
        index = 0;
    } else if (result == 1) {
        index = 1;
    } else {
        index = 2;
    }
    Branch(labels[index]);
}

// 0x005F93FC
void TalkSeq::Tag10_26(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    SvPlayer* other = Func71AD8C(&GetGardenPlus()->data.players);
    SvPlayer* player = SvGetCurrentPlayer();
    u32 index = 1;
    if (other != NULL && other->playerInfo.IsValid() && player != NULL && player->playerInfo.IsValid() && other == player) {
        index = 0;
    }
    Branch(labels[index]);
}

// 0x005FB100
void TalkSeq::Tag10_27(const Tag* tag)
{
    u16 labels[3];
    GetTagArgs(tag, labels, 3);
    u32 index = 2;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid() && labels[0] <= Func72519C(&player->inventory)) {
        index = 1;
    }
    Branch(labels[index]);
}

// 0x005F9628
void TalkSeq::Tag10_28(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        Branch(recept->GetBranch10_28());
    }
}

// 0x005F898C
void TalkSeq::Tag10_29(const Tag* tag)
{
    u16 labels[2];
    GetTagArgs(tag, labels, 2);
    u32 index = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        for (u32 i = 0; i < SvPlayerInventory::SIZE; i++) {
            const SvFgName* item = player->inventory.GetItem(i);
            u32 kind = Func768FF8(item);
            if (kind == 1 || kind == 2) {
                index = 0;
            }
        }
    }
    Branch(labels[index]);
}

// 0x005FAC14
void TalkSeq::Tag10_30(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        Branch(recept->GetBranch10_30());
    }
}

// 0x005F9EF0
void TalkSeq::Tag10_31(const Tag* tag)
{
    // the parameters: the kind of the count, then the labels
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 kind = *param++;
    u16 labels[12];
    for (u32 i = 0; i < 12; i++) {
        nnnstdMemCpy(&labels[i], param, sizeof(u16));
        param += sizeof(u16);
    }
    SvMuseum* data = &GetGardenPlus()->data.townData.museum;
    s32 count = 0;
    switch (kind) {
    case 0:
        count = Func76E230(data);
        break;
    case 1:
        count = Func76E4C8(data);
        break;
    case 2:
        count = Func76E44C(data);
        break;
    case 3:
        count = Func76E5CC(data);
        break;
    }
    u32 index = TensToIndex(count);
    Branch(labels[index]);
}

// 0x005F8318
void TalkSeq::OnTagGroup10(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 32) {
        TagFunc func = s_Group10Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F835C
void TalkSeq::Tag11(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (static_cast<u32>(tag->index) < 6) {
        recept->OnTag11(tag);
    }
}

// 0x005F842C
void TalkSeq::Tag12(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (static_cast<u32>(tag->index) < 10) {
        recept->OnTag12(tag);
    }
}

// 0x005FA8C8
void TalkSeq::TagSound(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 7) {
        SeID sound = s_Sounds13_0[index];
        if (index != 2) {
            SoundMgr::PlaySound(sound, 0.0f);
        } else {
            ITalkRecept* recept = m_pOwner->m_pRecept;
            if (recept != NULL) {
                recept->OnTag13_0(tag);
            }
        }
    }
}

// 0x005FAD74
void TalkSeq::TagIslandGameMusic(const Tag* tag)
{
    if (*reinterpret_cast<const u8*>(tag->args) == 0) {
        audio::bgm::GetFieldMusic()->StartIslandGame();
    }
}

// 0x005FAD98
void TalkSeq::TagUpaCloseMusic(const Tag* tag)
{
    if (*reinterpret_cast<const u8*>(tag->args) == 0) {
        audio::bgm::GetFieldMusic()->PlayUpaClose();
    }
}

// 0x005FB748
void TalkSeq::Tag13_3(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL && !recept->ProcessTag13_3(tag)) {
        HoldAtTag(tag);
    }
}

// 0x005FBDB0
void TalkSeq::Tag13_4(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag13_4(tag);
    }
}

// 0x005FBFC4
void TalkSeq::Tag13_5(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->OnTag13_5(tag);
    }
}

// 0x006027C4
void TalkSeq::Tag13_6(const Tag* tag)
{
    m_Song.Process(tag, false);
}

// 0x005FBDA4
void TalkSeq::Tag13_7(const Tag* tag)
{
    m_Song.Process(tag, true);
}

// 0x005FC17C
void TalkSeq::TagChoiceKind(const Tag* tag)
{
    u8 kind = *reinterpret_cast<const u8*>(tag->args);
    if (kind < 5) {
        m_pOwner->m_ChoiceKind = kind;
    }
}

// 0x005845E8
void TalkSeq::TagFxBegin(const Tag* tag)
{
    SoundMgr::SSetFx6(true);
}

// 0x005FAD6C
void TalkSeq::TagFxEnd(const Tag* tag)
{
    SoundMgr::SSetFx6(false);
}

// 0x0058468C
void TalkSeq::TagSpeechFilterBegin(const Tag* tag)
{
    SoundMgr::SSetSpeechFilter(true);
}

// 0x005FB410
void TalkSeq::TagSpeechFilterEnd(const Tag* tag)
{
    SoundMgr::SSetSpeechFilter(false);
}

// 0x005F872C
void TalkSeq::OnTagGroup13(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 13) {
        TagFunc func = s_Group13Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FBD94
void TalkSeq::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FAF34
void TalkSeq::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005FB71C
void TalkSeq::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x005FBD64
void TalkSeq::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x005FBFA4
void TalkSeq::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x005FC058
void TalkSeq::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x005FC1B4
void TalkSeq::TagSexText(const Tag* tag)
{
    InsertSexText(m_pOwner->m_pRecept, tag, GetInsert());
}

// 0x005FBD70
void TalkSeq::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x005FBD80
void TalkSeq::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005FBAB8
void TalkSeq::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005FB2F4
void TalkSeq::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005FAC0C
void TalkSeq::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005FAC10
void TalkSeq::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005FAF48
void TalkSeq::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005FB308
void TalkSeq::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005FB730
void TalkSeq::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x005FBF10
void TalkSeq::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x005FBF24
void TalkSeq::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x005FBFB0
void TalkSeq::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005F9CF0
void TalkSeq::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FAA4C
void TalkSeq::TagChoice2(const Tag* tag)
{
    SetChoice15(tag, 2, 0);
}

// 0x005FB6EC
void TalkSeq::TagChoice3(const Tag* tag)
{
    SetChoice15(tag, 3, 0);
}

// 0x005FB6F8
void TalkSeq::TagChoice4(const Tag* tag)
{
    SetChoice15(tag, 4, 0);
}

// 0x005FB704
void TalkSeq::TagChoice5(const Tag* tag)
{
    SetChoice15(tag, 5, 0);
}

// 0x005FB710
void TalkSeq::TagChoice6(const Tag* tag)
{
    SetChoice15(tag, 6, 0);
}

// 0x005FC140
void TalkSeq::TagChoice2Cancel(const Tag* tag)
{
    SetChoice15(tag, 2, 1);
}

// 0x005FC14C
void TalkSeq::TagChoice3Cancel(const Tag* tag)
{
    SetChoice15(tag, 3, 1);
}

// 0x005FC158
void TalkSeq::TagChoice4Cancel(const Tag* tag)
{
    SetChoice15(tag, 4, 1);
}

// 0x005FC164
void TalkSeq::TagChoice5Cancel(const Tag* tag)
{
    SetChoice15(tag, 5, 1);
}

// 0x005FC170
void TalkSeq::TagChoice6Cancel(const Tag* tag)
{
    SetChoice15(tag, 6, 1);
}

// 0x005FBA4C
void TalkSeq::TagChoiceList(const Tag* tag)
{
    ChoiceStandardWin* choice = m_pOwner->m_pChoice;
    choice->Reset();
    choice->m_IsFixed = 1;
    s32 length = ReadChoices15_10(tag, choice->m_pItems, ChoiceStandardWin::ITEM_NUM);
    choice->m_Num = ChoiceStandardWin::ITEM_NUM;
    choice->m_CancelMode = 0;
    StartChoice(length);
}

// 0x005FAEF0
void TalkSeq::OnTagGroup15(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group15Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FA5D0
void TalkSeq::Tag16(const Tag* tag)
{
    ITalkRecept* recept = m_pOwner->m_pRecept;
    if (static_cast<u32>(tag->index) < 4) {
        recept->OnTag16(tag);
    }
}

// 0x005FB278
void TalkSeq::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FB264
void TalkSeq::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FBD24
void TalkSeq::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FBD10
void TalkSeq::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FC008
void TalkSeq::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x005FC100
void TalkSeq::Tag17_5(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x005FBCFC
void TalkSeq::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FB8DC
void TalkSeq::Tag17_7(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x005FBCBC
void TalkSeq::Tag17_8(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x005FC0EC
void TalkSeq::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FBED0
void TalkSeq::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pOwner->m_pPhrase, tag, 1);
}

// 0x005FAB10
void TalkSeq::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FBB80
void TalkSeq::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 0, 7);
}

// 0x005FBDF4
void TalkSeq::Tag18_1(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005FBB08
void TalkSeq::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(2), tag);
}

// 0x005FC08C
void TalkSeq::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 1, 7);
}

// 0x005FC068
void TalkSeq::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 2, 7);
}

// 0x005FB418
void TalkSeq::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 3, 7);
}

// 0x005FBBA4
void TalkSeq::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 4, 7);
}

// 0x005FBDD0
void TalkSeq::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 5, 7);
}

// 0x005FC0B0
void TalkSeq::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x005FC1DC
void TalkSeq::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x005FBFE4
void TalkSeq::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 6, 7);
}

// 0x005FC194
void TalkSeq::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 7, 7);
}

// 0x005FB1F0
void TalkSeq::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 13, 7);
}

// 0x005FBBC8
void TalkSeq::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x005FBF38
void TalkSeq::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005FA2F0
void TalkSeq::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005F8F24
void TalkSeq::Tag18_16(const Tag* tag)
{
    m_Frame.color = 1;
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x005FBC04
void TalkSeq::Tag18_17(const Tag* tag)
{
    m_Frame.color = 3;
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005FB874
void TalkSeq::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 8, 7);
}

// 0x005FB43C
void TalkSeq::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 9, 7);
}

// 0x005FA920
void TalkSeq::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005FB598
void TalkSeq::Tag18_21(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005FB850
void TalkSeq::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 10, 7);
}

// 0x005FB49C
void TalkSeq::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 11, 7);
}

// 0x005FB4C0
void TalkSeq::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 12, 7);
}

// 0x005FB4E4
void TalkSeq::Tag18_25(const Tag* tag)
{
    const char16* text = s_FrameTag5;
    PushText(&text, &s_pNoStop);
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
    u32 region = GetRegion();
    if (region >= 10) {
        region = 0;
    }
    m_Timer.override = s_Waits18_25[region];
}

// 0x005FB55C
void TalkSeq::Tag18_26(const Tag* tag)
{
    m_Frame.color = 2;
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005FB7D8
void TalkSeq::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005FB460
void TalkSeq::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005FB814
void TalkSeq::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x005FBB44
void TalkSeq::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005FA2B4
void TalkSeq::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005FB214
void TalkSeq::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005FBACC
void TalkSeq::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005FB79C
void TalkSeq::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005F8770
void TalkSeq::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F92A4
void TalkSeq::Tag19_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FC264
void TalkSeq::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_0(index), tag);
    }
}

// 0x005FC304
void TalkSeq::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_1(index), tag);
    }
}

// 0x005FC218
void TalkSeq::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_2(index), tag);
    }
}

// 0x005FC2B0
void TalkSeq::Tag20_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag20_3(), tag);
}

// 0x005FC350
void TalkSeq::Tag20_4(const Tag* tag)
{
    InsertIslandGenderEnemy(GetInsert(), tag);
}

// 0x005FAE9C
void TalkSeq::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FBD98
void TalkSeq::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x005FC2E8
void TalkSeq::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x005FC1D0
void TalkSeq::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}

// 0x005FC2F4
void TalkSeq::TagGeneric_5(const Tag* tag)
{
    m_Timer.override = -1.0f;
}
} // namespace script
