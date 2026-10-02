#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class BankFileReader
{
public:
    BankFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    BankFileReader(const void*); // 0x0013BB38 | nintendogs:callseq-callee [tier A]
    void GetWaveIdTable() const; // 0x0013D38C | nintendogs:callseq-callee [tier A]
    void Initialize(const void*); // 0x004C8164 | fefates:bytes [tier B]
    void Finalize(); // 0x004C81B8 | fefates:bytes [tier B]
    void ReadVelocityRegionInfo(nw::snd::internal::VelocityRegionInfo*, int, int, int) const; // 0x0073F6E0 | nintendogs:callseq [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
