#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
class TransportSignatureGenerator
{
public:
    TransportSignatureGenerator(); // 0x003BD454 | fefates:bytes [tier B]

    u32 m_Unknown0x00;                       // 0x00
    ChecksumAlgorithm* m_pChecksumAlgorithm; // 0x04 (an HMACChecksum)
};
} // namespace nex
} // namespace nn
