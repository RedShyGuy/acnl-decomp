#include "nw/snd/internal/snd_SequenceSoundFile_LabelBlockBody.h"

#include <cstring>

namespace nw {
namespace snd {
namespace internal {
// 0x007404E0 | nintendogs:bytes [tier A]
bool nw::snd::internal::SequenceSoundFile::LabelBlockBody::GetOffsetByLabel(const char* pLabel, u32* pOffset) const
{
    const size_t length = std::strlen(pLabel);
    for (int i = 0; i < static_cast<int>(labelInfoTable.count); ++i) {
        const LabelInfo* pLabelInfo = static_cast<const LabelInfo*>(labelInfoTable.GetReferedItem(i));
        if (std::strncmp(pLabel, pLabelInfo->name, length) == 0) {
            *pOffset = pLabelInfo->reference.offset;
            return true;
        }
    }
    return false;
}

} // namespace internal
} // namespace snd
} // namespace nw
