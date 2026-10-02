#include "nw/lyt/lyt_ResourceAccessor.h"
#include "nw/lyt/lyt_ArcResourceAccessor.h"

namespace nw {
namespace lyt {
// ctor candidate(s) 0x0012BF18 (unverified)
nw::lyt::ArcResourceAccessor::ArcResourceAccessor()
{
}

// 0x004B6D04 slot 0x00 | slot vf_0x00 of nw::lyt::ResourceAccessor
nw::lyt::ArcResourceAccessor::~ArcResourceAccessor()
{
}

// 0x004B6BCC slot 0x08 | virtual slot, introduced by nw::lyt::ResourceAccessor
void nw::lyt::ArcResourceAccessor::vf_0x08()
{
}

// 0x004B6C84 slot 0x0C | virtual slot, introduced by nw::lyt::ResourceAccessor
void nw::lyt::ArcResourceAccessor::vf_0x0C()
{
}

// 0x004B6B64 slot 0x10 | libgarden
void nw::lyt::ArcResourceAccessor::GetTexture(char const*)
{
}

// 0x004B52F4 | libgarden [tier A]
void nw::lyt::ArcResourceAccessor::RegistFont(char const*, nw::font::Font const*)
{
}

} // namespace lyt
} // namespace nw
