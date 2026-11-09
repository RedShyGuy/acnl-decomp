#include "script/dInsert.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvTown.h"
#include "anonymous/dInsertSearchCand.h"
#include "Other/dRegion.h"
#include "script/dInflectEngine.h"
#include "script/dInsertEngine.h"
#include "script/dNumberFormat.h"
#include "script/dScriptExtern.h"
#include "script/dStrMgr.h"
#include "script/dWordFix.h"
#include "time/time_Api.h"

namespace script {
namespace {
// (inline) whether the word has no text
inline bool IsEmpty(IWord& word)
{
    const char16* text = static_cast<const IWord&>(word).GetWord();
    u32 size = word.GetSize();
    return text == NULL || size == 0 || text[0] == 0;
}

// the zodiac signs of the strings of STR_Constellation
// 0x008941CA
extern const u8 s_ConstellationIndices[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// (inline) a random string of the list that no other word of the array has; the index of the string goes into the
// key of the word
template <u32 N>
DECOMP_ALWAYS_INLINE void PickRandomString(WordKey<N, int>* words, u32 index, const char* label)
{
    WordKey<N, int>* word = &words[index];
    if (!IsEmpty(*word)) {
        return;
    }
    u32 count = StrMgr::s_pInstance->GetCount(label);
    d_script_insert::InsertSearchCand<256> cand;
    if (static_cast<s32>(count) <= 256) {
        cand.m_Num = count;
    }
    cand.Reset();
    for (WordKey<N, int>* other = words; other != words + 10; other++) {
        if (!IsEmpty(*other)) {
            cand.Remove(other->m_Key);
        }
    }
    s32 key = cand.Pick();
    word->m_Key = key;
    StrMgr::s_pInstance->GetString(word, label, key);
}
} // namespace

// 0x005EB010 (name is ours)
IWord* GetEmptyWord()
{
    // 0x00AD6748 (guard 0x0095EF1C)
    static WordFix<1> s_EmptyWord;
    return &s_EmptyWord;
}

// 0x005F10B4
Insert::Insert(IMeasure* pMeasure) : m_pMeasure(pMeasure)
{
}

// 0x005F1694
Insert::~Insert()
{
    // (only the members are destroyed)
}

// 0x005EE778
void Insert::Inflect(IWord* word)
{
    WordInfo* info = word->GetInfo();
    if (info != NULL && info->color >= 4) {
        InflectEngine inflect(word);
        WordInfo* wordInfo = word->GetInfo();
        if (wordInfo != NULL) {
            wordInfo->color = 0;
            const char16* text = word->GetWord();
            if (text != NULL && word->GetSize() != 0 && text[0] != 0) {
                inflect.Run(text);
            }
        }
    }
    InsertEngine insert(this, false);
    const char16* text = word->GetWord();
    if (insert.m_pInsert != NULL && text != NULL) {
        insert.Run(text);
    }
}

// 0x005F0990 slot 0x008
void Insert::Clear()
{
    for (u32 i = 0; i < 16; i++) {
        m_Tag4_6[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Tag4_13[i].Clear();
        m_Month[i].Clear();
        m_Day[i].Clear();
        m_Tag4_16[i].Clear();
        m_Tag4_17[i].Clear();
        m_Tag4_19[i].Clear();
        m_Tag4_20[i].Clear();
    }
    m_Tag4_18.Clear();
    m_Tag5_0.Clear();
    m_Tag5_2.Clear();
    m_Tag5_3.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_Word228[i].Clear();
    }
    m_Tag5_5.Clear();
    m_Tag5_6.Clear();
    m_Tag5_7.Clear();
    m_Word238.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_Impression[i].Clear();
    }
    m_Constellation.Clear();
    m_Tag5_12.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_Drink[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Object[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Sweets[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Sports[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Hobby[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Food[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Music[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Tag5_21[i].Clear();
    }
    m_Tag5_24.Clear();
    m_Word278.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_TownFruit[i].Clear();
        m_Word280[i].Clear();
        m_Word284[i].Clear();
        m_Book[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Tag5_31[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Tag5_33[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Word298[i].Clear();
    }
    m_Tag5_35.Clear();
    m_Tag5_36.Clear();
    m_Tag5_37.Clear();
    for (u32 i = 0; i < 23; i++) {
        m_Tag5_32[i].Clear();
    }
    for (u32 i = 0; i < 14; i++) {
        for (u32 j = 0; j < 10; j++) {
            m_Word2AC[i][j].Clear();
        }
    }
    for (u32 i = 0; i < 10; i++) {
        m_Word2B4[i].Clear();
        m_Word2B8[i].Clear();
        m_Word2BC[i].Clear();
        m_Word2C0[i].Clear();
        m_Tag17_4[i].Clear();
    }
    m_Tag17_5.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_Word2CC[i].Clear();
    }
    m_Tag0_4.Clear();
    m_Tag17_8.Clear();
    for (u32 i = 0; i < 10; i++) {
        m_Word2D8[i].Clear();
        m_Word2DC[i].Clear();
        m_Tag20_0[i].Clear();
    }
    for (u32 i = 0; i < 10; i++) {
        m_Tag20_1[i].Clear();
        m_Tag20_2[i].Clear();
    }
    m_Tag20_3.Clear();
}

// 0x005EF440 slot 0x048
void Insert::ClearExtra()
{
    // empty (in the original too)
}

// 0x005ED4CC slot 0x04C
void Insert::SetTag4_6(const WordNumDigit& number, u32 index)
{
    if (index >= 16) {
        return;
    }
    WordNumDigit* word = &m_Tag4_6[index];
    word->Copy(number);
    word->m_Kind = number.m_Kind;
    word->m_Value = number.m_Value;
}

// 0x005EDAC0 slot 0x050
void Insert::SetTag4_13(s32 value, u32 index)
{
    WordNumDigit* word = &m_Tag4_13[index];
    s32 number = value >= 0 ? value : 0;
    NumberFormat format;
    format.Format(word, number, NumberFormat::BUFFER_SIZE - 9, NumberFormat::PAD_NONE, NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = number;
    word->m_Kind = 1;
}

// 0x005EE918 slot 0x054
void Insert::SetMonth(s32 month, u32 index)
{
    WordFix<33>* word = &m_Month[index];
    if (static_cast<u32>(month - 1) < 12) {
        StrMgr::s_pInstance->GetString(word, "STR_Month", month - 1);
    }
}

// 0x005ED5DC slot 0x058
void Insert::SetDay(s32 day, u32 index)
{
    WordFix<33>* word = &m_Day[index];
    if (static_cast<u32>(day - 1) <= 30) {
        StrMgr::s_pInstance->GetString(word, "STR_Day", day - 1);
    }
}

// 0x005EDA04 slot 0x05C
void Insert::SetTag4_16(u32 hour, u32 index)
{
    if (index >= 10) {
        return;
    }
    WordNumDigit* word = &m_Tag4_16[index];
    u32 value = hour & 0xFF;
    if (value >= 24) {
        value = 0;
    }
    NumberFormat format;
    s32 shown = value;
    s32 region = GetRegion();
    if (region == 0) {
        if (shown >= 12) {
            shown -= 12;
        }
    } else if (region == 9) {
        if (shown > 12) {
            shown -= 12;
        }
    } else if (region == 1 || region == 4) {
        if (shown >= 12) {
            shown -= 12;
        }
        if (shown == 0) {
            shown = 12;
        }
    }
    format.Format(word, shown, 2, NumberFormat::PAD_NONE, NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = value;
    word->m_Kind = 2;
}

// 0x005ED620 slot 0x060
void Insert::SetTag4_17(u32 minute, u32 index)
{
    if (index >= 10) {
        return;
    }
    WordNumDigit* word = &m_Tag4_17[index];
    u32 value = minute < 60 ? minute : 0;
    NumberFormat format;
    u32 region = GetRegionIndex();
    format.Format(word, value, 2, region == 1 || region == 2 ? NumberFormat::PAD_ZERO_LEFT : NumberFormat::PAD_NONE,
                  NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = value;
    word->m_Kind = 3;
}

// 0x005ED948 slot 0x064
void Insert::SetTag4_19(u32 value, u32 index)
{
    if (index >= 10) {
        return;
    }
    WordNumDigit* word = &m_Tag4_19[index];
    NumberFormat format;
    u32 clamped = value < 0x4000 ? value : 0x3FFF;
    s32 region = GetRegion();
    if (region == 1 || region == 4) {
        format.FormatDecimal(word, Func5D2A54(clamped) + 0.005f, 2);
    } else {
        format.FormatDecimal(word, Func5D2B2C(clamped) + 0.05f, 1);
    }
    word->m_Value = value;
    word->m_Kind = 7;
}

// 0x005EEC98 slot 0x068
void Insert::SetTag4_20(u32 value, u32 index)
{
    if (index >= 10) {
        return;
    }
    WordNumDigit* word = &m_Tag4_20[index];
    NumberFormat format;
    u32 clamped = value < 0x4000 ? value : 0x3FFF;
    GetRegion();
    format.FormatDecimal(word, Func5D2A6C(clamped) + 0.05f, 1);
    word->m_Value = value;
    word->m_Kind = 6;
}

// 0x005ED544 slot 0x084
void Insert::SetWord2AC(const IWord& word, u32 index, u32 subIndex)
{
    if (index < 14 && subIndex < 10) {
        m_Word2AC[index][subIndex].Copy(word);
    }
}

// 0x005EF6E0 slot 0x0A4
void Insert::SetTag0_4(u64 value)
{
    NumberFormat format;
    format.FormatHex(&m_Tag0_4, value);
}

// 0x005EDB20 slot 0x0DC
void Insert::UpdateNumber(u32 index, s32 digits, bool isRandom)
{
    WordNumDigit* word = &m_Tag4_6[index];
    if (IsEmpty(*word) && static_cast<u32>(digits - 1) <= 9) {
        if (isRandom) {
            u32 max = 10;
            for (s32 i = 1; i < digits; i++) {
                max *= 10;
            }
            u32 value = GetRandom(max);
            NumberFormat format;
            format.Format(word, value, digits, NumberFormat::PAD_BY_REGION, NumberFormat::SEPARATOR_BY_REGION);
            word->m_Value = value;
            word->m_Kind = 0;
        } else {
            IRecept* recept = GetRecept();
            if (recept != NULL) {
                recept->GetNumber(word, digits, index);
            }
        }
    }
    Inflect(word);
}

// 0x005EF17C slot 0x0E0
void Insert::UpdateYear(u32 index)
{
    WordNumDigit* word = &m_Tag4_13[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 value = 0;
            recept->GetYear(&value, index);
            if (value < 0) {
                value = 0;
            }
            if (value != 0) {
                NumberFormat format;
                format.Format(word, value, 10, NumberFormat::PAD_NONE, NumberFormat::SEPARATOR_BY_REGION);
                word->m_Value = value;
                word->m_Kind = 1;
            }
        }
    }
    Inflect(word);
}

// 0x005EF474 slot 0x0E4
void Insert::UpdateMonth(u32 index)
{
    WordFix<33>* word = &m_Month[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 month = 0;
            recept->GetMonth(&month, index);
            if (month != 0 && static_cast<u32>(month - 1) < 12) {
                StrMgr::s_pInstance->GetString(word, "STR_Month", month - 1);
            }
        }
    }
    Inflect(word);
}

// 0x005EED9C slot 0x0E8
void Insert::UpdateDay(u32 index)
{
    WordFix<33>* word = &m_Day[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 day = 0;
            recept->GetDay(&day, index);
            if (day != 0 && static_cast<u32>(day - 1) <= 30) {
                StrMgr::s_pInstance->GetString(word, "STR_Day", day - 1);
            }
        }
    }
    Inflect(word);
}

// 0x005EF16C slot 0x0EC
void Insert::UpdateTag4_16(u32 index)
{
    Inflect(&m_Tag4_16[index]);
}

// 0x005EEE6C slot 0x0F0
void Insert::UpdateTag4_17(u32 index)
{
    Inflect(&m_Tag4_17[index]);
}

// 0x005EED1C slot 0x0F4
void Insert::UpdateTag4_18()
{
    if (IsEmpty(m_Tag4_18)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag4_18(&m_Tag4_18);
        }
    }
    Inflect(&m_Tag4_18);
}

// 0x005EF038 slot 0x0F8
void Insert::UpdateTag4_19(u32 index)
{
    WordNumDigit* word = &m_Tag4_19[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 value = 0;
            recept->GetTag4_19(&value, index);
            NumberFormat format;
            u32 clamped = static_cast<u32>(value) < 0x4000 ? value : 0x3FFF;
            s32 region = GetRegion();
            if (region == 1 || region == 4) {
                format.FormatDecimal(word, Func5D2A54(clamped) + 0.005f, 2);
            } else {
                format.FormatDecimal(word, Func5D2B2C(clamped) + 0.05f, 1);
            }
            word->m_Value = value;
            word->m_Kind = 7;
        }
    }
    Inflect(word);
}

// 0x005EF720 slot 0x0FC
void Insert::UpdateTag4_20(u32 index)
{
    WordNumDigit* word = &m_Tag4_20[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 value = 0;
            recept->GetTag4_20(&value, index);
            NumberFormat format;
            u32 clamped = static_cast<u32>(value) < 0x4000 ? value : 0x3FFF;
            GetRegion();
            format.FormatDecimal(word, Func5D2A6C(clamped) + 0.05f, 1);
            word->m_Value = value;
            word->m_Kind = 6;
        }
    }
    Inflect(word);
}

// 0x005EE200 slot 0x100
void Insert::UpdateTag5_0()
{
    if (IsEmpty(m_Tag5_0)) {
        WordFix<9> name;
        Func56DE5C(&name, 4);
        m_Tag5_0.Copy(name);
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerInfo.IsValid();
    }
    Inflect(&m_Tag5_0);
}

// 0x005EF660 slot 0x108
void Insert::UpdateTag5_2()
{
    if (IsEmpty(m_Tag5_2)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_2(&m_Tag5_2);
        }
    }
    Inflect(&m_Tag5_2);
}

// 0x005EF5E0 slot 0x10C
void Insert::UpdateTag5_3()
{
    if (IsEmpty(m_Tag5_3)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_3(&m_Tag5_3);
        }
    }
    Inflect(&m_Tag5_3);
}

// 0x005EFBA0 slot 0x110
void Insert::UpdateWord228(u32 index)
{
    WordKey<9, int>* word = &m_Word228[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord228(m_Word228, index);
        }
    }
    Inflect(word);
}

// 0x005EDC48 slot 0x114
void Insert::UpdateTag5_5()
{
    if (IsEmpty(m_Tag5_5)) {
        const SvTownId* town = &GetGardenPlus()->data.townData.townData1;
        WordFix<9> name;
        MakeTownName(&name, town);
        m_Tag5_5.Copy(name);
    }
    Inflect(&m_Tag5_5);
}

// 0x005EE2AC slot 0x118
void Insert::UpdateTag5_6()
{
    if (IsEmpty(m_Tag5_6)) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL && player->playerInfo.IsValid()) {
            const SvTownId* town = &player->playerInfo.townData;
            if (town->IsValid()) {
                WordFix<9> name;
                MakeTownName(&name, town);
                m_Tag5_6.Copy(name);
            }
        }
        if (IsEmpty(m_Tag5_6)) {
            m_Tag5_6.Clear();
        }
    }
    Inflect(&m_Tag5_6);
}

// 0x005EDE24 slot 0x11C
void Insert::UpdateTag5_7()
{
    if (IsEmpty(m_Tag5_7)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_7(&m_Tag5_7);
        }
    }
    Inflect(&m_Tag5_7);
}

// 0x005EF254 slot 0x120
void Insert::UpdateWord238()
{
    if (IsEmpty(m_Word238)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord238(&m_Word238);
        }
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerInfo.IsValid();
    }
    Inflect(&m_Word238);
}

// 0x005EFA9C slot 0x128
void Insert::UpdateImpression(u32 index)
{
    WordFix<36>* word = &m_Impression[index];
    if (IsEmpty(*word) && SvGetCurrentPlayer() != NULL) {
        u32 impression = Func7713C8();
        const SvPlayer* player = SvGetCurrentPlayer();
        const char* label;
        if (player != NULL && player->playerInfo.IsValid() && (player->playerInfo.playerData.gender & 1) != 0) {
            label = impression <= 50 ? "STR_ImpressF" : NULL;
        } else {
            label = impression <= 50 ? "STR_ImpressM" : NULL;
        }
        if (label != NULL) {
            StrMgr::s_pInstance->GetString(word, label, impression);
        } else {
            word->Clear();
        }
    }
    Inflect(word);
}

// 0x005F0310 slot 0x12C
void Insert::UpdateConstellation()
{
    if (IsEmpty(m_Constellation)) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL) {
            u8 sign = time::GetZodiacSign(player->birthMonth, player->birthDay);
            if (sign < 12) {
                StrMgr::s_pInstance->GetString(&m_Constellation, "STR_Constellation", s_ConstellationIndices[sign]);
            }
        }
    }
    Inflect(&m_Constellation);
}

// 0x005F0290 slot 0x130
void Insert::UpdateTag5_12()
{
    if (IsEmpty(m_Tag5_12)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_12(&m_Tag5_12);
        }
    }
    Inflect(&m_Tag5_12);
}

// 0x005EDCE4 slot 0x134
void Insert::UpdateDrink(u32 index, bool)
{
    PickRandomString(m_Drink, index, "STR_Drink");
    Inflect(&m_Drink[index]);
}

// 0x005EEAB0 slot 0x138
void Insert::UpdateObject(u32 index, bool)
{
    if (IsEmpty(m_Object[index])) {
        const char* label = "STR_ObjectM";
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 sex = recept->GetSex();
            if (sex == 1) {
                label = "STR_ObjectM";
            } else if (sex == 2) {
                label = "STR_ObjectF";
            }
        }
        PickRandomString(m_Object, index, label);
    }
    Inflect(&m_Object[index]);
}

// 0x005EE5A8 slot 0x13C
void Insert::UpdateSweets(u32 index, bool)
{
    PickRandomString(m_Sweets, index, "STR_Sweets");
    Inflect(&m_Sweets[index]);
}

// 0x005EE468 slot 0x140
void Insert::UpdateSports(u32 index, bool)
{
    PickRandomString(m_Sports, index, "STR_Sports");
    Inflect(&m_Sports[index]);
}

// 0x005EDEA4 slot 0x144
void Insert::UpdateHobby(u32 index, bool)
{
    if (IsEmpty(m_Hobby[index])) {
        const char* label = "STR_HobbyM";
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            s32 sex = recept->GetSex();
            if (sex == 1) {
                label = "STR_HobbyM";
            } else if (sex == 2) {
                label = "STR_HobbyF";
            }
        }
        PickRandomString(m_Hobby, index, label);
    }
    Inflect(&m_Hobby[index]);
}

// 0x005ED808 slot 0x148
void Insert::UpdateFood(u32 index, bool)
{
    PickRandomString(m_Food, index, "STR_Food");
    Inflect(&m_Food[index]);
}

// 0x005EE02C slot 0x14C
void Insert::UpdateMusic(u32 index, bool)
{
    PickRandomString(m_Music, index, "STR_Music");
    Inflect(&m_Music[index]);
}

// 0x005EF2EC slot 0x150
void Insert::UpdateTag5_21(u32 index, bool)
{
    static const u32 NAME_NUM = 91;
    static const u16 FIRST_NAME = 0x212B;

    WordKey<29, int>* word = &m_Tag5_21[index];
    if (IsEmpty(*word)) {
        d_script_insert::InsertSearchCand<256> cand;
        cand.m_Num = NAME_NUM;
        cand.Reset();
        for (WordKey<29, int>* other = m_Tag5_21; other != m_Tag5_21 + 10; other++) {
            if (!IsEmpty(*other)) {
                cand.Remove(other->m_Key);
            }
        }
        s32 key = cand.Pick();
        SvFgName name(static_cast<SvFgName::ID>(key + FIRST_NAME));
        word->Copy(name.GetName(NULL));
        word->m_Key = key;
    }
    Inflect(word);
}

// 0x005EEE80 slot 0x15C
void Insert::UpdateTag5_24(u32 index)
{
    u32 hour = m_Tag4_16[index].m_Value & 0xFF;
    s32 region = GetRegion();
    if (region == 0 || region == 1 || region == 2 || region == 3 || region == 4 || region == 9) {
        StrMgr::s_pInstance->GetString(&m_Tag5_24, "STR_Common", hour < 12 ? 20 : 21);
    } else {
        m_Tag5_24.Clear();
    }
    Inflect(&m_Tag5_24);
}

// 0x005EFC38 slot 0x160
void Insert::UpdateWord278(u32 index)
{
    u32 hour = m_Tag4_16[index].m_Value;
    if (hour >= 24) {
        hour = 0;
    }
    nn::fnd::DateTimeParameters date = time::MakeDate(2012, 1, 1, hour, 0, 0);
    u32 index2;
    switch (time::GetPartOfDay(date)) {
    case 2:
        index2 = 16;
        break;
    case 3:
        index2 = 17;
        break;
    case 4:
        index2 = 18;
        break;
    case 5:
        index2 = 19;
        break;
    default:
        index2 = 15;
        break;
    }
    StrMgr::s_pInstance->GetString(&m_Word278, "STR_Common", index2);
    Inflect(&m_Word278);
}

// 0x005EF818 slot 0x164
void Insert::UpdateTownFruit(u32 index)
{
    WordFix<45>* word = &m_TownFruit[index];
    if (IsEmpty(*word)) {
        SvGardenPlus* data = GetGardenPlus();
        if (data->data.townData.townData1.IsValid()) {
            word->Copy(data->data.townData.townFruit.GetName(NULL));
        }
    }
    Inflect(word);
}

// 0x005F03C8 slot 0x168
void Insert::UpdateWord280(u32 index)
{
    WordFix<9>* word = &m_Word280[index];
    if (IsEmpty(*word)) {
        SvPlayer* other = Func71AD8C(&GetGardenPlus()->data.players);
        if (SvGetCurrentPlayer() != NULL) {
            const SvPersonalId* id = &other->playerInfo;
            WordFix<9> name;
            MakePlayerName(&name, id->playerData.playerName, id->playerData.gender & 1);
            word->Copy(name);
        } else {
            word->Clear();
        }
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerInfo.IsValid();
    }
    Inflect(word);
}

// 0x005F01A0 slot 0x16C
void Insert::UpdateWord284(u32 index)
{
    WordFix<9>* word = &m_Word284[index];
    if (IsEmpty(*word)) {
        SvPlayer* other = Func71AD8C(&GetGardenPlus()->data.players);
        if (other != NULL) {
            const SvPersonalId* id = &other->playerInfo;
            WordFix<9> name;
            MakePlayerName(&name, id->playerData.playerName, id->playerData.gender & 1);
            word->Copy(name);
        } else {
            word->Clear();
        }
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerInfo.IsValid();
    }
    Inflect(word);
}

// 0x005ED6C8 slot 0x170
void Insert::UpdateBook(u32 index, bool)
{
    PickRandomString(m_Book, index, "STR_Book");
    Inflect(&m_Book[index]);
}

// 0x005EE16C slot 0x178
void Insert::UpdateTag5_31(u32 index)
{
    WordFix<45>* word = &m_Tag5_31[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_31(word, index);
        }
    }
    Inflect(word);
}

// 0x005EE95C slot 0x17C
void Insert::UpdateTag5_33(u32 index)
{
    WordKey<45, int>* word = &m_Tag5_33[index];
    if (IsEmpty(*word)) {
        SvFgNameSet set;
        SetupFgNameSet(&set, 74);
        for (WordKey<45, int>* other = m_Tag5_33; other != m_Tag5_33 + 10; other++) {
            if (!IsEmpty(*other)) {
                SvFgName name(static_cast<SvFgName::ID>(other->m_Key));
                RemoveFgName(&set, &name);
            }
        }
        SvFgName picked = PickFgName(&set);
        word->Copy(picked.GetName(NULL));
        word->m_Key = GetFgNameId(&picked);
    }
    Inflect(word);
}

// 0x005EF548 slot 0x180
void Insert::UpdateWord298(u32 index)
{
    WordKey<9, int>* word = &m_Word298[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord298(m_Word298, index);
        }
    }
    Inflect(word);
}

// 0x005EFD20 slot 0x184
void Insert::UpdateTag5_35(u32 index)
{
    if (IsEmpty(m_Tag5_35)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_35(&m_Tag5_35, index);
        }
    }
    Inflect(&m_Tag5_35);
}

// 0x005EFE90 slot 0x188
void Insert::UpdateTag5_36()
{
    if (IsEmpty(m_Tag5_36)) {
        WordFix<9> name;
        Func56DE5C(&name, 0);
        m_Tag5_36.Copy(name);
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerInfo.IsValid();
    }
    Inflect(&m_Tag5_36);
}

// 0x005EFCEC slot 0x18C
void Insert::UpdateTag5_37()
{
    Inflect(&m_Tag5_37);
}

// 0x005EEF1C slot 0x190
void Insert::UpdateTag5_32(u32 index)
{
    WordFix<109>* word = &m_Tag5_32[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag5_32(word, index);
        }
    }
    Inflect(word);
}

// 0x005EE3BC slot 0x194
void Insert::UpdateWord2AC(u32 index, u32 subIndex)
{
    WordFix<109>* word = &m_Word2AC[index][subIndex];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2AC(word, index, subIndex);
        }
    }
    Inflect(word);
}

// 0x005EF71C slot 0x198
void Insert::UpdateWord198()
{
    // empty (in the original too)
}

// 0x005EFA08 slot 0x19C
void Insert::UpdateWord2B4(u32 index)
{
    IWord* word = &m_Word2B4[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2B4(word, index);
        }
    }
    Inflect(word);
}

// 0x005EF974 slot 0x1A0
void Insert::UpdateWord2B8(u32 index)
{
    IWord* word = &m_Word2B8[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2B8(word, index);
        }
    }
    Inflect(word);
}

// 0x005F0110 slot 0x1A4
void Insert::UpdateWord2BC(u32 index)
{
    IWord* word = &m_Word2BC[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2BC(word, index);
        }
    }
    Inflect(word);
}

// 0x005F007C slot 0x1A8
void Insert::UpdateWord2C0(u32 index)
{
    IWord* word = &m_Word2C0[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2C0(word, index);
        }
    }
    Inflect(word);
}

// 0x005F0514 slot 0x1AC
void Insert::UpdateTag17_4(u32 index)
{
    IWord* word = &m_Tag17_4[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag17_4(word, index);
        }
    }
    Inflect(word);
}

// 0x005F06A4 slot 0x1B0
void Insert::UpdateTag17_5()
{
    if (IsEmpty(m_Tag17_5)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag17_5(&m_Tag17_5);
        }
    }
    Inflect(&m_Tag17_5);
}

// 0x005EFFE8 slot 0x1B4
void Insert::UpdateWord2CC(u32 index)
{
    IWord* word = &m_Word2CC[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2CC(word, index);
        }
    }
    Inflect(word);
}

// 0x005EFF68 slot 0x1BC
void Insert::UpdateTag17_8()
{
    if (IsEmpty(m_Tag17_8)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag17_8(&m_Tag17_8);
        }
    }
    Inflect(&m_Tag17_8);
}

// 0x005F0610 slot 0x1C0
void Insert::UpdateWord2D8(u32 index)
{
    IWord* word = &m_Word2D8[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetWord2D8(word, index);
        }
    }
    Inflect(word);
}

// 0x005F07E8 slot 0x1C8
void Insert::UpdateTag20_0(u32 index)
{
    IWord* word = &m_Tag20_0[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag20_0(word, index);
        }
    }
    Inflect(word);
}

// 0x005F08FC slot 0x1CC
void Insert::UpdateTag20_1(u32 index)
{
    IWord* word = &m_Tag20_1[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag20_1(word, index);
        }
    }
    Inflect(word);
}

// 0x005F0754 slot 0x1D0
void Insert::UpdateTag20_2(u32 index)
{
    IWord* word = &m_Tag20_2[index];
    if (IsEmpty(*word)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag20_2(word, index);
        }
    }
    Inflect(word);
}

// 0x005F087C slot 0x1D4
void Insert::UpdateTag20_3()
{
    if (IsEmpty(m_Tag20_3)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            recept->GetTag20_3(&m_Tag20_3);
        }
    }
    Inflect(&m_Tag20_3);
}

// 0x005EFDE0 slot 0x1B8
void Insert::UpdateTag0_4()
{
    if (IsEmpty(m_Tag0_4)) {
        IRecept* recept = GetRecept();
        if (recept != NULL) {
            u64 value = 0;
            recept->GetTag0_4(&value);
            NumberFormat format;
            format.FormatHex(&m_Tag0_4, value);
        }
    }
    Inflect(&m_Tag0_4);
}

// 0x005F0500 slot 0x1C4
void Insert::UpdateWord2DC(u32 index)
{
    Inflect(&m_Word2DC[index]);
}

// 0x005EED18 slot 0x1D8
void Insert::UpdateWord1D8()
{
    // empty (in the original too)
}

// 0x0075CBBC slot 0x2B0
IWord* Insert::GetWord2B0()
{
    return GetEmptyWord();
}

// 0x0075CC60 slot 0x2F0
IWord* Insert::GetWord2F0()
{
    return GetEmptyWord();
}

// 0x0075CDC0 slot 0x2FC
bool Insert::IsTalk()
{
    return false;
}
} // namespace script
