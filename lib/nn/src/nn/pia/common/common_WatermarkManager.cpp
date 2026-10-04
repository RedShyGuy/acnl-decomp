#include "nn/pia/common/common_WatermarkManager.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097E404
WatermarkManager* WatermarkManager::s_pInstance;

// 0x00427F64 | fefates:bytes [tier B]
Watermark* nn::pia::common::WatermarkManager::GetWatermark(int index)
{
    if (static_cast<u32>(index) >= WATERMARK_NUM) {
        return nullptr;
    }
    return &m_Watermarks[index];
}

// 0x00427F84 | fefates:bytes [tier B]
void nn::pia::common::WatermarkManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00731AFC (name after StepSequenceJob::Trace)
void nn::pia::common::WatermarkManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
