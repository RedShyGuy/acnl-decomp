#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace Util {
void CalcRandom(); // 0x004C9BCC | nintendogs:bytes [tier A]
void CalcLpfFreq(float); // 0x004C9BF8 | nintendogs:bytes [tier A]
void CalcPanRatio(float, const nw::snd::internal::Util::PanInfo&); // 0x004C9C90 | fefates:bytes [tier B]
void CalcPitchRatio(int); // 0x004C9D98 | nintendogs:bytes [tier A]
void CalcVolumeRatio(float); // 0x004C9EC4 | nintendogs:bytes [tier A]
void GetByteBySample(unsigned long, nw::snd::SampleFormat); // 0x004C9F1C | nintendogs:bytes [tier A]
void CalcSurroundPanRatio(float, const nw::snd::internal::Util::PanInfo&); // 0x004C9F8C | nintendogs:bytes [tier A]
} // namespace Util
} // namespace internal
} // namespace snd
} // namespace nw
