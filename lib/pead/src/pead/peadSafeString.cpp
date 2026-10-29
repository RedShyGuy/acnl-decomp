#include "pead/peadBufferedSafeStringBase.h"
#include "pead/peadFixedSafeString.h"
#include "pead/peadFixedSafeStringBase.h"
#include "pead/peadSafeStringBase.h"

#include <cstdarg>
#include <cstdio>

// The functions of pia's copies of the SafeString classes, their null character, the empty strings and the
// copies of their template members (the name of the file is ours).

namespace pead {
// (the null character of <wchar_t> is not in the binary: nothing of pead takes its address)
// 0x008C9AF0
template const char pead::SafeStringBase<char>::s_NullChar;
// 0x009830C0
template <> const pead::SafeStringBase<char> pead::SafeStringBase<char>::s_EmptyString("");
// 0x009830C8
template <> const pead::SafeStringBase<wchar_t> pead::SafeStringBase<wchar_t>::s_EmptyString(L"");

// constructs the empty strings (GCC initializes them constantly)
// 0x00798E34 (static initializer)

// 0x0053B384 (name after sead)
template <>
s32 pead::BufferedSafeStringBase<char>::format(const char* fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);
    char* top = getMutableStringTop();
    s32 size = mBufferSize;
    s32 length = std::vsnprintf(top, size, fmt, args);
    if (length < 0 || length >= size) {
        top[size - 1] = '\0';
        length = size - 1;
    }
    va_end(args);
    return length;
}

// 0x00810060 (name after sead)
template <>
pead::FixedSafeString<32>::FixedSafeString()
{
    // (only the bases are constructed)
}

// 0x00828238 slot 0x08
template void pead::SafeStringBase<char>::assureTerminationImpl_() const;
// 0x0082823C slot 0x08
template void pead::SafeStringBase<wchar_t>::assureTerminationImpl_() const;
// (the copy of BufferedSafeStringBase<char> is folded with the one of sead, 0x00828328)

// 0x00810054
// 0x00810050 (deleting dtor)
template pead::SafeStringBase<char>::~SafeStringBase();
// 0x0081005C
// 0x00810058 (deleting dtor)
template pead::SafeStringBase<wchar_t>::~SafeStringBase();
// 0x0081004C
// 0x00810048 (deleting dtor)
template pead::SafeStringBase<char>::iterator::~iterator();
// 0x008100C4
// 0x008100C0 (deleting dtor)
template pead::BufferedSafeStringBase<char>::~BufferedSafeStringBase();
// 0x008100AC
// 0x008100A8 (deleting dtor)
template pead::FixedSafeStringBase<char, 1024>::~FixedSafeStringBase();
// 0x008100B4
// 0x008100B0 (deleting dtor)
template pead::FixedSafeStringBase<char, 128>::~FixedSafeStringBase();
// 0x008100BC
// 0x008100B8 (deleting dtor)
template pead::FixedSafeStringBase<char, 32>::~FixedSafeStringBase();
// 0x00538984
// 0x00538980 (deleting dtor)
template pead::FixedSafeString<1024>::~FixedSafeString();
// 0x0053898C
// 0x00538988 (deleting dtor)
template pead::FixedSafeString<128>::~FixedSafeString();
// 0x00538994
// 0x00538990 (deleting dtor)
template pead::FixedSafeString<32>::~FixedSafeString();
} // namespace pead
