#pragma once

#include "decomp.h"
#include "nn/dbm/dbm_RomPathTool.h"

class nn::dbm::RomPathTool::PathParser
{
public:
    void Initialize(const wchar_t*); // 0x00351038 | nintendogs:bytes [tier A]
    void GetNextDirectoryName(nn::dbm::RomPathTool::RomEntryName*); // 0x003510A4 | nintendogs:bytes [tier A]
    PathParser(); // 0x0035115C | nintendogs:callgraph [tier A]
    void GetAsFileName(nn::dbm::RomPathTool::RomEntryName*) const; // 0x00729628 | nintendogs:callgraph [tier A]
    void IsDirectoryPath() const; // 0x00729658 | nintendogs:bytes [tier A]
    void GetAsDirectoryName(nn::dbm::RomPathTool::RomEntryName*) const; // 0x007296AC | nintendogs:callgraph [tier A]
};
