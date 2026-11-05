#include "event/dEventBase.h"
#include "Other/dRegion.h"
#include "Sv/dSvApi.h"
#include "event/event_Extern.h"
#include "netgame/netgame_Api.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

namespace event {
// 0x0058FE70 (name is ours)
event::EventBase::Schedule::Schedule(s32 a, s32 b, s32 c, Time time0, Time time1, Time time2, Time time3, Time time4,
                                     u8 flag)
    : a(a), b(b), c(c), time0(time0), time1(time1), time2(time2), time3(time3), time4(time4), flag(flag)
{
}

// 0x0058FEA8 (name is ours)
event::EventBase::Schedule::Schedule() : a(0), b(0), c(0), flag(0)
{
    time0.value = 0;
    time1.value = 0;
    time2.value = 0;
    time3.value = 0;
    time4.value = 0;
}

// 0x0058FED4 (name is ours)
u16 GetRegionMask()
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return 0;
    }
    // (a debug path that returns Func165B08() is disabled)
    if (g_SvIsSaveMemoryValid) {
        return 1 << (GetGardenPlus()->data.townData.unk_0xEDAA & 0xF);
    }
    return 1 << ::GetRegion();
}

// 0x0058FF4C (name is ours)
event::EventBase::EventBase(const char* name, const char* comment, const Times& times, const Info& info,
                            const Range& range, const Schedule& schedule)
    : m_pNext(NULL), m_Name(name), m_Comment(comment), m_Times(times), m_Info(info), m_Range(range),
      m_Schedule(schedule)
{
}

// 0x007546D4 (name is ours)
bool event::EventBase::HasTimeLimit(nn::fnd::DateTimeParameters* /* date */) const
{
    return !(m_Times.start.value == DAY_START && m_Times.end.value == DAY_START);
}

// 0x007546F4 slot 0x00 (name is ours)
bool event::EventBase::IsActive(const nn::fnd::DateTimeParameters& /* date */) const
{
    return false;
}

// 0x007546FC (name is ours)
bool event::EventBase::IsInRange(nn::fnd::DateTimeParameters* date) const
{
    if (!m_Range.isEnabled) {
        return false;
    }
    if (Func750EFC(date)) {
        return m_Range.start.value == DAY_START && m_Range.end.value == DAY_START;
    }
    Time now = ToTime(*date);
    if (ToDayTime(m_Range.start.value, m_Range.start.GetHour()) > ToDayTime(now.value, now.GetHour())) {
        return false;
    }
    return ToDayTime(now.value, now.GetHour()) < ToDayEndTime(m_Range.end.value);
}

// 0x007547D0 (name is ours)
bool event::EventBase::IsBeforeStartAllDay(nn::fnd::DateTimeParameters* /* date */) const
{
    return false;
}

// 0x007547D8 slot 0x04 (name is ours)
bool event::EventBase::IsOnDate(s32 /* year */, s32 /* month */, s32 /* day */) const
{
    return false;
}

// 0x007547E0 (name is ours)
bool event::EventBase::HasSchedule() const
{
    return m_Schedule.a != 0 || m_Schedule.b != 0;
}

// 0x007547F8 (name is ours)
bool event::EventBase::GetEndDate(nn::fnd::DateTimeParameters* date) const
{
    *date = *time::GetCurrentDate();
    time::Func308EF0(date);
    if (m_Times.end.GetHour() <= DAY_START_HOUR) {
        AddDays(date, 1);
    }
    time::SetHour(date, m_Times.end.GetHour());
    time::SetMinute(date, static_cast<s8>(m_Times.end.value));
    return true;
}

// 0x00754858 (name is ours)
bool event::EventBase::GetStartDate(nn::fnd::DateTimeParameters* date) const
{
    *date = *time::GetCurrentDate();
    time::Func308EF0(date);
    if (m_Times.start.GetHour() < DAY_START_HOUR) {
        AddDays(date, 1);
    }
    time::SetHour(date, m_Times.start.GetHour());
    time::SetMinute(date, static_cast<s8>(m_Times.start.value));
    return true;
}

// 0x007548B8 (name is ours)
bool event::EventBase::IsInTime(nn::fnd::DateTimeParameters* date) const
{
    Time now = ToTime(*date);
    const Time& start = m_Times.start2.value == TIME_NONE.value ? m_Times.start : m_Times.start2;
    const Time& end = m_Times.end2.value == TIME_NONE.value ? m_Times.end : m_Times.end2;
    if (ToDayTime(start.value, start.GetHour()) > ToDayTime(now.value, now.GetHour())) {
        return false;
    }
    return ToDayTime(now.value, now.GetHour()) < ToDayEndTime(end.value);
}

// 0x00754984 (name is ours)
bool event::EventBase::IsAfterEnd(nn::fnd::DateTimeParameters* date) const
{
    Time now = ToTime(*date);
    const Time& end = m_Times.end2.value == TIME_NONE.value ? m_Times.end : m_Times.end2;
    return ToDayEndTime(end.value) <= ToDayTime(now.value, now.GetHour());
}

// 0x007549F4 (name is ours)
bool event::EventBase::IsBeforeStart(nn::fnd::DateTimeParameters* date) const
{
    Time now = ToTime(*date);
    const Time& start = m_Times.start2.value == TIME_NONE.value ? m_Times.start : m_Times.start2;
    return ToDayTime(now.value, now.GetHour()) < ToDayTime(start.value, start.GetHour());
}

// 0x00754A68 slot 0x08 (name is ours)
void event::EventBase::Update()
{
    // empty (in the original too)
}

// 0x00754A6C (name is ours)
u8 event::EventBase::GetPhase(const nn::fnd::DateTimeParameters& date) const
{
    s32 hour = date.hour + (date.hour < DAY_START_HOUR ? 24 : 0);
    u16 now = static_cast<u16>(date.minute | hour << 8);
    if (m_Schedule.a != 0 && ToDayTime(m_Schedule.time0.value, m_Schedule.time0.GetHour()) <= now &&
        now < ToDayEndTime(m_Schedule.time1.value)) {
        return 1;
    }
    if (m_Schedule.b != 0 && ToDayTime(m_Schedule.time2.value, m_Schedule.time2.GetHour()) <= now &&
        now < ToDayEndTime(m_Schedule.time3.value)) {
        return 2;
    }
    if (m_Schedule.c != 0) {
        if (ToDayTime(m_Schedule.time3.value, m_Schedule.time3.GetHour()) <= now &&
            now < ToDayEndTime(m_Schedule.time4.value)) {
            return 3;
        }
        if (ToDayTime(m_Schedule.time4.value, m_Schedule.time4.GetHour()) <= now) {
            return 4;
        }
    }
    return 0;
}

// 0x00754BB8 (name is ours)
bool event::EventBase::IsAllDay(nn::fnd::DateTimeParameters* /* date */) const
{
    return m_Times.start.value == DAY_START && m_Times.end.value == DAY_START;
}
} // namespace event
