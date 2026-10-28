#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::SafeStringBase<char>  typeinfo 0x008D1758  vtable 0x0090591C
//   sead::SafeStringBase<char>::iterator  typeinfo 0x008D1750  vtable 0x0090590C
//   sead::SafeStringBase<wchar_t>  typeinfo 0x008D1774  vtable 0x00905940
//   sead::SafeStringBase<wchar_t>::iterator  typeinfo 0x008D176C
//   sead::SafeStringBase<wchar_t>::token_iterator  typeinfo 0x008D1760  vtable 0x00905930
//
// A string with a vtable: a pointer to the characters. The derived buffers (BufferedSafeStringBase)
// keep their characters terminated by slot 0x08 (assureTerminationImpl_, the name of the reference
// symbols of mk7dlp; symbols_us calls the base's copy SetNT). The class names are from the RTTI, the
// member names and the names of the inline functions are ours.
template <typename T>
class SafeStringBase
{
public:
    // the iterator over the characters; only its destructors are in the binary (the members are unknown)
    class iterator
    {
    public:
        virtual ~iterator() {}
    };

    // the iterator over the tokens between separators; only its destructors are in the binary
    class token_iterator : public iterator
    {
    public:
        virtual ~token_iterator() {}
    };

    // the longest string whose length is counted (getLength gives 0 above it)
    static const s32 MAX_LENGTH = 0x80000;

    SafeStringBase(const T* str) : mStringTop(str) {}
    virtual ~SafeStringBase() {}
    // makes sure the characters end with a null character (nothing to do for a string literal)
    virtual void assureTerminationImpl_() const {}

    // the terminated characters (the copy of <char> is at 0x0030F648)
    const T* getCString() const
    {
        assureTerminationImpl_();
        return mStringTop;
    }

    // the number of characters before the null character, 0 for a string longer than MAX_LENGTH
    s32 getLength() const
    {
        assureTerminationImpl_();
        s32 length = 0;
        while (mStringTop[length] != s_NullChar) {
            length++;
            if (length > MAX_LENGTH) {
                break;
            }
        }
        if (length > MAX_LENGTH) {
            return 0;
        }
        return length;
    }

    // the null character of the type (in .rodata)
    static const T s_NullChar;

    const T* mStringTop; // 0x04
};

template <>
const char SafeStringBase<char>::s_NullChar;
template <>
const wchar_t SafeStringBase<wchar_t>::s_NullChar;
} // namespace sead
