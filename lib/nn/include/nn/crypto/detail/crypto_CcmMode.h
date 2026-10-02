#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
namespace detail {
class CcmMode
{
public:
    void Initialize(const nn::crypto::BlockCipher&, const void*, unsigned int, unsigned int, unsigned int, unsigned int); // 0x0048389C | fefates:bytes [tier B]
    void GenerateMac(void*, unsigned int); // 0x004839E0 | fefates:bytes [tier B]
    void UpdateAdata(const void*, unsigned int); // 0x00483A4C | fefates:bytes [tier B]
    void UpdateCdata(void*, unsigned int, const void*, unsigned int); // 0x00483B0C | fefates:bytes [tier B]
    void UpdatePdata(void*, unsigned int, const void*, unsigned int); // 0x00483C5C | fefates:bytes [tier B]
    void UpdateAdataFinal(); // 0x00483DB0 | fefates:bytes [tier B]
    void UpdateCdataFinal(void*, unsigned int); // 0x00483E00 | fefates:bytes [tier B]
    void UpdatePdataFinal(void*, unsigned int); // 0x00483EA0 | fefates:bytes [tier B]
    void Finalize(); // 0x00483F44 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace crypto
} // namespace nn
