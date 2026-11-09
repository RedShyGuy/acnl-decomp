#include "script/dInsert.h"
#include "script/dTalkInsert.h"
#include "script/dITalkRecept.h"
#include "script/dNumberFormat.h"
#include "script/dStrMgr.h"
#include "script/dTalkMeasure.h"
#include "script/dTalkSystem.h"
#include "Other/dRegion.h"
#include "time/time_Api.h"

namespace script {
namespace {
// (inline) whether the word has no text
inline bool IsEmptyWord(IWord& word)
{
    const char16* text = static_cast<const IWord&>(word).GetWord();
    u32 size = word.GetSize();
    return text == NULL || size == 0 || text[0] == 0;
}
} // namespace

// 0x005D85C4 slot 0x124 (name is ours)
void TalkInsert::UpdateWeekday()
{
    WordFix<13>* word = &m_Weekday;
    u32 weekday = static_cast<u8>(m_pOwner->m_pDate->dayOfWeek);
    if (weekday < 7) {
        StrMgr::s_pInstance->GetString(word, "STR_Week", s_WeekStrings[weekday]);
    }
    Inflect(word);
}

// 0x005D8628 slot 0x0CC (name is ours)
void TalkInsert::UpdateDateDay()
{
    WordFix<33>* word = &m_Day;
    s32 day = m_pOwner->m_pDate->day;
    if (day < 32) {
        StrMgr::s_pInstance->GetString(word, "STR_Day", day - 1);
    }
    Inflect(word);
}

// 0x005D8684 slot 0x104 (name is ours)
void TalkInsert::UpdateTag5_1()
{
    IWord& word = m_Speaker;
    if (IsEmptyWord(word)) {
        ITalkRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            word.Copy(recept->m_Name);
        } else {
            word.Clear();
        }
    }
    Inflect(&m_Speaker);
}

// 0x005D871C slot 0x174 (name is ours)
void TalkInsert::UpdateSeason(u32 index)
{
    WordFix<22>* word = &m_Season[index];
    u32 season = time::GetSeason(*m_pOwner->m_pDate);
    if (season < 4) {
        StrMgr::s_pInstance->GetString(word, "STR_Common", s_SeasonStrings[season]);
    }
    Inflect(word);
}

// 0x005D8788 slot 0x0D4 (name is ours)
void TalkInsert::UpdateDateMinute()
{
    WordNumDigit* word = &m_Minute;
    u32 minute = m_pOwner->m_pDate->minute;
    if (minute >= 60) {
        minute = 0;
    }
    NumberFormat format;
    u32 region = GetRegionIndex();
    format.Format(word, minute, 2, region == 1 || region == 2 ? NumberFormat::PAD_ZERO_LEFT : NumberFormat::PAD_NONE,
                  NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = minute;
    word->m_Kind = 3;
    Inflect(word);
}

// 0x005D8818 slot 0x0D8 (name is ours)
void TalkInsert::UpdateDateSecond()
{
    WordNumDigit* word = &m_Second;
    u32 second = m_pOwner->m_pDate->second;
    if (second >= 60) {
        second = 0;
    }
    NumberFormat format;
    u32 region = GetRegionIndex();
    format.Format(word, second, 2, region == 1 || region == 2 ? NumberFormat::PAD_ZERO_LEFT : NumberFormat::PAD_NONE,
                  NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = second;
    word->m_Kind = 4;
    Inflect(word);
}

// 0x005D88A4 slot 0x0D0 (name is ours)
void TalkInsert::UpdateDateHour()
{
    WordNumDigit* word = &m_Hour;
    u32 hour = static_cast<u8>(m_pOwner->m_pDate->hour);
    if (hour >= 24) {
        hour = 0;
    }
    NumberFormat format;
    s32 shown = hour;
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
    word->m_Value = hour;
    word->m_Kind = 2;
    Inflect(word);
}

// 0x005D897C slot 0x0C4 (name is ours)
void TalkInsert::UpdateDateYear()
{
    WordNumDigit* word = &m_Year;
    s32 year = m_pOwner->m_pDate->year;
    if (year < 0) {
        year = 0;
    }
    NumberFormat format;
    format.Format(word, year, 4, NumberFormat::PAD_NONE, NumberFormat::SEPARATOR_BY_REGION);
    word->m_Value = year;
    word->m_Kind = 1;
    Inflect(word);
}

// 0x005D89F4 slot 0x048 (name is ours)
void TalkInsert::ClearExtra()
{
    static_cast<IWord&>(m_Word2B0).Clear();
}

// 0x005D8A10 slot 0x0C8 (name is ours)
void TalkInsert::UpdateDateMonthName()
{
    WordFix<33>* word = &m_Month;
    u32 month = m_pOwner->m_pDate->month - 1;
    if (month < 12) {
        StrMgr::s_pInstance->GetString(word, "STR_Month", month);
    }
    Inflect(word);
}

// 0x005D8A70 slot 0x198 (name is ours)
void TalkInsert::UpdateWord198()
{
    IWord& word = m_Word2B0;
    if (IsEmptyWord(word)) {
        ITalkRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            recept->GetWord198(&word);
        }
    }
    Inflect(&m_Word2B0);
}

// 0x005D8AF0 slot 0x154 (name is ours)
void TalkInsert::UpdateAmPm()
{
    WordFix<12>* word = &m_AmPm;
    u32 hour = static_cast<u8>(m_pOwner->m_pDate->hour);
    s32 region = GetRegion();
    if (region == 0 || region == 1 || region == 2 || region == 3 || region == 4 || region == 9) {
        StrMgr::s_pInstance->GetString(word, "STR_Common", hour < 12 ? 20 : 21);
    } else {
        static_cast<IWord&>(*word).Clear();
    }
    Inflect(word);
}

// 0x005D8B90 slot 0x158 (name is ours)
void TalkInsert::UpdatePartOfDayText()
{
    u8 part = time::GetPartOfDay(*m_pOwner->m_pDate);
    WordFix<13>* word = &m_PartOfDay;
    u32 string;
    if (part == 2) {
        string = 16;
    } else if (part == 3) {
        string = 17;
    } else if (part == 4) {
        string = 18;
    } else if (part == 5) {
        string = 19;
    } else {
        string = 15;
    }
    StrMgr::s_pInstance->GetString(word, "STR_Common", string);
    Inflect(word);
}

// 0x005D8C10 slot 0x008 (name is ours)
void TalkInsert::Clear()
{
    Insert::Clear();
    static_cast<IWord&>(m_Year).Clear();
    static_cast<IWord&>(m_Month).Clear();
    static_cast<IWord&>(m_Day).Clear();
    static_cast<IWord&>(m_Hour).Clear();
    static_cast<IWord&>(m_Minute).Clear();
    static_cast<IWord&>(m_Second).Clear();
    static_cast<IWord&>(m_Speaker).Clear();
    static_cast<IWord&>(m_Weekday).Clear();
    static_cast<IWord&>(m_AmPm).Clear();
    static_cast<IWord&>(m_PartOfDay).Clear();
    static_cast<IWord&>(m_Word2B0).Clear();
    for (u32 i = 0; i < 10; i++) {
        static_cast<IWord&>(m_Season[i]).Clear();
    }
}

// 0x005D8D30 slot 0x2F4 (name is ours)
IRecept* TalkInsert::GetRecept()
{
    return m_pOwner->m_pRecept;
}

// 0x005D8D44 (name after the RTTI)
TalkInsert::TalkInsert(TalkSystem* pOwner) : Insert(pOwner->m_pMeasure), m_pOwner(pOwner)
{
}

// 0x005D8F20 slot 0x00 (name after the RTTI)
// 0x005D8E50 (deleting dtor)
TalkInsert::~TalkInsert()
{
    // (only the members are destroyed)
}

// 0x0075B948 slot 0x23C (name is ours)
IWord* TalkInsert::GetWeekday()
{
    return &m_Weekday;
}

// 0x0075B954 slot 0x1E4 (name is ours)
IWord* TalkInsert::GetDateDay()
{
    return &m_Day;
}

// 0x0075B960 slot 0x21C (name is ours)
IWord* TalkInsert::GetTag5_1()
{
    return &m_Speaker;
}

// 0x0075B96C slot 0x28C (name is ours)
IWord* TalkInsert::GetSeason(u32 index)
{
    return &m_Season[index];
}

// 0x0075B980 slot 0x1EC (name is ours)
IWord* TalkInsert::GetDateMinute()
{
    return &m_Minute;
}

// 0x0075B98C slot 0x1F0 (name is ours)
IWord* TalkInsert::GetDateSecond()
{
    return &m_Second;
}

// 0x0075B998 slot 0x1E8 (name is ours)
IWord* TalkInsert::GetDateHour()
{
    return &m_Hour;
}

// 0x0075B9A4 slot 0x1DC (name is ours)
IWord* TalkInsert::GetDateYear()
{
    return &m_Year;
}

// 0x0075B9B0 slot 0x1E0 (name is ours)
IWord* TalkInsert::GetDateMonthName()
{
    return &m_Month;
}

// 0x0075B9BC slot 0x2B0 (name is ours)
IWord* TalkInsert::GetWord2B0()
{
    return &m_Word2B0;
}

// 0x0075B9C8 slot 0x26C (name is ours)
IWord* TalkInsert::GetAmPm()
{
    return &m_AmPm;
}

// 0x0075B9D4 slot 0x270 (name is ours)
IWord* TalkInsert::GetPartOfDayText()
{
    return &m_PartOfDay;
}

// 0x0075B9E0 slot 0x2FC (name is ours)
bool TalkInsert::IsTalk()
{
    return true;
}

// 0x0075B9E8 slot 0x2F8 (name is ours)
const IRecept* TalkInsert::GetRecept() const
{
    return m_pOwner->m_pRecept;
}

} // namespace script
