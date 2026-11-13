#include "Sv/dSvPersonalId.h"

#include <cstring>

#include "netgame/dGameFunctions.h"
#include "script/dWordCPtr.h"
#include "script/dWordPtrSv.h"

// 0x0076D6F0 (name is ours)
DECOMP_NOINLINE bool SvTownId::IsValid() const
{
    return tid != 0;
}

// 0x002FBE08 (name is ours)
bool SvPersonalId::IsValid() const
{
    if (playerData.pid != 0 && townData.IsValid()) {
        return true;
    }
    return false;
}

namespace {
// (inline) the town id is cleared and gets the name (the name is copied and then cut with the word functions)
DECOMP_ALWAYS_INLINE void SetTownName(SvTownId* town, const char16* name)
{
    std::memset(town->dataTownName, 0, sizeof(town->dataTownName));
    town->tid = 0;
    town->unk_0x14 = 10;
    town->unk_0x15 = 0;
    std::memcpy(town->dataTownName, name, sizeof(town->dataTownName));
    const script::WordCPtr source(name, 9);
    script::WordPtrSv word(town->dataTownName, 9);
    static_cast<script::IWord&>(word).Copy(source);
}
} // namespace

// 0x006BC944 (name is ours)
void MakeTownId(SvTownId* town, const char16* name, u32 region)
{
    SetTownName(town, name);
    town->tid = 0xFFFF;
    town->unk_0x14 = region;
}

// 0x006BCA08 (name is ours)
void MakeNewTownId(SvTownId* town, const char16* name, u32 region)
{
    const u16 tid = netgame::Func2FF63C(0x7FFF) | 0x8000;
    SetTownName(town, name);
    town->tid = tid;
    town->unk_0x14 = region;
}

// 0x006BCADC (name is ours)
void MakeTownId(SvTownId* town, const char16* name, u16 tid, u32 region)
{
    SetTownName(town, name);
    town->tid = tid;
    town->unk_0x14 = region;
}
