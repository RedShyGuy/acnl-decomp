#include "sead/seadStringFormatter.h"

namespace sead {
// 0x0054B12C (name is ours)
sead::StringFormatter::StringFormatter(BufferedSafeStringBase<char>* string)
    : PrintFormatter(&mStringOutput), mStringOutput(string)
{
    // (only the base and the members are constructed)
}
} // namespace sead
