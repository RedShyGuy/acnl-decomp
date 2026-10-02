#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
namespace internal {
void EqualsResName(const char*, const char*); // 0x004BEE20 | nintendogs:bytes [tier A]
void EqualsMaterialName(const char*, const char*); // 0x004BEF48 | nintendogs:bytes [tier A]
void UnbindAnimationLink(nw::ut::LinkList<nw::lyt::AnimationLink, (long)0>*, nw::lyt::AnimTransform*); // 0x004BEFD4 | nintendogs:bytes [tier A]
} // namespace internal
} // namespace lyt
} // namespace nw
