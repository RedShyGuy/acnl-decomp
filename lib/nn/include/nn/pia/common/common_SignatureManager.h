#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class SignatureManager
{
public:
    class Specified;
    void SetNecessity(bool); // 0x00427680 | fefates:bytes [tier B]
    void CheckSignature(const nn::pia::common::StationAddress&, const void*, unsigned int, unsigned int*); // 0x004276A8 | fefates:bytes [tier B]
    void CreateInstance(); // 0x004277FC | fefates:bytes [tier B]
    void ResetNecessity(); // 0x004278B8 | fefates:bytes [tier B]
    void AppendSignature(const nn::pia::common::StationAddress&, void*, unsigned int, unsigned int); // 0x00427930 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x004279F0 | fefates:bytes [tier B]
    void UpdateSignature(const nn::pia::common::StationAddress&, void*, unsigned int); // 0x00427A38 | fefates:bytes [tier B]
    void Cleanup(); // 0x00427AC0 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
