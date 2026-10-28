#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"

#include <cstring>

namespace sead {
// Instantiations found in the binary:
//   sead::BufferedSafeStringBase<char>  typeinfo 0x008D1EFC  vtable 0x009066B0
//   sead::BufferedSafeStringBase<wchar_t>  typeinfo 0x008D1F08  vtable 0x009066C4
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
    // appends the formatted string; the new length
    s32 appendWithFormat(const T* fmt, ...);

    T* getMutableStringTop() const { return const_cast<T*>(this->mStringTop); }

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
template <>
s32 BufferedSafeStringBase<char>::appendWithFormat(const char* fmt, ...);
} // namespace sead
