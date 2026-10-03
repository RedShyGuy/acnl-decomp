#include "nn/dbm/dbm_RomPathTool_PathParser.h"

namespace nn {
namespace dbm {

namespace {
// the results of the path tool (usage, invalid argument, fs; descriptions ours)
const bit32 RESULT_FILE_NAME_TOO_LONG = 0xE0E046C7;       // 711: a name over 256 characters
const bit32 RESULT_DIRECTORY_NAME_TOO_LONG = 0xE0E046C8;  // 712
const bit32 RESULT_INVALID_PATH_FORMAT = 0xE0E046C9;      // 713: not absolute
const u32 MAX_NAME_LENGTH = 256;
} // namespace

// 0x0035115C | nintendogs:callgraph [tier A]
nn::dbm::RomPathTool::PathParser::PathParser()
    : mPrevStart(nullptr), mPrevEnd(nullptr), mNext(nullptr), mFinished(false)
{
}

// 0x00351038 | nintendogs:bytes [tier A]
nn::Result nn::dbm::RomPathTool::PathParser::Initialize(const wchar_t* path)
{
    if (path[0] != L'/')
        return nn::Result(RESULT_INVALID_PATH_FORMAT);
    while (path[1] == L'/')
        path++;
    mPrevStart = path;
    SkipSeparators(path);
    return nn::Result();
}

// 0x003510A4 | nintendogs:bytes [tier A]
nn::Result nn::dbm::RomPathTool::PathParser::GetNextDirectoryName(nn::dbm::RomPathTool::RomEntryName* name)
{
    name->length = mPrevEnd - mPrevStart;
    name->path = mPrevStart;
    mPrevStart = mNext;
    const wchar_t* next = mNext;
    for (u32 i = 0;; i++)
    {
        if (next[i] == L'/')
        {
            if (i >= MAX_NAME_LENGTH)
                return nn::Result(RESULT_DIRECTORY_NAME_TOO_LONG);
            SkipSeparators(&next[i]);
            if (*mNext == L'\0')
                mFinished = true;
            return nn::Result();
        }
        if (next[i] == L'\0')
        {
            mPrevEnd = &next[i];
            mFinished = true;
            mNext = &next[i];
            return nn::Result();
        }
    }
}

// 0x00729628 | nintendogs:callgraph [tier A]
nn::Result nn::dbm::RomPathTool::PathParser::GetAsFileName(nn::dbm::RomPathTool::RomEntryName* name) const
{
    u32 length = mPrevEnd - mPrevStart;
    if (length > MAX_NAME_LENGTH)
        return nn::Result(RESULT_FILE_NAME_TOO_LONG);
    name->length = length;
    name->path = mPrevStart;
    return nn::Result();
}

// 0x00729658 | nintendogs:bytes [tier A]
bool nn::dbm::RomPathTool::PathParser::IsDirectoryPath() const
{
    const wchar_t* p = mNext;
    if (p[0] == L'\0' && p[-1] == L'/')
        return true;
    if (p[0] == L'.' && p[1] == L'\0')
        return true;
    if (p[0] == L'.' && p[1] == L'.' && p[2] == L'\0')
        return true;
    return false;
}

// 0x007296AC | nintendogs:callgraph [tier A]
nn::Result nn::dbm::RomPathTool::PathParser::GetAsDirectoryName(nn::dbm::RomPathTool::RomEntryName* name) const
{
    u32 length = mPrevEnd - mPrevStart;
    if (length > MAX_NAME_LENGTH)
        return nn::Result(RESULT_DIRECTORY_NAME_TOO_LONG);
    name->length = length;
    name->path = mPrevStart;
    return nn::Result();
}

} // namespace dbm
} // namespace nn
