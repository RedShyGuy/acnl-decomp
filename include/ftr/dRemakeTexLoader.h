#pragma once

#include "decomp.h"
#include "item/dResLoader.h"

namespace ftr {
// RTTI N3ftr15RemakeTexLoaderE @ 0x008D0C64
// vtable 0x00903924 (vptr 0x0090392C), offset_to_top 0, 2 entries
class RemakeTexLoader : public ::item::ResLoader
{
public:
    RemakeTexLoader(); // ctor candidate(s) 0x004E39B4 (unverified)
    virtual void vf_0x00(); // 0x005378C4 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
    virtual void vf_0x04(); // 0x004E39CC slot 0x04 | virtual slot, introduced by g3d::ResourceSet
};
} // namespace ftr
