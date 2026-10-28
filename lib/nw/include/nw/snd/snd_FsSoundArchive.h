#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

namespace nw {
namespace snd {
// A sound archive in a file system whose errors are not fatal (the class name is from the reference symbols).
// RTTI N2nw3snd14FsSoundArchiveE @ 0x008D08E8
// vtable 0x00902EFC (vptr 0x00902F04), offset_to_top 0, 6 entries
class FsSoundArchive : public ::nw::snd::internal::FsSoundArchiveBase
{
public:
    FsSoundArchive(); // 0x004C0358 (symbols.json: [ctor?])
    // 0x004C0374 (deleting dtor)
    virtual ~FsSoundArchive(); // 0x004C8A5C slot 0x00
};
} // namespace snd
} // namespace nw
