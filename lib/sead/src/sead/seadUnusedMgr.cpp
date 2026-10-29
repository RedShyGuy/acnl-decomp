#include "sead/seadUnusedMgr.h"

namespace sead {
// (never set in the binary)
// 0x009762F8
UnusedMgr* UnusedMgr::s_pInstance = nullptr;

// 0x00540B90 (name is ours)
void sead::UnusedMgr::setPair0x134(u32 first, u32 second)
{
    field_0x134[0] = first;
    field_0x134[1] = second;
}

// 0x00540B9C (name is ours)
void sead::UnusedMgr::setPair0x12C(u32 first, u32 second)
{
    field_0x12C[0] = first;
    field_0x12C[1] = second;
}
} // namespace sead
