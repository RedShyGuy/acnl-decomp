#pragma once

#include "decomp.h"
#include "sead/seadBufferedSafeStringBase.h"
#include "sead/seadPrintOutput.h"

namespace sead {
template <typename T>
class BufferedSafeStringBase;

// RTTI N4sead17StringPrintOutputE @ 0x008D1C14
// vtable 0x0090606C (vptr 0x00906074), offset_to_top 0, 3 entries
//
// A PrintOutput that writes into a string from a position on (layout from write; the member names
// are ours).
class StringPrintOutput : public ::sead::PrintOutput
{
public:
    // empties the string (inline in StringFormatter; name is ours)
    explicit StringPrintOutput(BufferedSafeStringBase<char>* string) : mString(string), mPosition(0)
    {
        string->clear();
    }
    virtual ~StringPrintOutput(); // 0x00548F88 slot 0x00
    // 0x00548F84 slot 0x04 (deleting dtor)
    virtual void write(const char* str, s32 length); // 0x00548E5C slot 0x08 (name is ours)

    BufferedSafeStringBase<char>* mString; // 0x04
    s32 mPosition;                         // 0x08, where the next characters go
};
ASSERT_SIZE(StringPrintOutput, 0xC);
} // namespace sead
