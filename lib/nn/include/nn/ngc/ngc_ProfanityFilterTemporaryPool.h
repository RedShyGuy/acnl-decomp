#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class ProfanityFilterTemporaryPool
{
public:
    void Initialize(unsigned int); // 0x003DFD50 | fefates:bytes [tier B]
    void Free(void*, unsigned int); // 0x003DFD70 | fefates:bytes [tier B]
    void Allocate(unsigned int); // 0x003DFDA4 | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
