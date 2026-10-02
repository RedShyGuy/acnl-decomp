#pragma once

#include "decomp.h"

namespace mutil {
// Instantiations found in the binary:
//   mutil::BaseResourceMgr<FgDataLoader, 18u>::ResourceNode  typeinfo 0x008D2D04  vtable 0x0090918C
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 13u>::ResourceNode  typeinfo 0x008D2D10  vtable 0x0090919C
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 15u>::ResourceNode  typeinfo 0x008D2D1C  vtable 0x009091AC
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 1u>::ResourceNode  typeinfo 0x008D2D28  vtable 0x009091BC
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 2u>::ResourceNode  typeinfo 0x008D2D34  vtable 0x009091CC
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 3u>::ResourceNode  typeinfo 0x008D2D40  vtable 0x009091DC
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 4u>::ResourceNode  typeinfo 0x008D2D4C  vtable 0x009091EC
//   mutil::BaseResourceMgr<g3d::ResourceLoader, 64u>::ResourceNode  typeinfo 0x008D2D58  vtable 0x009091FC
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 110u>::ResourceNode  typeinfo 0x008D2D64  vtable 0x0090920C
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 18u>::ResourceNode  typeinfo 0x008D2D70  vtable 0x0090921C
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 20u>::ResourceNode  typeinfo 0x008D2D7C  vtable 0x0090922C
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 31u>::ResourceNode  typeinfo 0x008D2D88  vtable 0x0090923C
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 3u>::ResourceNode  typeinfo 0x008D2D94  vtable 0x0090924C
//   mutil::BaseResourceMgr<mutil::LoaderWrapper, 62u>::ResourceNode  typeinfo 0x008D2DA0  vtable 0x0090925C
template <typename T0, auto T1>
class BaseResourceMgr
{
public:
    // TODO: members unknown
};
} // namespace mutil
