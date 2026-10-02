#include "nn/util/util_Crc32.h"

namespace nn {
namespace util {

// 0x0047F0F0 (name is ours)
u32 nn::util::Crc32Msb::Calculate(const void* data, size_t size, u32 context)
{
    Crc32Msb crc;
    crc.InitializeContext(context);
    crc.Update(data, size);
    return crc.GetHash();
}

// 0x0047F1B8 | fefates:bytes [tier B]
u32 nn::util::Crc32::Calculate(const void* data, size_t size, u32 context)
{
    Crc32 crc;
    crc.InitializeContext(context);
    crc.Update(data, size);
    return crc.GetHash();
}

} // namespace util
} // namespace nn
