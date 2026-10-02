#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class StringConverter
{
public:
    StringConverter(); // TODO: default ctor added so derived stubs compile - may not exist
    void Decode(wchar_t*, unsigned short, bool); // 0x0037DC64 | fefates:bytes [tier B]
    void Encode(const wchar_t*, bool); // 0x0037DC94 | mk7dlp:callgraph [tier A]
    StringConverter(int); // 0x0037DD58 | mk7dlp:bytes [tier A]
    ~StringConverter(); // 0x0037DD90 | mk7dlp:callgraph [tier A]
};
} // namespace nex
} // namespace nn
