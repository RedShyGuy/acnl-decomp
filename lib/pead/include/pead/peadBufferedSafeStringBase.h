#pragma once

#include "decomp.h"
#include "pead/peadSafeStringBase.h"

#include <cstdarg>
#include <cstring>

namespace pead {
// Instantiations found in the binary:
//   pead::BufferedSafeStringBase<char>  typeinfo 0x008D1240  vtable 0x00904C54
//
// A string in a buffer of mBufferSize characters whose last one is always the null character.
// format/appendWithFormat are the names of the reference symbols, the others are ours.
template <typename T>
class BufferedSafeStringBase : public SafeStringBase<T>
{
public:
    BufferedSafeStringBase(T* buffer, s32 size) : SafeStringBase<T>(buffer), mBufferSize(size)
    {
        assureTerminationImpl_();
    }
    virtual ~BufferedSafeStringBase() {}
   
    virtual void assureTerminationImpl_() const
    {
        getMutableStringTop()[mBufferSize - 1] = SafeStringBase<T>::s_NullChar;
    }

    // replaces the string by the formatted one (cut at the end of the buffer); the length or the
    // cut length
    s32 format(const T* fmt, ...);

    T* getMutableStringTop() const { return const_cast<T*>(this->mStringTop); }
    // the character at index of the buffer; the null character outside the buffer
    const T& at(s32 index) const
    {
        if (index < 0 || index > mBufferSize - 1) {
            return SafeStringBase<T>::s_NullChar;
        }
        return this->mStringTop[index];
    }
    // appends the character c if there is room for it
    void append(T c)
    {
        s32 length = this->getLength();
        if (length < mBufferSize - 1) {
            getMutableStringTop()[length] = c;
            getMutableStringTop()[length + 1] = SafeStringBase<T>::s_NullChar;
        }
    }
    // the string src (cut at the end of the buffer); its length
    s32 copy(const SafeStringBase<T>& src)
    {
        T* dst = getMutableStringTop();
        const T* srcTop = src.getCString();
        if (dst == srcTop) {
            return 0;
        }
        s32 length = src.getLength();
        if (length >= mBufferSize) {
            length = mBufferSize - 1;
        }
        std::memcpy(dst, srcTop, length * sizeof(T));
        dst[length] = SafeStringBase<T>::s_NullChar;
        return length;
    }
    // copies length characters of src (length < 0: all; cut at the end of the buffer); the number
    // of copied characters (the version that is called, the one above is inlined)
    DECOMP_NOINLINE s32 copy(const SafeStringBase<T>& src, s32 length)
    {
        T* dst = getMutableStringTop();
        const T* srcTop = src.getCString();
        if (dst == srcTop) {
            return 0;
        }
        if (length < 0) {
            length = src.getLength();
        }
        if (length >= mBufferSize) {
            length = mBufferSize - 1;
        }
        std::memcpy(dst, srcTop, length * sizeof(T));
        dst[length] = SafeStringBase<T>::s_NullChar;
        return length;
    }
    // writes length characters of src over the string from at on (at < 0: counted from the end);
    // terminated only where the string gets longer
    s32 overwriteAt(s32 at, const SafeStringBase<T>& src, s32 length)
    {
        s32 oldLength = this->getLength();
        if (length < 0) {
            length = src.getLength();
        }
        if (at < 0) {
            at = at + oldLength + 1;
            if (at < 0) {
                at = 0;
            }
        }
        if (mBufferSize - at <= length) {
            length = mBufferSize - at - 1;
        }
        if (length <= 0) {
            return 0;
        }
        std::memcpy(getMutableStringTop() + at, src.getCString(), length * sizeof(T));
        if (at + length > oldLength) {
            getMutableStringTop()[at + length] = SafeStringBase<T>::s_NullChar;
        }
        return length;
    }
    // the empty string
    void clear() { getMutableStringTop()[0] = SafeStringBase<T>::s_NullChar; }
    // appends num times the character c (cut at the end of the buffer); the number appended
    // (name is ours)
    DECOMP_NOINLINE s32 append(T c, s32 num)
    {
        if (num <= 0) {
            return 0;
        }
        s32 length = this->getLength();
        if (mBufferSize - length <= num) {
            num = mBufferSize - length - 1;
        }
        T* top = getMutableStringTop();
        for (s32 i = 0; i < num; i++) {
            top[length + i] = c;
        }
        top[length + num] = SafeStringBase<T>::s_NullChar;
        return num;
    }
    // the terminated buffer
    T* getBuffer()
    {
        this->assureTerminationImpl_();
        return getMutableStringTop();
    }
    // cuts the string after length characters; the new length (the length of the string if length
    // lies beyond the buffer)
    s32 trim(s32 length)
    {
        if (length >= mBufferSize) {
            return this->getLength();
        }
        if (length < 0) {
            length = 0;
        }
        getMutableStringTop()[length] = SafeStringBase<T>::s_NullChar;
        return length;
    }

    // copies length characters of src (length < 0: all) to the position at (cut at the end of the
    // buffer, terminated); the number of copied characters
    s32 copyAt(s32 at, const SafeStringBase<T>& src, s32 length)
    {
        this->assureTerminationImpl_();
        T* dst = nullptr;
        s32 rest = 0;
        if (at < mBufferSize && at >= 0) {
            dst = getMutableStringTop() + at;
            rest = mBufferSize - at;
            dst[rest - 1] = SafeStringBase<T>::s_NullChar;
        }
        const T* srcTop = src.getCString();
        if (dst == srcTop) {
            return 0;
        }
        if (length < 0) {
            length = src.getLength();
        }
        if (length >= rest) {
            length = rest - 1;
        }
        std::memcpy(dst, srcTop, length * sizeof(T));
        dst[length] = SafeStringBase<T>::s_NullChar;
        return length;
    }

    s32 mBufferSize; // 0x08
};

template <>
s32 BufferedSafeStringBase<char>::format(const char* fmt, ...);
} // namespace pead
