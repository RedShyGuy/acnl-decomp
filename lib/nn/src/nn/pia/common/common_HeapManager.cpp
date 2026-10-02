#include "nn/pia/common/common_HeapManager.h"

namespace nn {
namespace pia {
namespace common {
// 0x00426960 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::HeapManager::Initialize(void*, unsigned int)
{
}

// 0x004269E4 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::SetCurrentHeap(nn::pia::ModuleType)
{
}

// 0x00426A10 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Setup(nn::pia::ModuleType, unsigned int, const pead::SafeStringBase<char>&)
{
}

// 0x00426A64 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Cleanup(nn::pia::ModuleType)
{
}

// 0x00426AC8 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::GetHeap()
{
}

// 0x00426B98 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Finalize()
{
}

} // namespace common
} // namespace pia
} // namespace nn
