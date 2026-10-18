#include "nn/nex/nex_IORequestContext.h"

namespace nn {
namespace nex {
// 0x0037F25C slot 0x00
// 0x0037F1CC (deleting dtor)
nn::nex::IORequestContext::~IORequestContext()
{
    Clear();
}

} // namespace nex
} // namespace nn
