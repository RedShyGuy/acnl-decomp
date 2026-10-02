#include "nn/err/CTR/CTR_Api.h"

namespace nn {
namespace err {
namespace CTR {
// 0x00129E04 | nintendogs:bytes-fuzzy [tier A]
void ThrowFatalErr(nn::Result, nnerrFatalErrType, unsigned)
{
}

// 0x00129E3C | nintendogs:bytes [tier A]
void ThrowFatalErr(nn::Result, unsigned)
{
}

// 0x00129E84 | nintendogs:bytes [tier A]
void ThrowFatalErrAll(nn::Result, unsigned)
{
}

} // namespace CTR
} // namespace err
} // namespace nn
