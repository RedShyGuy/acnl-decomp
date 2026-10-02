#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_GroupFile.h"

namespace nw {
namespace snd {
namespace internal {
class GroupFileReader
{
public:
    GroupFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    GroupFileReader(const void*); // 0x0013BB9C | nintendogs:bytes [tier A]
    void GetGroupItemExCount() const; // 0x0013D3A8 | nintendogs:callseq-callee [tier A]
    void ReadGroupItemInfoEx(nw::snd::internal::GroupFile::GroupItemInfoEx*, unsigned) const; // 0x0013D3B8 | nintendogs:bytes [tier A]
    void ReadGroupItemLocationInfo(nw::snd::internal::GroupItemLocationInfo*, unsigned) const; // 0x0013D3F8 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
