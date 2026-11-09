#pragma once

#include "decomp.h"
#include "script/dInsert.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"

namespace script {
class MailSystem;
// The insert of the mail messages: the date and time of the message (MailSystem::m_pDate), the words of the recept (tags 5/0, 5/1) (member names are
// ours, the shared words as in TalkInsert).
// RTTI N6script10MailInsertE @ 0x008D3714
// vtable 0x00909648 (vptr 0x00909650), offset_to_top 0, 192 entries
class MailInsert : public ::script::Insert
{
public:
    // (inline in MailSystem; defined in dMailSystem.cpp, its only user)
    explicit MailInsert(MailSystem* pOwner);
    virtual ~MailInsert(); // 0x005D6AD0 slot 0x00
    // 0x005D6AC0 slot 0x04 (deleting dtor)
    virtual void Clear(); // 0x005D6978 slot 0x008
    virtual void UpdateDateYear(); // 0x005D6780 slot 0x0C4
    virtual void UpdateDateMonthName(); // 0x005D67F8 slot 0x0C8
    virtual void UpdateDateDay(); // 0x005D6390 slot 0x0CC
    virtual void UpdateDateHour(); // 0x005D66A8 slot 0x0D0
    virtual void UpdateDateMinute(); // 0x005EE6E8 slot 0x0D4
    virtual void UpdateDateSecond(); // 0x005D661C slot 0x0D8
    virtual void UpdateTag5_0(); // 0x005D6480 slot 0x100
    virtual void UpdateTag5_1(); // 0x005D63EC slot 0x104
    virtual void UpdateWeekday(); // 0x005D632C slot 0x124
    virtual void UpdateAmPm(); // 0x005D6858 slot 0x154
    virtual void UpdatePartOfDayText(); // 0x005D68F8 slot 0x158
    virtual void UpdateSeason(u32 index); // 0x005D6514 slot 0x174
    virtual void UpdateWord1D8(); // 0x005D6580 slot 0x1D8
    virtual IWord* GetDateYear(); // 0x0075AD34 slot 0x1DC
    virtual IWord* GetDateMonthName(); // 0x0075AD40 slot 0x1E0
    virtual IWord* GetDateDay(); // 0x0075ACD8 slot 0x1E4
    virtual IWord* GetDateHour(); // 0x0075AD28 slot 0x1E8
    virtual IWord* GetDateMinute(); // 0x0075AD10 slot 0x1EC
    virtual IWord* GetDateSecond(); // 0x0075AD1C slot 0x1F0
    virtual IWord* GetTag5_0(); // 0x0075ACF0 slot 0x218
    virtual IWord* GetTag5_1(); // 0x0075ACE4 slot 0x21C
    virtual IWord* GetWeekday(); // 0x0075ACCC slot 0x23C
    virtual IWord* GetAmPm(); // 0x0075AD4C slot 0x26C
    virtual IWord* GetPartOfDayText(); // 0x0075AD64 slot 0x270
    virtual IWord* GetSeason(u32 index); // 0x0075ACFC slot 0x28C
    virtual IWord* GetWord2F0(); // 0x0075AD58 slot 0x2F0
    virtual IRecept* GetRecept(); // 0x005D6AAC slot 0x2F4
    virtual const IRecept* GetRecept() const; // 0x0075AD70 slot 0x2F8

    MailSystem* m_pOwner;          // 0x11CD0
    WordNumDigit m_Year;           // 0x11CD4 (tag 4/0)
    WordFix<33> m_Month;           // 0x11D18 (tag 4/1: STR_Month)
    WordFix<33> m_Day;             // 0x11D74 (tag 4/2: STR_Day)
    WordNumDigit m_Hour;           // 0x11DD0 (tag 4/3)
    WordNumDigit m_Minute;         // 0x11E14 (tag 4/4)
    WordNumDigit m_Second;         // 0x11E58 (tag 4/5)
    WordFix<9> m_Tag5_0;           // 0x11E9C (tag 5/0: IMailRecept slot 0x94)
    WordFix<9> m_Speaker;          // 0x11EC8 (tag 5/1: IMailRecept slot 0x98)
    WordFix<13> m_Weekday;         // 0x11EF4 (tag 5/9: STR_Week)
    WordFix<12> m_AmPm;            // 0x11F28 (tag 5/22: STR_Common 20/21)
    WordFix<13> m_PartOfDay;       // 0x11F58 (tag 5/23: STR_Common 15..19)
    WordFix<22> m_Season[10];      // 0x11F8C (tag 5/30: STR_Common 11..14)
    WordFix<9> m_Word1D8;          // 0x12234 (slot 0x1D8: IMailRecept slot 0x94)

};
ASSERT_SIZE(MailInsert, 0x12260);
} // namespace script
