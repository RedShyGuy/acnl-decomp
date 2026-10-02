#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
class ExtSaveDataSpecifier
{
public:
    void Make(nn::fs::MediaType, unsigned long long); // 0x001290C0 | nintendogs:bytes [tier A]
};
} // namespace fs
} // namespace nn
