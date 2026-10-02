#include "nw/snd/internal/snd_WaveArchiveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::internal::WaveArchiveFileReader::WaveArchiveFileReader()
{
}

// 0x0013F310 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::WaveArchiveFileReader::SetWaveFile(unsigned, const void*)
{
}

// 0x0013F344 | fefates:bytes [tier B]
nw::snd::internal::WaveArchiveFileReader::WaveArchiveFileReader(const void*, bool)
{
}

// 0x0013FE8C | nintendogs:callseq-callee [tier A]
void nw::snd::internal::WaveArchiveFileReader::GetWaveFile(unsigned) const
{
}

// 0x0013FEE4 | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::HasIndividualLoadTable() const
{
}

// 0x00141914 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::WaveArchiveFileReader::InitializeFileTable()
{
}

// 0x00141F30 | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::GetWaveFileSize(unsigned int) const
{
}

// 0x00141F58 | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::GetWaveFileOffsetFromFileHead(unsigned int) const
{
}

// 0x004C980C | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::Initialize(const void*, bool)
{
}

// 0x004C98B4 | fefates:bytes [tier B]
void nw::snd::internal::WaveArchiveFileReader::Finalize()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
