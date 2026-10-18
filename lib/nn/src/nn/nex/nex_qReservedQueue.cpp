#include "nn/nex/nex_qReservedQueue.h"

namespace nn {
namespace nex {
// 0x00375FEC
template <>
void nn::nex::qReservedQueue<u8>::push(const u8* pValues, unsigned size)
{
    PushInline(pValues, size);
}

} // namespace nex
} // namespace nn
