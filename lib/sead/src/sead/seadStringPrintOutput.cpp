#include "sead/seadStringPrintOutput.h"
#include "sead/seadBufferedSafeStringBase.h"

namespace sead {
// 0x00548E5C (name is ours)
void sead::StringPrintOutput::write(const char* str, s32 length)
{
    mPosition += mString->copyAt(mPosition, SafeStringBase<char>(str), length);
}

} // namespace sead
