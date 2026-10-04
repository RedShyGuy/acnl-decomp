#include "nn/pia/common/common_Log.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097E3CC
Log* Log::s_pInstance;

// 0x00428BAC | fefates:bytes [tier B]
nn::pia::common::Log::Log() : m_Unknown0x0(0xFFFFFFFF), m_Unknown0x10(false)
{
    m_StartTime.SetNow();
}

} // namespace common
} // namespace pia
} // namespace nn
