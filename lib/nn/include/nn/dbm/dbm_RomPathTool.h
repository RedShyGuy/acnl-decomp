#pragma once

#include "decomp.h"

namespace nn {
namespace dbm {
class RomPathTool
{
public:
    class PathParser;
    struct RomEntryName { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void GetParentDirectoryName(nn::dbm::RomPathTool::RomEntryName*, const nn::dbm::RomPathTool::RomEntryName&, const wchar_t*); // 0x00351174 | nintendogs:bytes [tier A]
};
} // namespace dbm
} // namespace nn
