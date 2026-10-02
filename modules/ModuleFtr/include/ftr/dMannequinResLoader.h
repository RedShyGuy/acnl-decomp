#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"

namespace ftr {
// vtable +0x2BA00 in ModuleFtr.cro, offset_to_top 0, 2 entries
class MannequinResLoader : public ::g3d::ResourceLoader
{
public:
    MannequinResLoader(); // ctor address unknown
};
} // namespace ftr
