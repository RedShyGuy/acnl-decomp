#pragma once

#include "decomp.h"

namespace nn {
namespace hidlow {
void ClampStickCross(short*, short*, int, int, int, int); // 0x00483F8C | nintendogs:bytes [tier A]
void ClampStickCircle(short*, short*, int, int, int, int); // 0x00484090 | nintendogs:bytes [tier A]
void ClampStickMinimum(short*, short*, int, int, int, int); // 0x00484174 | nintendogs:bytes [tier A]
void GatherStartAndSelect(nn::hid::CTR::PadStatus*); // 0x00484340 | nintendogs:callgraph [tier A]
void GatherStartAndSelect(unsigned&, unsigned&, unsigned&); // 0x0048434C | nintendogs:bytes [tier A]
} // namespace hidlow
} // namespace nn
