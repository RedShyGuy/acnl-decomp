#pragma once

#include "decomp.h"

namespace imgdb {
// Instantiations found in the binary:
//   imgdb::Singleton<imgdb::CtrNandArchiveMounter>  typeinfo 0x008D2C98  vtable 0x009090E8
//   imgdb::Singleton<imgdb::ImageDatabase>  typeinfo 0x008D2C88  vtable 0x009090C8
//   imgdb::Singleton<imgdb::SdArchiveMounter>  typeinfo 0x008D2C90  vtable 0x009090D8
//   imgdb::Singleton<imgdb::TwlNandArchiveMounter>  typeinfo 0x008D2CA0  vtable 0x009090F8
template <typename T0>
class Singleton
{
public:
    // TODO: members unknown
};
} // namespace imgdb
