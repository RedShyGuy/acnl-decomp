#include "nn/pia/common/common_ChunkOld.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097F9E8
unsigned int ChunkOld::s_DataSizeLimit = DATA_SIZE_LIMIT_MAX;

// 0x004293F0 | fefates:bytes [tier B]
nn::Result nn::pia::common::ChunkOld::SetDataSizeLimit(unsigned int size)
{
    if (size > DATA_SIZE_LIMIT_MAX) {
        return RESULT_INVALID_ARGUMENT;
    }
    s_DataSizeLimit = size;
    return nn::Result();
}

// 0x00429418 (name is ours)
bool nn::pia::common::ChunkOld::IsValidDataSizeLimit(unsigned int size)
{
    return size <= DATA_SIZE_LIMIT_MAX;
}

} // namespace common
} // namespace pia
} // namespace nn
