#include "nn/hidlow/hidlow_Api.h"

namespace nn {
namespace hidlow {
// 0x00483F8C | nintendogs:bytes [tier A]
void ClampStickCross(short*, short*, int, int, int, int)
{
}

// 0x00484090 | nintendogs:bytes [tier A]
void ClampStickCircle(short*, short*, int, int, int, int)
{
}

// 0x00484174 | nintendogs:bytes [tier A]
void ClampStickMinimum(short*, short*, int, int, int, int)
{
}

// 0x00484340 | nintendogs:callgraph [tier A]
void GatherStartAndSelect(nn::hid::CTR::PadStatus*)
{
}

// 0x0048434C | nintendogs:bytes [tier A]
void GatherStartAndSelect(unsigned&, unsigned&, unsigned&)
{
}

} // namespace hidlow
} // namespace nn
