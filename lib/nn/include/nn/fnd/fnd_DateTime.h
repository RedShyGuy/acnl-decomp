#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace fnd {
class DateTime
{
public:
    DateTime(); // TODO: default ctor added so derived stubs compile - may not exist
    void FromParameters(const nn::fnd::DateTimeParameters&); // 0x0012468C | fefates:bytes [tier B]
    void GetNow(); // 0x001246C0 | fefates:bytes [tier B]
    void GetParameters() const; // 0x00126C04 | nintendogs:bytes [tier A]
    void DaysToDate(int*, int*, int*, int); // 0x0012A08C | nintendogs:bytes [tier A]
    void FromParameters(int, int, int, int, int, int, int); // 0x0012A21C | nintendogs:bytes [tier A]
    void operator+=(const nn::fnd::TimeSpan&); // 0x0012A28C | fefates:bytes [tier B]
    void DateToDays(int, int, int); // 0x00130ED4 | nintendogs:callgraph [tier A]
    DateTime(int, int, int, int, int, int, int); // 0x00130FCC | nintendogs:bytes [tier A]
    void IsValidDate(int, int, int); // 0x00352450 | nintendogs:bytes [tier B]
    void IsValidParameters(int, int, int, int, int, int, int); // 0x00352584 | nintendogs:bytes [tier B]
    s32 GetMilliSecond() const; // 0x0072975C | fefates:bytes [tier B]
    s32 GetDay() const; // 0x007297B0 | nintendogs:bytes [tier A]
    s32 GetHour() const; // 0x00729808 | nintendogs:bytes [tier A]
    s32 GetYear() const; // 0x00729900 | nintendogs:bytes [tier A]
    s32 GetMonth() const; // 0x00729958 | nintendogs:bytes [tier A]
    s32 GetMinute() const; // 0x007299B0 | nintendogs:bytes [tier A]
    s32 GetSecond() const; // 0x00729A1C | nintendogs:bytes [tier A]

    // the earliest date: the origin of mMilliSeconds (name is ours)
    static const DateTime MIN_DATE_TIME;

    // inline (name is ours)
    friend TimeSpan operator-(const DateTime& lhs, const DateTime& rhs)
    {
        return TimeSpan::FromMilliSeconds(lhs.mMilliSeconds - rhs.mMilliSeconds);
    }

private:
    explicit DateTime(s64 milliSeconds) : mMilliSeconds(milliSeconds) {}

    s64 mMilliSeconds;      // 0x0 (the member name is ours)
};
ASSERT_SIZE(DateTime, 8);
} // namespace fnd
} // namespace nn
