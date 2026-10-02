#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class PacketEncDec
{
public:
    void GenerateKey(const nn::nex::Key&, const unsigned char*, const unsigned char*, nn::nex::Key*); // 0x0035D6D4 | fefates:bytes [tier B]
    void ReinitUserReliableSubStreams(); // 0x0035DA2C | fefates:bytes [tier B]
    ~PacketEncDec(); // 0x0035E774 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
