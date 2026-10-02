#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
namespace internal {
class LinkListImpl
{
public:
    struct iterator { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void erase(nw::ut::LinkListNode*); // 0x00137580 | libgarden [tier A]
    void insert(nw::ut::internal::LinkListImpl::iterator, nw::ut::LinkListNode*); // 0x0013EEFC | libgarden [tier A]
    void erase(nw::ut::internal::LinkListImpl::iterator, nw::ut::internal::LinkListImpl::iterator); // 0x00140EE8 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace ut
} // namespace nw
