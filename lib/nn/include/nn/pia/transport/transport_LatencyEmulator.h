#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class LatencyEmulator
{
public:
    LatencyEmulator(); // TODO: default ctor added so derived stubs compile - may not exist
    void writeDispatch(); // 0x004502AC | fefates:bytes [tier B]
    void init(unsigned int); // 0x004504FC | fefates:bytes [tier B]
    void Dispatch(); // 0x00450708 | fefates:bytes [tier B]
    LatencyEmulator(unsigned int); // 0x0045075C | fefates:bytes [tier B]
    ~LatencyEmulator(); // 0x004507C8 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
