#include "nn/pia/common/common_String.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x002FA990 | fefates:bytes [tier B]
size_t nn::pia::common::String::StrLen() const
{
    return strlen(m_Buffer);
}

// 0x00429388 | fefates:bytes [tier B]
int nn::pia::common::String::Format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int length = vsnprintf(m_Buffer, BUFFER_SIZE, format, args);
    va_end(args);
    if (length >= BUFFER_SIZE) {
        m_Buffer[BUFFER_SIZE - 1] = '\0';
        length = BUFFER_SIZE - 1;
    }
    return length;
}

// 0x004293C0 | fefates:bytes [tier B]
nn::pia::common::String::String(const char* str)
{
    strncpy(m_Buffer, str, BUFFER_SIZE - 1);
    m_Buffer[BUFFER_SIZE - 1] = '\0';
}

// 0x007333D4 (name after StepSequenceJob::Trace)
void nn::pia::common::String::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
