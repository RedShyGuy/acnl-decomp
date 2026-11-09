#pragma once

#include "decomp.h"
#include "script/dInsert.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"

namespace script {
class TalkSystem;
// The insert of the talk: the date and time of the talk (TalkSystem::m_pDate), the speaker and the words of the recept
// (member names are ours).
// RTTI N6script10TalkInsertE @ 0x008D3750
// vtable 0x009099A8 (vptr 0x009099B0), offset_to_top 0, 192 entries
class TalkInsert : public ::script::Insert
{
public:
    explicit TalkInsert(TalkSystem* pOwner); // 0x005D8D44 (name after the RTTI)
    virtual ~TalkInsert(); // 0x005D8F20 slot 0x00
    // 0x005D8E50 slot 0x04 (deleting dtor)
    virtual void Clear(); // 0x005D8C10 slot 0x008
    virtual void ClearExtra(); // 0x005D89F4 slot 0x048
    virtual void UpdateDateYear(); // 0x005D897C slot 0x0C4
    virtual void UpdateDateMonthName(); // 0x005D8A10 slot 0x0C8
    virtual void UpdateDateDay(); // 0x005D8628 slot 0x0CC
    virtual void UpdateDateHour(); // 0x005D88A4 slot 0x0D0
    virtual void UpdateDateMinute(); // 0x005D8788 slot 0x0D4
    virtual void UpdateDateSecond(); // 0x005D8818 slot 0x0D8
    virtual void UpdateTag5_1(); // 0x005D8684 slot 0x104
    virtual void UpdateWeekday(); // 0x005D85C4 slot 0x124
    virtual void UpdateAmPm(); // 0x005D8AF0 slot 0x154
    virtual void UpdatePartOfDayText(); // 0x005D8B90 slot 0x158
    virtual void UpdateSeason(u32 index); // 0x005D871C slot 0x174
    virtual void UpdateWord198(); // 0x005D8A70 slot 0x198
    virtual IWord* GetDateYear(); // 0x0075B9A4 slot 0x1DC
    virtual IWord* GetDateMonthName(); // 0x0075B9B0 slot 0x1E0
    virtual IWord* GetDateDay(); // 0x0075B954 slot 0x1E4
    virtual IWord* GetDateHour(); // 0x0075B998 slot 0x1E8
    virtual IWord* GetDateMinute(); // 0x0075B980 slot 0x1EC
    virtual IWord* GetDateSecond(); // 0x0075B98C slot 0x1F0
    virtual IWord* GetTag5_1(); // 0x0075B960 slot 0x21C
    virtual IWord* GetWeekday(); // 0x0075B948 slot 0x23C
    virtual IWord* GetAmPm(); // 0x0075B9C8 slot 0x26C
    virtual IWord* GetPartOfDayText(); // 0x0075B9D4 slot 0x270
    virtual IWord* GetSeason(u32 index); // 0x0075B96C slot 0x28C
    virtual IWord* GetWord2B0(); // 0x0075B9BC slot 0x2B0
    virtual IRecept* GetRecept(); // 0x005D8D30 slot 0x2F4
    virtual const IRecept* GetRecept() const; // 0x0075B9E8 slot 0x2F8
    virtual bool IsTalk(); // 0x0075B9E0 slot 0x2FC

    TalkSystem* m_pOwner;          // 0x11CD0
    WordNumDigit m_Year;           // 0x11CD4 (tag 4/0)
    WordFix<33> m_Month;           // 0x11D18 (tag 4/1: STR_Month)
    WordFix<33> m_Day;             // 0x11D74 (tag 4/2: STR_Day)
    WordNumDigit m_Hour;           // 0x11DD0 (tag 4/3)
    WordNumDigit m_Minute;         // 0x11E14 (tag 4/4)
    WordNumDigit m_Second;         // 0x11E58 (tag 4/5)
    WordFix<9> m_Speaker;          // 0x11E9C (tag 5/1: the name of the recept)
    WordFix<13> m_Weekday;         // 0x11EC8 (tag 5/9: STR_Week)
    WordFix<12> m_AmPm;            // 0x11EFC (tag 5/22: STR_Common 20/21)
    WordFix<13> m_PartOfDay;       // 0x11F2C (tag 5/23: STR_Common 15..19)
    WordFix<22> m_Season[10];      // 0x11F60 (tag 5/30: STR_Common 11..14)
    WordFix<17> m_Word2B0;         // 0x12208 (from the recept, ITalkRecept slot 0xB8)

    // the strings of STR_Week per weekday and of STR_Common per season
    static const u8 s_WeekStrings[7];
    static const u8 s_SeasonStrings[4];
};
ASSERT_SIZE(TalkInsert, 0x12244);
} // namespace script
