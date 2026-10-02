#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/os/os_HandleObject.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchiveE @ 0x008CDCDC
// vtable 0x008FBF34 (vptr 0x008FBF3C), offset_to_top 0, 15 entries
class FileServerArchive : public ::nn::fs::CTR::MPCore::detail::IArchive, public ::nn::os::HandleObject
{
public:
    class Directory;
    class File;
    FileServerArchive(); // ctor candidate(s) 0x00129584, 0x003467C0, 0x00346A68, 0x00347E94, 0x00348B4C (unverified)
    virtual void OpenFile(nn::fs::CTR::MPCore::detail::IFile**, const nn::fslow::LowPath<const char*, const wchar_t*>&, unsigned); // 0x003486D4 slot 0x00 | nintendogs:callseq
    virtual void OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory**, const nn::fslow::LowPath<const char*, const wchar_t*>&); // 0x0034811C slot 0x04 | nintendogs:callseq
    virtual void vf_0x08(); // 0x00347FD8 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void RenameFile(const nn::fslow::LowPath<const char*,const wchar_t*>&, const nn::fslow::LowPath<const char*,const wchar_t*>&); // 0x00348034 slot 0x0C | slot vf_0x0C of nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void DeleteDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&); // 0x00348224 slot 0x10 | slot vf_0x10 of nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void vf_0x14(); // 0x00348354 slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void CreateFile(const nn::fslow::LowPath<const char*, const wchar_t*>&, long long); // 0x00347F68 slot 0x18 | nintendogs:bytes
    virtual void CreateDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&); // 0x003481C8 slot 0x1C | nintendogs:bytes
    virtual void vf_0x20(); // 0x00348280 slot 0x20 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void vf_0x24(); // 0x00348318 slot 0x24 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void vf_0x28(); // 0x003482E4 slot 0x28 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void vf_0x2C(); // 0x003480E8 slot 0x2C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
    virtual void DeleteObject(); // 0x00348098 slot 0x30 | nintendogs:bytes
    virtual ~FileServerArchive(); // 0x00348938 slot 0x34 | nintendogs:bytes
    virtual void vf_0x38(); // 0x003488DC slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
