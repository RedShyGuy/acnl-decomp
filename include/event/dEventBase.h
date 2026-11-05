#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

// The events of the calendar (classes after the RTTI, events after the wikis, other names ours). The original file of the definitions is
// dEventData.cpp (RTTI of its unnamed namespace). The events have ids 0..37 (EventBase::Info::id, event::EventId); the days of the game start at 6:00, so earlier times count as the end of the day before.
namespace event {
// a time of the day: hour << 8 | minute
union Time
{
    Time() {}
    Time(s32 hour, s32 minute) : value(hour << 8 | minute) {}

    s8 GetHour() const { return parts.hour; }

    u16 value;
    struct
    {
        u8 minute;
        s8 hour;
    } parts;
};
ASSERT_SIZE(Time, 2);

const Time TIME_NONE = Time(0xFF, 0xFF);
const u16 DAY_START = 6 << 8;
// the times before 6:00 belong to the day before
const u16 NEXT_DAY = 24 << 8;
const s32 DAY_START_HOUR = 6;

// (inline) the time with the times before the start of the day after the others
inline u16 ToDayTime(u16 value, s8 hour)
{
    return value + (hour < DAY_START_HOUR ? NEXT_DAY : 0);
}
// (inline) an end time: 6:00 and before is the end of the day
inline u16 ToDayEndTime(u16 value)
{
    return value + (value <= DAY_START ? NEXT_DAY : 0);
}
inline Time ToTime(const nn::fnd::DateTimeParameters& date)
{
    Time time;
    time.value = date.hour << 8 | static_cast<u8>(date.minute);
    return time;
}

// the regions of the game (event::GetRegionMask, the bits of EventBase::Info::regionMask): the index the game makes from
// the region and the language of the console (::GetRegion 0x5D51AC); the town keeps it in SvTownData::unk_0xEDAA
enum Region : u8
{
    REGION_JAPAN = 0,
    REGION_AMERICA_ENGLISH = 1,
    REGION_AMERICA_SPANISH = 2,
    REGION_AMERICA_FRENCH = 3,
    REGION_EUROPE_ENGLISH = 4,
    REGION_EUROPE_SPANISH = 5,
    REGION_EUROPE_FRENCH = 6,
    REGION_EUROPE_ITALIAN = 7,
    REGION_EUROPE_GERMAN = 8,
    REGION_KOREA = 9,
};

// a day of the year
struct MonthDay
{
    u8 month; // 0x0
    s8 day;   // 0x1
};
ASSERT_SIZE(MonthDay, 2);

// RTTI N5event9EventBaseE @ 0x008D28FC
// vtable 0x00908A14 (vptr 0x00908A1C), offset_to_top 0, 3 entries
class EventBase
{
public:
    // the times of the event: start2/end2 replace start/end if set
    struct Times
    {
        Time start;  // 0x0
        Time end;    // 0x2
        Time start2; // 0x4
        Time end2;   // 0x6
    };
    struct Info // (size 6)
    {
        u16 regionMask; // 0x0 the regions with the event (bits of Region; GetRegionMask)
        u8 kind;      // 0x2 (low two bits)
        u8 id;        // 0x3
        u8 _4;        // 0x4
    };
    // a second range of times (only with isEnabled)
    struct Range // (size 6)
    {
        Time start;     // 0x0
        Time end;       // 0x2
        u8 isEnabled;   // 0x4
    };
    // the phases of the day of the event: 1 [time0, time1) if a, 2 [time2, time3) if b, 3 [time3, time4) if c, 4 from time4
    struct Schedule
    {
        Schedule(); // 0x0058FEA8
        Schedule(s32 a, s32 b, s32 c, Time time0, Time time1, Time time2, Time time3, Time time4, u8 flag); // 0x0058FE70

        s32 a;      // 0x00
        s32 b;      // 0x04
        s32 c;      // 0x08
        Time time0; // 0x0C
        Time time1; // 0x0E
        Time time2; // 0x10
        Time time3; // 0x12
        Time time4; // 0x14
        u8 flag;    // 0x16
    };

    EventBase(const char* name, const char* comment, const Times& times, const Info& info, const Range& range,
              const Schedule& schedule); // 0x0058FF4C
    // (inline in the constructors of the subclasses; the copy is not in a list)
    EventBase(const EventBase& other)
        : m_pNext(NULL), m_Name(other.m_Name), m_Comment(other.m_Comment), m_Times(other.m_Times), m_Info(other.m_Info),
          m_Range(other.m_Range), m_Schedule(other.m_Schedule)
    {
    }

    // the event is on the date and time (by the subclass)
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x007546F4 slot 0x00
    // the event is on the day
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x007547D8 slot 0x04
    virtual void Update(); // 0x00754A68 slot 0x08

    // the functions the events manager calls through member pointers (s_EventFunctions; the "all day" ones get NULL)
    typedef bool (EventBase::*Function)(nn::fnd::DateTimeParameters* date) const;
    enum FunctionIndex
    {
        FUNCTION_IS_ALL_DAY,
        FUNCTION_IS_IN_TIME,
        FUNCTION_IS_BEFORE_START_ALL_DAY,
        FUNCTION_IS_BEFORE_START,
        FUNCTION_HAS_TIME_LIMIT,
        FUNCTION_IS_AFTER_END,
        FUNCTION_IS_IN_RANGE,
        FUNCTION_GET_START_DATE,
        FUNCTION_GET_END_DATE,
        FUNCTION_NUM,
    };
    bool HasTimeLimit(nn::fnd::DateTimeParameters* date) const; // 0x007546D4
    bool IsInRange(nn::fnd::DateTimeParameters* date) const; // 0x007546FC
    bool IsBeforeStartAllDay(nn::fnd::DateTimeParameters* date) const; // 0x007547D0
    bool GetEndDate(nn::fnd::DateTimeParameters* date) const; // 0x007547F8
    bool GetStartDate(nn::fnd::DateTimeParameters* date) const; // 0x00754858
    bool IsInTime(nn::fnd::DateTimeParameters* date) const; // 0x007548B8
    bool IsAfterEnd(nn::fnd::DateTimeParameters* date) const; // 0x00754984
    bool IsBeforeStart(nn::fnd::DateTimeParameters* date) const; // 0x007549F4
    bool IsAllDay(nn::fnd::DateTimeParameters* date) const; // 0x00754BB8
    bool HasSchedule() const; // 0x007547E0
    // 0 or the phase of the schedule
    u8 GetPhase(const nn::fnd::DateTimeParameters& date) const; // 0x00754A6C

    // (inline) before the end of the day of the event
    bool IsBeforeEnd(const nn::fnd::DateTimeParameters& date) const
    {
        u16 time = static_cast<u16>(date.hour << 8 | date.minute);
        if (date.hour < DAY_START_HOUR) {
            if (m_Times.end.GetHour() > DAY_START_HOUR) {
                return false;
            }
            if (m_Times.end.value <= time) {
                return false;
            }
            return true;
        }
        if (m_Times.end.GetHour() <= DAY_START_HOUR) {
            return true;
        }
        if (m_Times.end.value <= time) {
            return false;
        }
        return true;
    }

    EventBase* m_pNext;     // 0x04 in the list of the active events
    const char* m_Name;     // 0x08 (empty in this build)
    const char* m_Comment;  // 0x0C
    Times m_Times;          // 0x10
    Info m_Info;            // 0x18
    Range m_Range;          // 0x1E
    Schedule m_Schedule;    // 0x24
};
ASSERT_SIZE(EventBase, 0x3C);

// the bit of the Region of the town (low four bits of SvTownData::unk_0xEDAA; without a save the one of the console;
// 0 in a dream)
DECOMP_NOINLINE u16 GetRegionMask(); // 0x0058FED4
} // namespace event
