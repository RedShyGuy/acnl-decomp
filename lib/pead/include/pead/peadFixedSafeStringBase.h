#pragma once

#include "decomp.h"
#include "pead/peadBufferedSafeStringBase.h"

namespace pead {
// Instantiations found in the binary:
//   pead::FixedSafeStringBase<char, 1024>  typeinfo 0x008D121C  vtable 0x00904C18
//   pead::FixedSafeStringBase<char, 128>  typeinfo 0x008D1228  vtable 0x00904C2C
//   pead::FixedSafeStringBase<char, 32>  typeinfo 0x008D1234  vtable 0x00904C40
//
// A string with a buffer of N characters of its own (empty at first). The member name is ours.
template <typename T, int N>
class FixedSafeStringBase : public BufferedSafeStringBase<T>
{
public:
    FixedSafeStringBase() : BufferedSafeStringBase<T>(mBuffer, N)
    {
        this->getMutableStringTop()[0] = SafeStringBase<T>::s_NullChar;
    }
    explicit FixedSafeStringBase(const SafeStringBase<T>& str) : BufferedSafeStringBase<T>(mBuffer, N)
    {
        this->copy(str);
    }
    virtual ~FixedSafeStringBase() {}
   

    T mBuffer[N]; // 0x0C
};
} // namespace pead
