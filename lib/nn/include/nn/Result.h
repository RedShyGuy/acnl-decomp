#pragma once

// nn::Result - the 32 bit result code returned by system calls and IPC commands.
// The class name is from the binary. Bit layout from 3dbrew ("Error codes"):
//   bits  0- 9  description
//   bits 10-17  module
//   bits 21-26  summary
//   bits 27-31  level (negative values = failure)
//
// Only "types.h" may be included here: include/forward.h includes this header.

#include "types.h"

namespace nn {

class Result
{
public:
    Result() : mValue(0) {}
    Result(bit32 value) : mValue(value) {}

    bool IsSuccess() const { return static_cast<s32>(mValue) >= 0; }
    bool IsFailure() const { return static_cast<s32>(mValue) < 0; }

    bit32 GetDescription() const { return mValue & 0x3FF; }
    bit32 GetModule() const { return (mValue >> 10) & 0xFF; }
    bit32 GetSummary() const { return (mValue >> 21) & 0x3F; }
    bit32 GetLevel() const { return (mValue >> 27) & 0x1F; }

    bit32 GetPrintableBits() const { return mValue; }

    bool operator==(const Result& rhs) const { return mValue == rhs.mValue; }
    bool operator!=(const Result& rhs) const { return mValue != rhs.mValue; }

    // A group of results: one module and a range of descriptions (level and summary are not
    // compared). The template is from the symbols ("ConstRange<-5,6,97,600,600,600>::Includes");
    // the parameter names are ours, the meaning of the last one is not known.
    template <int Level, int Summary, int Module, int DescriptionBegin, int DescriptionEnd, int Description>
    class ConstRange
    {
    public:
        static __attribute__((noinline)) bool Includes(Result result)
        {
            return result.GetModule() == static_cast<bit32>(Module) &&
                   DescriptionBegin <= static_cast<int>(result.GetDescription()) &&
                   static_cast<int>(result.GetDescription()) <= DescriptionEnd;
        }
    };

private:
    bit32 mValue;
};

static const bit32 RESULT_SUCCESS = 0;

} // namespace nn
