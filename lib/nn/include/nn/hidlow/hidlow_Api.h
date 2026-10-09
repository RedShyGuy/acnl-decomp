#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"

namespace nn {
namespace hidlow {
// The clamping of the circle pad: the stick values beyond the dead zone (min) are scaled so that
// the maximum (max) gives 128 (cross, circle) or the remaining range (minimum). The parameter
// names are ours; Cross and Circle return the used range.
s16 ClampStickCross(s16* pX, s16* pY, int x, int y, int min, int max); // 0x00483F8C | nintendogs:bytes [tier A]
s16 ClampStickCircle(s16* pX, s16* pY, int x, int y, int min, int max); // 0x00484090 | nintendogs:bytes [tier A]
// (min is not used)
void ClampStickMinimum(s16* pX, s16* pY, int x, int y, int min, int max); // 0x00484174 | nintendogs:bytes [tier A]
// SELECT counts as START: START is set when only one of the two is held, SELECT is cleared
void GatherStartAndSelect(nn::hid::CTR::PadStatus* pStatus); // 0x00484340 | nintendogs:callgraph [tier A]
void GatherStartAndSelect(bit32& hold, bit32& trigger, bit32& release); // 0x0048434C | nintendogs:bytes [tier A]
} // namespace hidlow
} // namespace nn
