#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_GroupFile.h"

namespace nw {
namespace snd {
namespace internal {
// Reads the files of a group file (the class and function names are from the reference symbols, the
// members are ours).
class GroupFileReader
{
public:
    GroupFileReader(const void* pGroupFile); // 0x0013BB9C | nintendogs:bytes [tier A]

    // the items of the info ex block (0: none)
    u32 GetGroupItemExCount() const; // 0x0013D3A8 | nintendogs:callseq-callee [tier A]
    bool ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* pInfo, u32 index) const; // 0x0013D3B8 | nintendogs:bytes [tier A]
    bool ReadGroupItemLocationInfo(GroupItemLocationInfo* pInfo, u32 index) const; // 0x0013D3F8 | nintendogs:bytes [tier A]

    const BinaryReferenceTable* m_pInfoBlockBody;   // 0x0 GroupFile::GroupItemInfo
    const void* m_pFileBlockBody;                  // 0x4
    const BinaryReferenceTable* m_pInfoExBlockBody; // 0x8 GroupFile::GroupItemInfoEx (NULL: none)
};
ASSERT_SIZE(GroupFileReader, 0xC);
} // namespace internal
} // namespace snd
} // namespace nw
