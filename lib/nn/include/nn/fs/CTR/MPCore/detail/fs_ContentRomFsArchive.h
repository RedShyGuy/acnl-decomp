#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail19ContentRomFsArchiveE @ 0x008CDCFC
// vtable 0x008FBF78 (vptr 0x008FBF80), offset_to_top 0, 18 entries
class ContentRomFsArchive : public ::nn::fs::CTR::MPCore::detail::RomFsArchive
{
public:
    virtual void DeleteObject(); // 0x003489DC slot 0x30
    virtual ~ContentRomFsArchive(); // 0x00348AB8 slot 0x34, 0x00348A30 slot 0x38 (deleting)
    virtual nn::Result OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle); // 0x00348990 slot 0x3C
    void AllocateBuffer(); // 0x00130280 | nintendogs:bytes-fuzzy [tier A]
    ContentRomFsArchive(); // 0x00130368 | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
