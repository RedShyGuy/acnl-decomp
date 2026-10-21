#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
// The signatures of the packets of a connection: an HMAC with a random key (the checksum of the
// time when it was made). No RTTI; the class name is from the fefates symbols, the member names
// are ours.
class TransportSignatureGenerator
{
public:
    TransportSignatureGenerator(); // 0x003BD454 | fefates:bytes [tier B]

    u32 m_Unknown0x00;                       // 0x00 (not set by the constructor)
    ChecksumAlgorithm* m_pChecksumAlgorithm; // 0x04 (an HMACChecksum)
};
} // namespace nex
} // namespace nn
