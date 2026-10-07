#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// The connection status of the local network (LocalNetworkManager keeps the last one).
class LocalConnectionStatus : public ::nn::pia::common::RootObject
{
public:
    // the number of the connected nodes (name is ours)
    virtual u8 GetNodeCount() const = 0; // slot 0x00
};
} // namespace local
} // namespace pia
} // namespace nn
