#include "nn/nex/nex_qResult.h"

// The constructors and the assignment are inline in the header (ARMCC inlines them).

namespace nn {
namespace nex {
// 0x0072E5FC | mk7dlp:bytes [tier A]
bool nn::nex::qResult::Equals(const bool& isSuccess) const
{
    return static_cast<bool>(*this) == isSuccess;
}

// 0x0072E61C (name is ours)
bool nn::nex::qResult::Equals(const int& code) const
{
    return m_Code == code;
}

// 0x0072E634
nn::nex::qResult::operator bool() const
{
    return m_Code >= 0;
}

} // namespace nex
} // namespace nn
