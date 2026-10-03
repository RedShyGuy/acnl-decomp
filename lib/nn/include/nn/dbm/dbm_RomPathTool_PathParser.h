#pragma once

#include "decomp.h"
#include "nn/dbm/dbm_RomPathTool.h"

// Splits an absolute path into its names, one directory at a time. The members are ours.
class nn::dbm::RomPathTool::PathParser
{
public:
    PathParser(); // 0x0035115C | nintendogs:callgraph [tier A]
    nn::Result Initialize(const wchar_t* path); // 0x00351038 | nintendogs:bytes [tier A]
    nn::Result GetNextDirectoryName(nn::dbm::RomPathTool::RomEntryName* name); // 0x003510A4 | nintendogs:bytes [tier A]
    nn::Result GetAsFileName(nn::dbm::RomPathTool::RomEntryName* name) const; // 0x00729628 | nintendogs:callgraph [tier A]
    nn::Result GetAsDirectoryName(nn::dbm::RomPathTool::RomEntryName* name) const; // 0x007296AC | nintendogs:callgraph [tier A]
    bool IsDirectoryPath() const; // 0x00729658 | nintendogs:bytes [tier A]
    // inline (name is ours)
    bool IsFinished() const { return mFinished; }

private:
    // the name ends at end, the next one starts after the separators (name is ours)
    void SkipSeparators(const wchar_t* end)
    {
        mPrevEnd = end;
        mNext = end + 1;
        while (*mNext == L'/')
            mNext++;
    }

    const wchar_t* mPrevStart;  // 0x00 the name returned last
    const wchar_t* mPrevEnd;    // 0x04
    const wchar_t* mNext;       // 0x08 the next name
    bool mFinished;             // 0x0C the end of the path is reached
};
ASSERT_SIZE(nn::dbm::RomPathTool::PathParser, 0x10);
