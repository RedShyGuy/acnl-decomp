#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
void IsValidBinaryFile(const nw::ut::BinaryFileHeader*, unsigned, unsigned, unsigned short); // 0x0048B900 | nintendogs:bytes [tier A]
void GetNextBinaryBlockHeader(nw::ut::BinaryFileHeader*, nw::ut::BinaryBlockHeader*); // 0x0048B980 | nintendogs:bytes [tier A]
} // namespace ut
} // namespace nw
