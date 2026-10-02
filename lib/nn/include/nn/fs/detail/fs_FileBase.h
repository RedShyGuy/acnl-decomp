#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBaseImpl.h"

namespace nn {
namespace fs {
namespace detail {
// RTTI N2nn2fs6detail8FileBaseE @ 0x008CDD54
class FileBase : public ::nn::fs::detail::FileBaseImpl
{
public:
    FileBase(); // ctor address unknown
    void TryRead(int*, void*, unsigned); // 0x0013E6E0 | nintendogs:bytes [tier A]
    void TryWrite(int*, const void*, unsigned int, bool); // 0x0013E768 | fefates:bytes [tier B]
    void TryGetSize(long long*) const; // 0x0013F9B8 | nintendogs:bytes [tier A]
    void TrySetSize(long long); // 0x0034971C | fefates:bytes [tier B]
    void TrySetPosition(long long); // 0x0034976C | nintendogs:bytes [tier A]
    void TrySeek(long long, nn::fs::PositionBase); // 0x0034980C | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace fs
} // namespace nn
