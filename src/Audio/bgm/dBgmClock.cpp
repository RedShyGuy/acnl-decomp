#include "Audio/bgm/dBgmClock.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {

// the hour or the date changed in this frame
// 0x0094F4BB
bool s_IsHourChanged;
// any of hour, minute and second changed
// 0x0094F4BC
bool s_IsTimeChanged;
// 0x0094F4BD
bool s_IsClockJumped;
// the date of the frame before
// 0x0098B0F4
nn::fnd::DateTimeParameters s_LastDate;

} // namespace bgm
} // namespace audio

// 0x00589F40
bool audio::bgm::UpdateHour(s8* last)
{
    s8 previous = *last;
    s8 hour = time::GetCurrentDate()->hour;
    *last = hour;
    return hour != previous;
}

// 0x00589F68
bool audio::bgm::IsInTime(u8 startMinute, u8 startSecond, u8 endMinute, u8 endSecond)
{
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    u16 start = (startMinute << 8) | startSecond;
    u16 end = (endMinute << 8) | endSecond;
    u16 now = (static_cast<u8>(date->minute) << 8) | date->second;
    if (end >= start) {
        return now >= start && end > now;
    }
    return now < end || start <= now;
}

// 0x00589FD4
bool audio::bgm::IsClockJumped()
{
    return s_IsClockJumped;
}

// 0x00589FE4
bool audio::bgm::UpdateSecond(s8* last)
{
    s8 previous = *last;
    s8 second = time::GetCurrentDate()->second;
    *last = second;
    return second != previous;
}

// 0x0058A00C
bool audio::bgm::IsTimeChanged()
{
    return s_IsTimeChanged;
}

// 0x0058A01C
void audio::bgm::UpdateClock()
{
    nn::fnd::DateTimeParameters previous = s_LastDate;
    s_LastDate = *time::GetCurrentDate();
    if (previous.year == 0) {
        return;
    }
    s_IsHourChanged = !(previous.hour == s_LastDate.hour && previous.day == s_LastDate.day &&
                        previous.month == s_LastDate.month && previous.year == s_LastDate.year);
    s_IsTimeChanged = s_IsHourChanged || previous.second != s_LastDate.second || previous.minute != s_LastDate.minute;
    if (!s_IsTimeChanged) {
        s_IsClockJumped = false;
        return;
    }
    // one second later is the normal step
    time::AddSeconds(&previous, 1);
    s_IsClockJumped = !(previous.second == s_LastDate.second && previous.minute == s_LastDate.minute &&
                        previous.hour == s_LastDate.hour && previous.day == s_LastDate.day &&
                        previous.month == s_LastDate.month && previous.year == s_LastDate.year);
}

// 0x0058A160
s8 audio::bgm::GetHour()
{
    return time::GetCurrentDate()->hour;
}

// 0x0058A170
s8 audio::bgm::GetMinute()
{
    return time::GetCurrentDate()->minute;
}

// 0x0058A180
s8 audio::bgm::GetSecond()
{
    return time::GetCurrentDate()->second;
}
