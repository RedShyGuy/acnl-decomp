#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class RttCalculator
{
public:
    void Update(int); // 0x0044EDC0 | fefates:bytes [tier B]
    void Cleanup(); // 0x0044EDFC | fefates:bytes [tier B]
    void Startup(); // 0x0044EE2C | fefates:bytes [tier B]
    RttCalculator(); // 0x0044EE54 | fefates:bytes [tier B]
    void GetRtt(unsigned int) const; // 0x00734D58 | fefates:bytes [tier B]
    void GetRtt() const; // 0x00734D80 | fefates:bytes [tier B]
    void IsTimeOut() const; // 0x00734DAC | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
