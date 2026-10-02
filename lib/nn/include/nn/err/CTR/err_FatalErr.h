#pragma once

#include "decomp.h"

namespace nn {
namespace err {
namespace CTR {
class FatalErr
{
public:
    void Throw(const nn::err::CTR::FatalErrInfo&); // 0x00130C84 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace err
} // namespace nn
