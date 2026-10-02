#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class HeapManager
{
public:
    void Initialize(void*, unsigned int); // 0x00426960 | fefates:bytes-fuzzy [tier B]
    void SetCurrentHeap(nn::pia::ModuleType); // 0x004269E4 | fefates:bytes [tier B]
    void Setup(nn::pia::ModuleType, unsigned int, const pead::SafeStringBase<char>&); // 0x00426A10 | fefates:bytes [tier B]
    void Cleanup(nn::pia::ModuleType); // 0x00426A64 | fefates:bytes [tier B]
    void GetHeap(); // 0x00426AC8 | fefates:bytes [tier B]
    void Finalize(); // 0x00426B98 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
