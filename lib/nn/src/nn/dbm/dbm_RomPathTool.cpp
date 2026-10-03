#include "nn/dbm/dbm_RomPathTool.h"

namespace nn {
namespace dbm {

namespace {
const bit32 RESULT_INVALID_PATH_FORMAT = 0xE0E046C9;  // usage, invalid argument, fs, 713: ".." above the root
} // namespace

// 0x00351174 | nintendogs:bytes [tier A]
nn::Result nn::dbm::RomPathTool::GetParentDirectoryName(RomEntryName* out, const RomEntryName& name, const wchar_t* path)
{
    const wchar_t* start = name.path;
    const wchar_t* end = name.path + name.length - 1;
    // the number of names to go up: one, two for ".."; a "." on the way counts one up, a ".." two
    s32 depth = 1;
    if (IsParentDirectory(name))
        depth = 2;

    if (start > path)
    {
        const wchar_t* p = start - 1;
        u32 count = 0;  // characters of the name before p
        for (; p >= path; p--, count++)
        {
            if (*p != L'/')
                continue;
            if (count == 1)
            {
                if (p[1] == L'.')
                    depth++;
            }
            else if (count == 2)
            {
                if (p[1] == L'.' && p[2] == L'.')
                    depth += 2;
            }
            if (depth == 0)
            {
                start = p + 1;
                break;
            }
            while (*--p == L'/')
            {
            }
            end = p;
            count = 0;
            depth--;
        }
        if (depth != 0)
            return nn::Result(RESULT_INVALID_PATH_FORMAT);
        if (p == path)
            start = path + 1;
    }

    if (end <= path)
    {
        out->length = 0;
        out->path = path;
    }
    else
    {
        out->length = end - start + 1;
        out->path = start;
    }
    return nn::Result();
}

} // namespace dbm
} // namespace nn
