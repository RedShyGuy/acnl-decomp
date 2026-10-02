#include "nw/snd/internal/snd_GroupFile.h"
#include "nw/snd/internal/snd_GroupFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::internal::GroupFileReader::GroupFileReader()
{
}

// 0x0013BB9C | nintendogs:bytes [tier A]
nw::snd::internal::GroupFileReader::GroupFileReader(const void*)
{
}

// 0x0013D3A8 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::GroupFileReader::GetGroupItemExCount() const
{
}

// 0x0013D3B8 | nintendogs:bytes [tier A]
void nw::snd::internal::GroupFileReader::ReadGroupItemInfoEx(nw::snd::internal::GroupFile::GroupItemInfoEx*, unsigned) const
{
}

// 0x0013D3F8 | nintendogs:bytes [tier A]
void nw::snd::internal::GroupFileReader::ReadGroupItemLocationInfo(nw::snd::internal::GroupItemLocationInfo*, unsigned) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
