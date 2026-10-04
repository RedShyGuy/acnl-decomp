#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_HeapManager.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace common {
// 0x004266C4 | fefates:callgraph [tier C]
void* nn::pia::common::RootObject::operator new(size_t size)
{
    return pead::AllocMemory(size, HeapManager::GetHeap());
}

// 0x0053B2DC | fefates:callgraph [tier C]
void* nn::pia::common::RootObject::operator new[](size_t size)
{
    return pead::AllocMemory(size, HeapManager::GetHeap());
}

// 0x004266B4
void nn::pia::common::RootObject::operator delete(void* p)
{
    if (p != nullptr) {
        pead::FreeMemory(p);
    }
}

// 0x00426694 | fefates:callgraph [tier C]
void nn::pia::common::RootObject::operator delete[](void* p)
{
    if (p != nullptr) {
        pead::FreeMemory(p);
    }
}

} // namespace common
} // namespace pia
} // namespace nn
