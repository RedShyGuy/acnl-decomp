#include "nn/cec/CTR/CTR_Api.h"

namespace nn {
namespace cec {
namespace CTR {
// 0x001409BC | nintendogs:callgraph [tier A]
void os_free(void*)
{
}

// 0x00144BF0 | nintendogs:callgraph [tier A]
void os_malloc(unsigned, int)
{
}

// 0x0034F408 | fefates:bytes [tier B]
void FinalizeAllocFunc()
{
}

// 0x0034F468 | nintendogs:bytes [tier A]
void Base64Str2CecTitleId(const unsigned char*)
{
}

} // namespace CTR
} // namespace cec
} // namespace nn
