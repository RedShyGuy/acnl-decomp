#include "nn/pia/common/common_Trace.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097E400
Trace* Trace::s_pInstance;

// 0x00428DD8 | fefates:callgraph [tier C]
void nn::pia::common::Trace::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

} // namespace common
} // namespace pia
} // namespace nn
