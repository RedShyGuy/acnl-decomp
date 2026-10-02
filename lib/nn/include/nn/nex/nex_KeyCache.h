#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class KeyCache
{
public:
    void RetrieveKey(unsigned int, const char*, nn::nex::Key*); // 0x003D5194 | fefates:bytes [tier B]
    void AddKey(unsigned, const char*, const nn::nex::Key&); // 0x003D5338 | mk7dlp:callseq [tier A]
    void AddKey(unsigned, const nn::nex::Buffer&, const nn::nex::Key&); // 0x003D5398 | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
