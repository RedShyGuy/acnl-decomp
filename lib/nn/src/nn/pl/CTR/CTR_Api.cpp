#include "nn/pl/CTR/CTR_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/ptm/CTR/detail/ptm_PtmIpc.h"

namespace nn {
namespace pl {
namespace CTR {

// the start of the shared font memory block (names are ours)
struct SharedFontHeader
{
    u32 unknown0;           // 0x0
    u32 unknown4;           // 0x4
    u32 size;               // 0x8 size of the font data
    u8 unknown0C[0x74];     // 0xC
    // 0x80 the font data
};
ASSERT_SIZE(SharedFontHeader, 0x80);

// the mapped shared font; nothing in this program sets it (name is ours)
// 0x00982638
SharedFontHeader* s_pSharedFont;

// 0x00123DC8 (name is ours, after PTM:GetStepHistory)
void GetStepHistory(u16* steps, u32 hours, s64 start)
{
    if (nn::ptm::CTR::detail::PtmIpc::GetStepHistory(steps, hours, start).IsFailure()) {
        nndbgPanic();
    }
}

// 0x00123DE4 | nintendogs:bytes [tier B]
u32 GetTotalStepCount()
{
    u32 count = 0;
    nn::ptm::CTR::detail::PtmIpc::GetTotalStepCount(&count);
    return count;
}

// 0x00143234 | fefates:bytes [tier B]
u32 GetSharedFontSize()
{
    if (s_pSharedFont == 0) {
        return 0;
    }
    return s_pSharedFont->size;
}

// 0x0014324C | fefates:bytes [tier B]
void* GetSharedFontAddress()
{
    if (s_pSharedFont == 0) {
        return 0;
    }
    return s_pSharedFont + 1;
}

} // namespace CTR
} // namespace pl
} // namespace nn
