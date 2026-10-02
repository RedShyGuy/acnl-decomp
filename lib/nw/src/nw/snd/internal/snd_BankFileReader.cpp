#include "nw/snd/internal/snd_BankFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::internal::BankFileReader::BankFileReader()
{
}

// 0x0013BB38 | nintendogs:callseq-callee [tier A]
nw::snd::internal::BankFileReader::BankFileReader(const void*)
{
}

// 0x0013D38C | nintendogs:callseq-callee [tier A]
void nw::snd::internal::BankFileReader::GetWaveIdTable() const
{
}

// 0x004C8164 | fefates:bytes [tier B]
void nw::snd::internal::BankFileReader::Initialize(const void*)
{
}

// 0x004C81B8 | fefates:bytes [tier B]
void nw::snd::internal::BankFileReader::Finalize()
{
}

// 0x0073F6E0 | nintendogs:callseq [tier A]
void nw::snd::internal::BankFileReader::ReadVelocityRegionInfo(nw::snd::internal::VelocityRegionInfo*, int, int, int) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
