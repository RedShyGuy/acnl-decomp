#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt5GroupE @ 0x008D0874
// vtable 0x00902C10 (vptr 0x00902C18), offset_to_top 0, 2 entries
class Group
{
public:
    Group(); // ctor candidate(s) 0x004B8550 (unverified)
    virtual ~Group(); // 0x004B8698 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004B8640 slot 0x04 | virtual slot, introduced by nw::lyt::Group
    Group(const nw::lyt::res::Group*, nw::lyt::Pane*); // 0x004B8550 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
