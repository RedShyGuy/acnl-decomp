#pragma once

#include "decomp.h"

namespace nn {
namespace ptm {
namespace CTR {
namespace detail {
class PtmIpc
{
public:
    void GetTotalStepCount(unsigned*); // 0x0012A7B4 | nintendogs:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace ptm
} // namespace nn
