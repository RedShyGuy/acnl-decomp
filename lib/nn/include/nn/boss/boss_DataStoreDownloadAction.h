#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskAction.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss23DataStoreDownloadActionE @ 0x008D0380
// vtable 0x009021A8 (vptr 0x009021B0), offset_to_top 0, 3 entries
class DataStoreDownloadAction : public ::nn::boss::TaskAction
{
public:
    DataStoreDownloadAction(); // ctor candidate(s) 0x0046C180 (unverified)
    virtual void vf_0x00(); // 0x0046C1A8 slot 0x00 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x04(); // 0x0046C198 slot 0x04 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x08(); // 0x0046BF68 slot 0x08 | virtual slot, introduced by nn::boss::TaskAction
    void Initialize(unsigned int, const wchar_t*); // 0x0046BEE4 | fefates:bytes [tier B]
};
} // namespace boss
} // namespace nn
