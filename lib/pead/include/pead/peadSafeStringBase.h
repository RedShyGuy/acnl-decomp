#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::SafeStringBase<char>  typeinfo 0x008D1198  vtable 0x00904A88
//   pead::SafeStringBase<char>::iterator  typeinfo 0x008D1190  vtable 0x00904A78
//   pead::SafeStringBase<wchar_t>  typeinfo 0x008D11A0  vtable 0x00904A9C
//
// pia's copy of sead::SafeStringBase (same code). A string with a vtable: a pointer to the characters. The derived buffers (BufferedSafeStringBase)
// keep their characters terminated by slot 0x08 (assureTerminationImpl_, the name of the reference
// symbols of mk7dlp; symbols_us calls the base's copy SetNT). The class names are from the RTTI, the
// member names and the names of the inline functions are ours.
template <typename T>
class SafeStringBase
{
public:
    // the iterator over the characters (the parsers of StringUtil walk with it)
    class iterator
    {
    public:
        explicit iterator(const SafeStringBase* string) : mString(string), mIndex(0) {}
        virtual ~iterator() {}
       

        const T& operator*() const { return mString->at(mIndex); }
        iterator& operator++()
        {
            mIndex++;
            return *this;
        }

        const SafeStringBase* mString; // 0x04
        s32 mIndex;                    // 0x08
    };

    // the longest string whose length is counted (getLength gives 0 above it)
    static const s32 MAX_LENGTH = 0x40000; // (sead: 0x80000)

    SafeStringBase(const T* str) : mStringTop(str) {}
    virtual ~SafeStringBase() {}
   
    // makes sure the characters end with a null character (nothing to do for a string literal)
    virtual void assureTerminationImpl_() const {}

    // the terminated characters
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

    // the character at index; the null character outside the string (its end included)
    const T& at(s32 index) const
    {
        s32 length = getLength();
        if (index < 0 || index > length) {
            return s_NullChar;
        }
        return mStringTop[index];
    }

    // whether the string equals str
    bool isEqual(const SafeStringBase& str) const
    {
        const T* a = getCString();
        const T* b = str.getCString();
        if (a == b) {
            return true;
        }
        for (s32 i = 0; i <= MAX_LENGTH; i++) {
            if (a[i] != b[i]) {
                return false;
            }
            if (a[i] == s_NullChar) {
                return true;
            }
        }
        return false;
    }
    // whether the first length characters equal those of str (or both end before)
    bool isEqualHead(const SafeStringBase& str, s32 length) const
    {
        const T* a = getCString();
        const T* b = str.getCString();
        if (a == b) {
            return true;
        }
        if (length > MAX_LENGTH) {
            length = MAX_LENGTH;
        }
        for (s32 i = 0; i < length; i++) {
            if (a[i] == s_NullChar) {
                return b[i] == s_NullChar;
            }
            if (b[i] == s_NullChar || a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
    // the string from index on (the empty string if index lies outside)
    DECOMP_NOINLINE SafeStringBase getPart(s32 index) const
    {
        s32 length = getLength();
        if (index < 0 || index > length) {
            return SafeStringBase(s_EmptyString.mStringTop);
        }
        return SafeStringBase(mStringTop + index);
    }
    // the position of the first occurrence of str, -1 if there is none
    DECOMP_NOINLINE s32 findIndex(const SafeStringBase& str) const
    {
        s32 length = getLength();
        s32 strLength = str.getLength();
        for (s32 i = 0; i <= length - strLength; i++) {
            if (SafeStringBase(mStringTop + i).isEqualHead(str, strLength)) {
                return i;
            }
        }
        return -1;
    }
    // the position of the last occurrence of str, -1 if there is none (inline; name is ours)
    s32 rfindIndex(const SafeStringBase& str) const
    {
        s32 length = getLength();
        s32 strLength = str.getLength();
        for (s32 i = length - strLength; i >= 0; i--) {
            if (SafeStringBase(mStringTop + i).isEqualHead(str, strLength)) {
                return i;
            }
        }
        return -1;
    }
    // whether the character c is in the string
    bool include(T c) const
    {
        const T* top = getCString();
        for (s32 i = 0; i <= MAX_LENGTH; i++) {
            if (top[i] == s_NullChar) {
                return false;
            }
            if (top[i] == c) {
                return true;
            }
        }
        return false;
    }

    // the empty string (name is ours)
    static const SafeStringBase s_EmptyString;
    // the null character of the type (in .rodata)
    // (unlike sead the value is known in the header: the inline functions compare with 0, only a reference loads
    // the object)
    static const T s_NullChar = 0;

    const T* mStringTop; // 0x04
};

template <typename T>
const T SafeStringBase<T>::s_NullChar;
template <>
const SafeStringBase<char> SafeStringBase<char>::s_EmptyString;
template <>
const SafeStringBase<wchar_t> SafeStringBase<wchar_t>::s_EmptyString;
} // namespace pead
