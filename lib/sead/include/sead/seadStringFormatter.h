#pragma once

#include "decomp.h"
#include "sead/seadPrintFormatter.h"
#include "sead/seadStringPrintOutput.h"

namespace sead {
// A PrintFormatter that writes into a string (emptied first) through its own StringPrintOutput. The game
// builds one on the stack, appends the format and the arguments and flushes it (no RTTI; the class and
// the names are ours).
class StringFormatter : public PrintFormatter
{
public:
    explicit StringFormatter(BufferedSafeStringBase<char>* string); // 0x0054B12C (name is ours)

    StringPrintOutput mStringOutput; // 0x14
};
ASSERT_SIZE(StringFormatter, 0x20);
} // namespace sead
