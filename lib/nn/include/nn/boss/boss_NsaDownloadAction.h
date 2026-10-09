#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskAction.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss17NsaDownloadActionE @ 0x008D0374
// Downloads an NSA archive from a URL (action code 2).
class NsaDownloadAction : public ::nn::boss::TaskAction
{
public:
    NsaDownloadAction();
    virtual ~NsaDownloadAction();
    virtual nn::Result GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size);
    // the name of the slot is ours
    virtual nn::Result SetProperty(nn::boss::PropertyType type, const void* pValue, unsigned size);

    nn::Result Initialize(const char* pUrl); // 0x0046B9A0 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn
