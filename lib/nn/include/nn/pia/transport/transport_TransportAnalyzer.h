#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class TransportAnalyzer
{
public:
    void DestroyInstance(); // 0x004574BC | fefates:bytes [tier B]
    void Finalize(); // 0x00457924 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
