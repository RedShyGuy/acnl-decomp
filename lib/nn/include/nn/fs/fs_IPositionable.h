#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs13IPositionableE @ 0x008CDC18
// The position and size part of the streams: the virtual base of IInputStream and IOutputStream
// (it shares their vptr). Each function comes as Try* with a result and as one that stops the
// program on a failure. The names of the functions are ours (after detail::FileBase).
class IPositionable
{
public:
    virtual ~IPositionable() {} // slots 0x00, 0x04
    virtual nn::Result TrySeek(s64 offset, PositionBase base) = 0; // slot 0x08
    virtual void Seek(s64 offset, PositionBase base) = 0; // slot 0x0C
    virtual nn::Result TryGetPosition(s64* position) const = 0; // slot 0x10
    virtual s64 GetPosition() const = 0; // slot 0x14
    virtual nn::Result TrySetPosition(s64 position) = 0; // slot 0x18
    virtual void SetPosition(s64 position) = 0; // slot 0x1C
    virtual nn::Result TryGetSize(s64* size) const = 0; // slot 0x20
    virtual s64 GetSize() const = 0; // slot 0x24
};
} // namespace fs
} // namespace nn
