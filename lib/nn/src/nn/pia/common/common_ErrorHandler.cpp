#include "nn/pia/common/common_ErrorHandler.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// bits 10-17 of a result (3dbrew: module)
const bit32 RESULT_MODULE_MASK = 0xFF << 10;
} // namespace

// 0x00426CE8 | fefates:callgraph [tier C]
bit32 nn::pia::common::ErrorHandler::TraceResult(unsigned long long, const nn::Result& result)
{
    return result.GetPrintableBits() & RESULT_MODULE_MASK;
}

} // namespace common
} // namespace pia
} // namespace nn
