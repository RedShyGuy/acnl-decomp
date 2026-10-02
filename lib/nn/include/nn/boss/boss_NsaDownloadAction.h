#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskAction.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss17NsaDownloadActionE @ 0x008D0374
// vtable 0x00902190 (vptr 0x00902198), offset_to_top 0, 4 entries
class NsaDownloadAction : public ::nn::boss::TaskAction
{
public:
    NsaDownloadAction(); // ctor candidate(s) 0x0046BAB4 (unverified)
    virtual void vf_0x00(); // 0x0046A8B0 slot 0x00 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x04(); // 0x0046BACC slot 0x04 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x08(); // 0x0046B210 slot 0x08 | virtual slot, introduced by nn::boss::TaskAction
    virtual void vf_0x0C(); // 0x0046BA4C slot 0x0C | virtual slot, introduced by nn::boss::NsaDownloadAction
    void Initialize(const char*); // 0x0046B9A0 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn
