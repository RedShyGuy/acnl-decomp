#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace dbm {
// Path helpers of the RomFs file table (paths are wchar_t, '/' separated).
class RomPathTool
{
public:
    class PathParser;

    // One name inside a path (not terminated). The members are ours.
    struct RomEntryName
    {
        u32 length;           // 0x00 in characters
        const wchar_t* path;  // 0x04
    };

    // "." and ".." (inline; names are ours)
    static bool IsCurrentDirectory(const RomEntryName& name)
    {
        return name.length == 1 && name.path[0] == L'.';
    }
    static bool IsParentDirectory(const RomEntryName& name)
    {
        return name.length == 2 && name.path[0] == L'.' && name.path[1] == L'.';
    }

    // The name of the directory that contains name (path is the whole path that name points into);
    // "." and ".." are resolved.
    static nn::Result GetParentDirectoryName(RomEntryName* out, const RomEntryName& name, const wchar_t* path); // 0x00351174 | nintendogs:bytes [tier A]
};
ASSERT_SIZE(RomPathTool::RomEntryName, 0x8);
} // namespace dbm
} // namespace nn
