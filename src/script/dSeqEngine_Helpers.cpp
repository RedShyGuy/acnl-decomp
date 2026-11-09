#include "Other/dRegion.h"
#include "script/dCmpEngine.h"
#include "script/dElisionEngine.h"
#include "script/dEpenthesisEngine.h"
#include "script/dIMeasure.h"
#include "script/dIRecept.h"
#include "script/dInsert.h"
#include "script/dInsertSearcher.h"
#include "script/dPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dSearcher.h"
#include "script/dSeqEngine.h"
#include "script/dStrMgr.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"
#include "script/dWordRes.h"
#include "ssys/ma/ma_Math.h"
#include "nn/nstd/nstd_String.h"

// The shared tag handlers of SeqEngine (names are ours, the grammar tags after their names in the binary).

namespace script {
namespace {
// (inline) the texts of the parameters of a tag: per text the size in bytes and the text
DECOMP_ALWAYS_INLINE void ReadTexts(const u8* param, TextPtr* texts, u16* lengths, u32 num)
{
    for (u32 i = 0; i < num; i++) {
        u16 size;
        nnnstdMemCpy(&size, param, sizeof(size));
        lengths[i] = size / sizeof(char16);
        param += sizeof(size);
        texts[i].pText = reinterpret_cast<const char16*>(param);
        param += size;
    }
}

// (inline) the form of the gender of a word (1: the first, 2: the second, 3: the third form; else -1)
DECOMP_ALWAYS_INLINE s32 GetGenderForm(const IWord* word, u32 num)
{
    const WordInfo* info = word->GetInfo();
    if (info == NULL) {
        return -1;
    }
    switch (info->color) {
    case 1:
        return 0;
    case 2:
        return 1;
    case 3:
        if (num > 2) {
            return 2;
        }
        break;
    }
    return -1;
}
} // namespace

// While measuring, the widest of the texts when the target of a grammar tag is not found (the message is the
// warning of the debug build; name is ours).
// 0x005F3D08 (name is ours)
void SelectWidestText(s32* pIndex, const TextPtr* texts, const u16* lengths, s32 num, const Insert* insert, const char*)
{
    if (insert == NULL) {
        return;
    }
    IMeasure* measure = insert->m_pMeasure;
    if (measure == NULL || !measure->IsFlag13()) {
        return;
    }
    f32 max = 0.0f;
    s32 index = -1;
    for (s32 i = 0; i < num; i++) {
        const char16* begin = texts[i].pText;
        const char16* end = begin + lengths[i];
        f32 width = measure->MeasureText(&begin, &end);
        if (width > max) {
            max = width;
            index = i;
        }
    }
    if (index >= 0) {
        *pIndex = index;
    }
}

// 0x005D9004
void SeqEngine::InsertWord2B0Range(Phrase* phrase, const Tag* tag)
{
    IWord* word = phrase->GetInsert()->GetWord2B0();
    const IWord* constWord = word;
    const char16* text = constWord->GetWord();
    if (text == NULL || word->GetSize() == 0 || *text == 0) {
        m_Frame.isTag2 = 0;
        m_Frame.isTag3 = 0;
        return;
    }
    const char16* cur = m_pCur;
    Searcher searcher;
    searcher.m_Mode = Searcher::MODE_NONE;
    searcher.m_Unknown0x64 = 0;
    searcher.m_Char = 0;
    searcher.m_Group = 0x7FFFFFFF;
    searcher.m_Index = -1;
    searcher.m_IsFound = false;
    searcher.m_pFound = NULL;
    searcher.SetText(&cur, &MsgEngine::s_pNoStop);
    searcher.m_Unknown0x64 = reinterpret_cast<uptr>(cur);
    searcher.m_Group = 7;
    searcher.m_Index = 7;
    searcher.m_Mode = Searcher::MODE_TAG;
    searcher.SetText2(&MsgEngine::s_pNoStop);
    if (searcher.m_IsFound) {
        s32 length = searcher.m_pFound - reinterpret_cast<const char16*>(searcher.m_Unknown0x64);
        if (length >= 0) {
            if (m_pCur != NULL) {
                m_pCur += length;
            }
            m_Frame.InsertWord(word, tag);
        }
    }
}

// 0x005F1E80
void SeqEngine::InsertWord238(Phrase* phrase, const Tag* tag, u32 isColor)
{
    if (isColor) {
        m_Frame.color = 3;
    }
    IWord* word = phrase->GetInsert()->GetWord238();
    WordFix<9> name;
    Func56DE5C(&name, 4);
    bool isSame = IsEqualWord(word, &name);
    m_Frame.PushClose();
    m_Frame.pEngine->PushWord(word);
    m_Frame.PushOpen();
    s32 group = tag->group;
    s32 index = tag->index;
    if (HasTagAttribute(group, index, 1)) {
        m_Frame.pWords[0] = word;
    }
    if (HasTagAttribute(group, index, 2)) {
        m_Frame.pWords[1] = word;
    }
    if (HasTagAttribute(group, index, 8)) {
        m_Frame.pWords[2] = isSame ? word : NULL;
    }
}

// 0x005F1F94
void SeqEngine::InsertArticle(Insert* insert, u32 kind)
{
    const char label[] = "STR_Article";
    const char16* text = m_pCur;
    IWord* word = InsertSearcher(insert).Search(&text, 0, InsertSearcher::ARG_ANY, InsertSearcher::GROUP_ANY,
                                                InsertSearcher::INDEX_ANY);
    if (word == NULL) {
        return;
    }
    const WordInfo* info = static_cast<const IWord*>(word)->GetInfo();
    s32 article;
    if (kind == 1) {
        article = info->begin;
    } else if (kind == 2) {
        article = info->buf;
    } else {
        return;
    }
    if (article > 0) {
        WordRes res;
        if (StrMgr::s_pInstance->FindNumbered(&res, label, article)) {
            m_Frame.PushClose();
            m_Frame.pEngine->PushWord(&res);
            m_Frame.PushOpen();
        }
    }
}

// 0x005F20BC
void SeqEngine::InsertNumberText4(const Tag*)
{
    const IWord* word = m_Frame.pWords[3];
    if (word == NULL) {
        return;
    }
    s32 value = static_cast<const WordNumDigit*>(word)->m_Value;
    if (value < 0) {
        value = -value;
    }
    u32 index;
    if (value == 0) {
        index = 22;
    } else {
        s32 digit = value % 10;
        if (digit == 2 || digit == 4 || digit == 5 || digit == 7 || digit == 8 || digit == 9) {
            index = 22;
        } else {
            index = 23;
        }
    }
    WordRes res;
    if (StrMgr::s_pInstance->Find(&res, "STR_Common", index)) {
        PushWord(&res);
    }
}

// 0x005F2198
void SeqEngine::InsertNumberText5(const Tag*)
{
    const IWord* word = m_Frame.pWords[4];
    if (word == NULL) {
        return;
    }
    s32 value = static_cast<const WordNumDigit*>(word)->m_Value;
    if (value < 0) {
        value = -value;
    }
    u32 index;
    if (value == 0) {
        index = 24;
    } else {
        s32 digit = value % 10;
        if (digit == 2 || digit == 4 || digit == 5 || digit == 7 || digit == 8 || digit == 9) {
            index = 24;
        } else if (digit == 1 || digit == 6 || digit == 0) {
            index = 25;
        } else {
            index = 26;
        }
    }
    WordRes res;
    if (StrMgr::s_pInstance->Find(&res, "STR_Common", index)) {
        PushWord(&res);
    }
}

// 0x005F22A0
void SeqEngine::InsertWord298(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 4;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord298(index), tag);
    }
}

// 0x005F2308
void SeqEngine::InsertGenderText(IRecept* recept, const Tag*)
{
    if (GetRegion() != 0) {
        return;
    }
    const IWord* word = m_Frame.pWords[2];
    if (word == NULL) {
        return;
    }
    const WordInfo* info = word->GetInfo();
    if (info == NULL) {
        return;
    }
    u8 gender = info->color;
    if (recept == NULL) {
        return;
    }
    s32 index = recept->GetValue88(gender);
    if (index < 0) {
        return;
    }
    WordRes res;
    if (StrMgr::s_pInstance->Find(&res, "STR_Common", index)) {
        PushWord(&res);
    }
}

// 0x005F23C4
void SeqEngine::InsertNumberText6(const Tag*)
{
    const IWord* word = m_Frame.pWords[5];
    if (word == NULL) {
        return;
    }
    s32 value = static_cast<const WordNumDigit*>(word)->m_Value;
    if (value < 0) {
        value = -value;
    }
    u32 index;
    if (value == 0) {
        index = 33;
    } else {
        s32 digit = value % 10;
        if (digit == 2 || digit == 4 || digit == 5 || digit == 7 || digit == 8 || digit == 9) {
            index = 33;
        } else if (digit == 1 || digit == 6 || digit == 0) {
            index = 34;
        } else {
            index = 35;
        }
    }
    WordRes res;
    if (StrMgr::s_pInstance->Find(&res, "STR_Common", index)) {
        PushWord(&res);
    }
}

// 0x005F24CC
void SeqEngine::InsertTownFruit(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 2;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetTownFruit(index), tag);
    }
}

// 0x005F2538
void SeqEngine::InsertWord2B8(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 4;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2B8(index), tag);
    }
}

// 0x005F25A4
void SeqEngine::InsertWord2B4(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 3;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2B4(index), tag);
    }
}

// 0x005F2610
void SeqEngine::InsertElision(Insert* insert, const Tag* tag)
{
    static const u32 TEXT_NUM = 2;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    ReadTexts(reinterpret_cast<const u8*>(tag->args), texts, lengths, TEXT_NUM);

    s32 form = 0;
    IWord* word;
    {
        InsertSearcher searcher(insert);
        const char16* text = m_pCur;
        word = searcher.Search(&text, 7, InsertSearcher::ARG_ANY, InsertSearcher::GROUP_ANY, InsertSearcher::INDEX_ANY);
        if (word == NULL) {
            SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "ELISION タグの対象が見つからない");
        }
    }
    if (word != NULL) {
        ElisionEngine elision;
        const IWord* constWord = word;
        const char16* text = constWord->GetWord();
        if (text != NULL && word->GetSize() != 0 && *text != 0) {
            elision.Run(text);
        }
        if (elision.m_IsElision) {
            form = 1;
        }
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F2864
void SeqEngine::InsertPatchimForm(Insert*, const Tag* tag, u32 isRieulFree)
{
    static const u32 TEXT_NUM = 2;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    ReadTexts(reinterpret_cast<const u8*>(tag->args), texts, lengths, TEXT_NUM);

    // the first syllable of Hangul and the final consonant ㄹ (the third of the 28 finals after none)
    static const char16 HANGUL_FIRST = 0xAC00;
    static const char16 HANGUL_LAST = 0xD7A3;
    static const u32 FINAL_NUM = 28;
    static const u32 FINAL_RIEUL = 8;

    s32 form = 0;
    char16 c = m_Frame.lastChar;
    if (c != 0) {
        if (ssys::ma::IsInRange(static_cast<s32>(c), HANGUL_FIRST, HANGUL_LAST)) {
            u32 final = (c - HANGUL_FIRST) % FINAL_NUM;
            bool hasFinal = final != 0;
            if (isRieulFree && final == FINAL_RIEUL) {
                form = 1;
            } else if (!hasFinal) {
                form = 1;
            }
        } else if ((c >= '0' && c <= '9') || static_cast<u32>(c - 0xFF10) <= 9) {
            // the digits (also full width) by the end of their Korean reading
            if (isRieulFree) {
                if (!(c == '0' || c == '3' || c == '6' || c == 0xFF10 || c == 0xFF13 || c == 0xFF16)) {
                    form = 1;
                }
            } else {
                if (c == '2' || c == '4' || c == '5' || c == '9' || c == 0xFF12 || c == 0xFF14 || c == 0xFF15 ||
                    c == 0xFF19) {
                    form = 1;
                }
            }
        }
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F2A5C
void SeqEngine::InsertWord228(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 4;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord228(index), tag);
    }
}

// 0x005F2AC4
void SeqEngine::InsertTag5_24OrWord278(Phrase* phrase, const Tag* tag)
{
    if (GetRegion() != 0) {
        if (*reinterpret_cast<const u8*>(tag->args) < 10) {
            m_Frame.InsertWord(phrase->GetInsert()->GetTag5_24(), tag);
        }
    } else {
        if (*reinterpret_cast<const u8*>(tag->args) < 10) {
            m_Frame.InsertWord(phrase->GetInsert()->GetWord278(), tag);
        }
    }
}

// 0x005F2B5C
void SeqEngine::InsertNumberForm(Insert* insert, const Tag* tag)
{
    static const u32 TEXT_NUM = 3;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 index = param[0];
    ReadTexts(param + 2, texts, lengths, TEXT_NUM);

    s32 form = 2;
    if (index < 16) {
        s32 value = static_cast<const WordNumDigit*>(insert->GetNumber(index))->m_Value;
        if (value == 0 || value == 1) {
            form = value;
        }
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F2C7C
void SeqEngine::InsertWord2CC(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 2;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2CC(index), tag);
    }
}

// 0x005F2CE8
void SeqEngine::InsertWord2C0(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 2;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2C0(index), tag);
    }
}

// 0x005F2D54
void SeqEngine::InsertWord2BC(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 1;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2BC(index), tag);
    }
}

// 0x005F2DBC
void SeqEngine::InsertGenderN(const Tag* tag, Insert* insert)
{
    static const u32 TEXT_NUM = 2;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    ReadTexts(reinterpret_cast<const u8*>(tag->args), texts, lengths, TEXT_NUM);

    s32 form = -1;
    const IWord* word = m_Frame.pWords[1];
    if (word != NULL) {
        form = GetGenderForm(word, TEXT_NUM);
    } else {
        SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "GENDER_N タグの対象が見つからない");
    }
    if (form < 0) {
        form = 0;
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F2F58
void SeqEngine::InsertGender(Insert* insert, const Tag* tag)
{
    static const u32 TEXT_NUM = 3;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u8 index = param[0];
    ReadTexts(param + 2, texts, lengths, TEXT_NUM);

    s32 form = -1;
    if (index < 10) {
        const IWord* word = InsertSearcher(insert).Search(&m_Frame.pBegin, 3, index, InsertSearcher::GROUP_ANY,
                                                          InsertSearcher::INDEX_ANY);
        if (word != NULL) {
            form = GetGenderForm(word, TEXT_NUM);
        } else {
            SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "GENDER タグの対象が見つからない");
        }
    }
    if (form < 0) {
        form = 0;
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F3160
void SeqEngine::InsertWord284(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 3;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord284(index), tag);
    }
}

// 0x005F31CC
void SeqEngine::InsertWord280(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 3;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord280(index), tag);
    }
}

// 0x005F3238
void SeqEngine::InsertWord2DC(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 3;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2DC(index), tag);
    }
}

// 0x005F32A4
void SeqEngine::InsertEpenthesisA(Insert* insert, const Tag* tag)
{
    EpenthesisEngine engine(false);
    InsertEpenthesis(insert, tag, &engine);
}

// 0x005F3310
void SeqEngine::InsertEpenthesisE(Insert* insert, const Tag* tag)
{
    EpenthesisEngine engine(true);
    InsertEpenthesis(insert, tag, &engine);
}

// 0x005F3368
void SeqEngine::InsertEpenthesis(Insert* insert, const Tag*, EpenthesisEngine* engine)
{
    const char16* text = m_pCur;
    const IWord* word = InsertSearcher(insert).Search(&text, 7, InsertSearcher::ARG_ANY, InsertSearcher::GROUP_ANY,
                                                      InsertSearcher::INDEX_ANY);
    const char16* conjunction;
    if (word != NULL) {
        engine->m_IsMatch = 0;
        engine->m_IsDone = 0;
        const char16* wordText = word->GetWord();
        if (wordText != NULL && word->GetSize() != 0 && *wordText != 0) {
            engine->Run(wordText);
        }
        if (engine->m_IsMatch) {
            conjunction = s_EpenthesisTexts[engine->m_IsE];
        } else {
            conjunction = s_ConjunctionTexts[engine->m_IsE];
        }
    } else {
        conjunction = s_ConjunctionTexts[engine->m_IsE];
    }
    WordRes res(conjunction);
    m_Frame.PushClose();
    m_Frame.pEngine->PushWord(&res);
    m_Frame.PushOpen();
}

// 0x005F34CC
void SeqEngine::InsertTag5_22OrTag5_23(Phrase* phrase, const Tag* tag)
{
    if (GetRegion() != 0) {
        m_Frame.InsertWord(phrase->GetInsert()->GetAmPm(), tag);
    } else {
        m_Frame.InsertWord(phrase->GetInsert()->GetPartOfDayText(), tag);
    }
}

// 0x005F352C
void SeqEngine::InsertWord2AC(Phrase* phrase, const Tag* tag, u32 kind, u32 color)
{
    if (color != InsertFrame::COLOR_NONE) {
        m_Frame.color = color;
    }
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2AC(kind, index), tag);
    }
}

// 0x005F3598
void SeqEngine::InsertGenderEx(Insert* insert, const Tag* tag)
{
    static const u32 TEXT_NUM = 3;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    u32 kind = param[0];
    u32 index = param[1];
    ReadTexts(param + 2, texts, lengths, TEXT_NUM);

    s32 form = -1;
    if (kind < GENDER_EX_TAG_NUM && index < 10) {
        const TagId& id = s_GenderExTags[kind];
        const IWord* word = InsertSearcher(insert).Search(&m_Frame.pBegin, 3, index, id.group, id.index);
        if (word != NULL) {
            form = GetGenderForm(word, TEXT_NUM);
        } else {
            SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "GENDER_EX タグの対象が見つからない");
        }
    }
    if (form < 0) {
        form = 0;
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F37B4
void SeqEngine::InsertGenderP(const Tag* tag, Insert* insert)
{
    static const u32 TEXT_NUM = 2;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    ReadTexts(reinterpret_cast<const u8*>(tag->args), texts, lengths, TEXT_NUM);

    s32 form = -1;
    const IWord* word = m_Frame.pWords[0];
    if (word != NULL) {
        form = GetGenderForm(word, TEXT_NUM);
    } else {
        SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "GENDER_P タグの対象が見つからない");
    }
    if (form < 0) {
        form = 0;
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}

// 0x005F3950
void SeqEngine::InsertWord2D8(Phrase* phrase, const Tag* tag, u32 isColor)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (index < 10) {
        if (isColor) {
            m_Frame.color = 2;
        }
        m_Frame.InsertWord(phrase->GetInsert()->GetWord2D8(index), tag);
    }
}

// 0x005F3B24
void SeqEngine::InsertIslandGenderEnemy(Insert* insert, const Tag* tag)
{
    static const u32 TEXT_NUM = 2;
    TextPtr texts[TEXT_NUM];
    u16 lengths[TEXT_NUM];
    ReadTexts(reinterpret_cast<const u8*>(tag->args), texts, lengths, TEXT_NUM);

    s32 form = -1;
    const IWord* word = InsertSearcher(insert).Search(&m_Frame.pBegin, 9, InsertSearcher::ARG_ANY,
                                                      InsertSearcher::GROUP_ANY, InsertSearcher::INDEX_ANY);
    if (word != NULL) {
        form = GetGenderForm(word, TEXT_NUM);
    } else {
        SelectWidestText(&form, texts, lengths, TEXT_NUM, insert, "ISLAND_GENDER_ENEMY タグの対象が見つからない");
    }
    if (form < 0) {
        form = 0;
    }
    InsertText(texts[form].pText, texts[form].pText + lengths[form]);
}
} // namespace script
