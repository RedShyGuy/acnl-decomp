#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace fs {
namespace CTR {
// the icon file (SMDH) of the running title: ExeFS "icon" of archive SelfNCCH
nn::Result GetSelfSystemMenuData(nn::CTR::SystemMenuData* data); // 0x00346A68 | nintendogs:bytes-fuzzy [tier A]
} // namespace CTR
} // namespace fs
} // namespace nn
