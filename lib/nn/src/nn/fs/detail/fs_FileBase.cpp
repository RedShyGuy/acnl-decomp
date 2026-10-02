#include "nn/fs/detail/fs_FileBaseImpl.h"
#include "nn/fs/detail/fs_FileBase.h"

namespace nn {
namespace fs {
namespace detail {
// ctor address unknown
nn::fs::detail::FileBase::FileBase()
{
}

// 0x0013E6E0 | nintendogs:bytes [tier A]
void nn::fs::detail::FileBase::TryRead(int*, void*, unsigned)
{
}

// 0x0013E768 | fefates:bytes [tier B]
void nn::fs::detail::FileBase::TryWrite(int*, const void*, unsigned int, bool)
{
}

// 0x0013F9B8 | nintendogs:bytes [tier A]
void nn::fs::detail::FileBase::TryGetSize(long long*) const
{
}

// 0x0034971C | fefates:bytes [tier B]
void nn::fs::detail::FileBase::TrySetSize(long long)
{
}

// 0x0034976C | nintendogs:bytes [tier A]
void nn::fs::detail::FileBase::TrySetPosition(long long)
{
}

// 0x0034980C | nintendogs:bytes [tier A]
void nn::fs::detail::FileBase::TrySeek(long long, nn::fs::PositionBase)
{
}

} // namespace detail
} // namespace fs
} // namespace nn
