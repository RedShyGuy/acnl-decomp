#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class ConnectionAnalysisData
{
public:
    void Clear(); // 0x0045BDAC | fefates:bytes [tier B]
    void Print(bool) const; // 0x0073658C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
