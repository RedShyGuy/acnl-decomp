#include "ssys/st/dListNode.h"
#include "ssys/ma/dLoadSplit.h"

namespace ssys {
namespace ma {
// ctor address unknown
ssys::ma::LoadSplit::LoadSplit()
{
}

// 0x0013CA08 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
ssys::ma::LoadSplit::~LoadSplit()
{
}

// 0x0056B854 slot 0x08 | virtual slot, introduced by ssys::ma::LoadSplit
void ssys::ma::LoadSplit::vf_0x08()
{
}

// 0x0056B858 slot 0x0C | virtual slot, introduced by ssys::ma::LoadSplit
void ssys::ma::LoadSplit::vf_0x0C()
{
}

// 0x0056B240 | libgarden [tier A]
void ssys::ma::LoadSplit::LoadBuffered(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int)
{
}

// 0x0056B484 | libgarden [tier A]
void ssys::ma::LoadSplit::Read(sead::SafeStringBase<char const> const&, sead::Heap*, unsigned long, unsigned long)
{
}

} // namespace ma
} // namespace ssys
