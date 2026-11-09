#pragma once

#include "decomp.h"
#include "script/dInsert.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"

namespace script {
class UiSystem;
// The insert of the ui messages: the date and time of the message (UiSystem::m_pDate) (member names are
// ours, the shared words as in TalkInsert).
// RTTI N6script8UiInsertE
// vtable 0x0090B71C (vptr 0x0090B724), offset_to_top 0, 192 entries
class UiInsert : public ::script::Insert
{
public:
    // (inline in UiSystem)
    explicit UiInsert(UiSystem* pOwner) : Insert(NULL), m_pOwner(pOwner) {}
    virtual ~UiInsert(); // 0x005FE96C slot 0x00
    // 0x005FE8C0 slot 0x04 (deleting dtor)
    virtual void Clear(); // 0x005FE7BC slot 0x008
    virtual void UpdateDateYear(); // 0x005FE5C4 slot 0x0C4
    virtual void UpdateDateMonthName(); // 0x005FE63C slot 0x0C8
    virtual void UpdateDateDay(); // 0x005FE304 slot 0x0CC
    virtual void UpdateDateHour(); // 0x005FE4EC slot 0x0D0
    virtual void UpdateDateMinute(); // 0x005FE3D0 slot 0x0D4
    virtual void UpdateDateSecond(); // 0x005FE460 slot 0x0D8
    virtual void UpdateTag5_1(); // 0x005FE360 slot 0x104
    virtual void UpdateWeekday(); // 0x005FE2A0 slot 0x124
    virtual void UpdateAmPm(); // 0x005FE69C slot 0x154
    virtual void UpdatePartOfDayText(); // 0x005FE73C slot 0x158
    virtual void UpdateSeason(u32 index); // 0x005FE364 slot 0x174
    virtual IWord* GetDateYear(); // 0x0075DCDC slot 0x1DC
    virtual IWord* GetDateMonthName(); // 0x0075DCE8 slot 0x1E0
    virtual IWord* GetDateDay(); // 0x0075DC94 slot 0x1E4
    virtual IWord* GetDateHour(); // 0x0075DCD0 slot 0x1E8
    virtual IWord* GetDateMinute(); // 0x0075DCB8 slot 0x1EC
    virtual IWord* GetDateSecond(); // 0x0075DCC4 slot 0x1F0
    virtual IWord* GetTag5_1(); // 0x0075DCA0 slot 0x21C
    virtual IWord* GetWeekday(); // 0x0075DC88 slot 0x23C
    virtual IWord* GetAmPm(); // 0x0075DCF4 slot 0x26C
    virtual IWord* GetPartOfDayText(); // 0x0075DD00 slot 0x270
    virtual IWord* GetSeason(u32 index); // 0x0075DCA4 slot 0x28C
    virtual IRecept* GetRecept(); // 0x005FE8AC slot 0x2F4
    virtual const IRecept* GetRecept() const; // 0x0075DD0C slot 0x2F8

    UiSystem* m_pOwner;          // 0x11CD0
    WordNumDigit m_Year;           // 0x11CD4 (tag 4/0)
    WordFix<33> m_Month;           // 0x11D18 (tag 4/1: STR_Month)
    WordFix<33> m_Day;             // 0x11D74 (tag 4/2: STR_Day)
    WordNumDigit m_Hour;           // 0x11DD0 (tag 4/3)
    WordNumDigit m_Minute;         // 0x11E14 (tag 4/4)
    WordNumDigit m_Second;         // 0x11E58 (tag 4/5)
    WordFix<13> m_Weekday;         // 0x11E9C (tag 5/9: STR_Week)
    WordFix<12> m_AmPm;            // 0x11ED0 (tag 5/22: STR_Common 20/21)
    WordFix<13> m_PartOfDay;       // 0x11F00 (tag 5/23: STR_Common 15..19)
    WordFix<22> m_Season[10];      // 0x11F34 (tag 5/30: STR_Common 11..14)

};
ASSERT_SIZE(UiInsert, 0x121DC);
} // namespace script
