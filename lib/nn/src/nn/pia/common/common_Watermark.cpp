#include "nn/pia/common/common_Watermark.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x0042A0B0 (name after C++)
nn::pia::common::Watermark::~Watermark()
{
    // empty (in the original too)
}

// 0x00151168 | fefates:bytes [tier B]
void nn::pia::common::Watermark::SetName(const char* pName)
{
    strncpy(m_Name, pName, NAME_SIZE - 1);
}

// 0x0042A048 | fefates:bytes [tier B]
void nn::pia::common::Watermark::Update(long long value)
{
    if (!m_IsEnabled) {
        return;
    }
    if (value >= m_Max) {
        m_Max = value;
    }
    if (m_Min >= value) {
        m_Min = value;
    }
    m_Current = value;
    m_Count++;
}

} // namespace common
} // namespace pia
} // namespace nn
