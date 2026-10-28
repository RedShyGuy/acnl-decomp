#include "nw/snd/snd_FsSoundArchive.h"

namespace nw {
namespace snd {
// 0x004C0358 (symbols.json: [ctor?])
nw::snd::FsSoundArchive::FsSoundArchive() : FsSoundArchiveBase(true)
{
}

// 0x004C8A5C slot 0x00
// 0x004C0374 (deleting dtor)
nw::snd::FsSoundArchive::~FsSoundArchive()
{
    // empty (in the original too)
}

} // namespace snd
} // namespace nw
