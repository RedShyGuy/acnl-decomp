#pragma once

#include "decomp.h"
#include "nn/Handle.h"

namespace nn {
namespace ptm {
namespace CTR {
namespace detail {
// the session of ptm:u (made by ptm::CTR::Initialize; name is ours)
extern nn::Handle s_Session;

// the current time from the clock of the shared page (milliseconds since DateTime::MIN_DATE_TIME),
// kept within 2000-01-01 .. 2100-01-01
s64 GetSwcMilliSeconds(); // 0x0012A4AC | fefates:bytes [tier B]
} // namespace detail
} // namespace CTR
} // namespace ptm
} // namespace nn
