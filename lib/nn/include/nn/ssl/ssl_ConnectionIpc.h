#pragma once

#include "decomp.h"

namespace nn {
namespace ssl {
class ConnectionIpc
{
public:
    void GenerateRandomBytes(unsigned char*, unsigned int); // 0x004672D4 | fefates:bytes [tier A]
    void InitializeGeneralSession(); // 0x00467314 | fefates:bytes [tier A]
};
} // namespace ssl
} // namespace nn
