#include "script/dInsert.h"
#include "script/dUiInsert.h"
#include "script/dIUiRecept.h"
#include "script/dUiSystem.h"
#include "script/dNumberFormat.h"
#include "script/dTalkInsert.h"
#include "script/dStrMgr.h"
#include "Other/dRegion.h"
#include "time/time_Api.h"

namespace script {
// 0x005FE2A0 slot 0x124 (name is ours)
void UiInsert::UpdateWeekday()
{
    WordFix<13>* word = &m_Weekday;
    u32 weekday = static_cast<u8>(m_pOwner->m_pDate->dayOfWeek);
    if (weekday < 7) {
        StrMgr::s_pInstance->GetString(word, "STR_Week", TalkInsert::s_WeekStrings[weekday]);
    }
    Inflect(word);
}

// 0x005FE304 slot 0x0CC (name is ours)
void UiInsert::UpdateDateDay()
{
    WordFix<33>* word = &m_Day;
    s32 day = m_pOwner->m_pDate->day;
    if (day < 32) {
        StrMgr::s_pInstance->GetString(word, "STR_Day", day - 1);
    }
    Inflect(word);
}

// 0x005FE364 slot 0x174 (name is ours)
void UiInsert::UpdateSeason(u32 index)
{
    WordFix<22>* word = &m_Season[index];
    u32 season = time::GetSeason(*m_pOwner->m_pDate);
    if (season < 4) {
        StrMgr::s_pInstance->GetString(word, "STR_Common", TalkInsert::s_SeasonStrings[season]);
    }
    Inflect(word);
}

// 0x005FE3D0 slot 0x0D4 (name is ours)
void UiInsert::UpdateDateMinute()
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

// 0x005FE460 slot 0x0D8 (name is ours)
void UiInsert::UpdateDateSecond()
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

// 0x005FE4EC slot 0x0D0 (name is ours)
void UiInsert::UpdateDateHour()
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

// 0x005FE5C4 slot 0x0C4 (name is ours)
void UiInsert::UpdateDateYear()
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

// 0x005FE63C slot 0x0C8 (name is ours)
void UiInsert::UpdateDateMonthName()
{
    WordFix<33>* word = &m_Month;
    u32 month = m_pOwner->m_pDate->month - 1;
    if (month < 12) {
        StrMgr::s_pInstance->GetString(word, "STR_Month", month);
    }
    Inflect(word);
}

// 0x005FE69C slot 0x154 (name is ours)
void UiInsert::UpdateAmPm()
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

// 0x005FE73C slot 0x158 (name is ours)
void UiInsert::UpdatePartOfDayText()
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

// 0x005FE360 slot 0x104 (name is ours)
void UiInsert::UpdateTag5_1()
{
    // empty (in the original too)
}

// 0x005FE7BC slot 0x008 (name is ours)
void UiInsert::Clear()
{
    Insert::Clear();
    static_cast<IWord&>(m_Year).Clear();
    static_cast<IWord&>(m_Month).Clear();
    static_cast<IWord&>(m_Day).Clear();
    static_cast<IWord&>(m_Hour).Clear();
    static_cast<IWord&>(m_Minute).Clear();
    static_cast<IWord&>(m_Second).Clear();
    static_cast<IWord&>(m_Weekday).Clear();
    static_cast<IWord&>(m_AmPm).Clear();
    static_cast<IWord&>(m_PartOfDay).Clear();
    for (u32 i = 0; i < 10; i++) {
        static_cast<IWord&>(m_Season[i]).Clear();
    }
}

// 0x005FE8AC slot 0x2F4 (name is ours)
IRecept* UiInsert::GetRecept()
{
    return m_pOwner->m_pRecept;
}

// 0x005FE96C slot 0x00 (name after the RTTI)
// 0x005FE8C0 (deleting dtor)
UiInsert::~UiInsert()
{
    // (only the members are destroyed)
}

// 0x0075DCA0 slot 0x21C (name is ours)
IWord* UiInsert::GetTag5_1()
{
    return GetEmptyWord();
}

// 0x0075DCA4 slot 0x28C (name is ours)
IWord* UiInsert::GetSeason(u32 index)
{
    return &m_Season[index];
}

// 0x0075DC88 slot 0x23C (name is ours)
IWord* UiInsert::GetWeekday()
{
    return &m_Weekday;
}

// 0x0075DC94 slot 0x1E4 (name is ours)
IWord* UiInsert::GetDateDay()
{
    return &m_Day;
}

// 0x0075DCB8 slot 0x1EC (name is ours)
IWord* UiInsert::GetDateMinute()
{
    return &m_Minute;
}

// 0x0075DCC4 slot 0x1F0 (name is ours)
IWord* UiInsert::GetDateSecond()
{
    return &m_Second;
}

// 0x0075DCD0 slot 0x1E8 (name is ours)
IWord* UiInsert::GetDateHour()
{
    return &m_Hour;
}

// 0x0075DCDC slot 0x1DC (name is ours)
IWord* UiInsert::GetDateYear()
{
    return &m_Year;
}

// 0x0075DCE8 slot 0x1E0 (name is ours)
IWord* UiInsert::GetDateMonthName()
{
    return &m_Month;
}

// 0x0075DCF4 slot 0x26C (name is ours)
IWord* UiInsert::GetAmPm()
{
    return &m_AmPm;
}

// 0x0075DD00 slot 0x270 (name is ours)
IWord* UiInsert::GetPartOfDayText()
{
    return &m_PartOfDay;
}

// 0x0075DD0C slot 0x2F8 (name is ours)
const IRecept* UiInsert::GetRecept() const
{
    return m_pOwner->m_pRecept;
}

} // namespace script
