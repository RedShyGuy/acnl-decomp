#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common6StringE @ 0x008CFED0
// vtable 0x00901608 (vptr 0x00901610), offset_to_top 0, 1 entries
//
// A string in a fixed buffer of 128 characters. The member name is ours.
class String : public ::nn::pia::common::RootObject
{
public:
    static const int BUFFER_SIZE = 128;

    // (inline, e.g. 0x003E87E8)
    String() { m_Buffer[0] = '\0'; }
    String(const char* str); // 0x004293C0 | fefates:bytes [tier B]

    virtual void Trace(u64 flag) const; // 0x007333D4 slot 0x00 (name after StepSequenceJob::Trace)

    // vsnprintf into the buffer; the length, at most BUFFER_SIZE - 1
    int Format(const char* format, ...); // 0x00429388 | fefates:bytes [tier B]
    size_t StrLen() const; // 0x002FA990 | fefates:bytes [tier B]

    const char* CStr() const { return m_Buffer; }

    char m_Buffer[BUFFER_SIZE]; // 0x04
};
ASSERT_SIZE(String, 0x84);
} // namespace common
} // namespace pia
} // namespace nn
