#pragma once

#include "decomp.h"

namespace imgdb {
class DateTimeSeconds
{
public:
    void SetSeconds(long long); // 0x005A960C | nintendogs:bytes [tier B]
    void GetMaxSeconds(); // 0x005A9684 | nintendogs:bytes [tier B]
    void GetMinSeconds(); // 0x005A971C | nintendogs:bytes [tier B]
    void ConvertFromParam(int, int, int, int, int, int); // 0x005A97A8 | nintendogs:bytes [tier B]
    void GetCurrentDateTime(); // 0x005A988C | nintendogs:bytes [tier B]
    void SetCurrentDateTime(); // 0x005A98F0 | nintendogs:bytes [tier B]
    void ConvertFromNNDateTime(const nn::fnd::DateTime&); // 0x005A9948 | nintendogs:bytes [tier B]
    void ConvertFromDateTimeString(const char*); // 0x005A9988 | nintendogs:bytes [tier B]
    void GetDateDays() const; // 0x007556D0 | nintendogs:bytes [tier B]
    void ConvertToParam(int*, int*, int*, int*, int*, int*) const; // 0x0075570C | nintendogs:bytes [tier B]
    void ConvertToNNDateTime() const; // 0x0075584C | nintendogs:bytes [tier B]
    void ConvertToDateTimeString(char*, int) const; // 0x007558AC | nintendogs:bytes-fuzzy [tier B]
    void IsInvalid() const; // 0x00755924 | nintendogs:bytes [tier B]
};
} // namespace imgdb
