#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class WaveArchiveFileReader
{
public:
    WaveArchiveFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    void SetWaveFile(unsigned, const void*); // 0x0013F310 | nintendogs:callseq-callee [tier A]
    WaveArchiveFileReader(const void*, bool); // 0x0013F344 | fefates:bytes [tier B]
    void GetWaveFile(unsigned) const; // 0x0013FE8C | nintendogs:callseq-callee [tier A]
    void HasIndividualLoadTable() const; // 0x0013FEE4 | fefates:bytes [tier B]
    void InitializeFileTable(); // 0x00141914 | nintendogs:callseq-callee [tier A]
    void GetWaveFileSize(unsigned int) const; // 0x00141F30 | fefates:bytes [tier B]
    void GetWaveFileOffsetFromFileHead(unsigned int) const; // 0x00141F58 | fefates:bytes [tier B]
    void Initialize(const void*, bool); // 0x004C980C | fefates:bytes [tier B]
    void Finalize(); // 0x004C98B4 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
