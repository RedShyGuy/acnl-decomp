#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Delegate2<BossSys, sead::Thread*, int>  typeinfo 0x008D22FC  vtable 0x00906FB8
//   sead::Delegate2<net::nex::Framework, sead::Thread*, int>  typeinfo 0x008D2308  vtable 0x00906FC8
//   sead::Delegate2<sead::TaskMgr, sead::Thread*, int>  typeinfo 0x008D2320  vtable 0x00906FE8
//   sead::Delegate2<ugc::Text, sead::Thread*, int>  typeinfo 0x008D2314  vtable 0x00906FD8
template <typename T0, typename T1, typename T2>
class Delegate2
{
public:
    // TODO: members unknown
};
} // namespace sead
