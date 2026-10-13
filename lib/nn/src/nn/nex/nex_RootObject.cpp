#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// 0x00355190 | fefates:callgraph [tier C]
void nn::nex::RootObject::operator delete(void* p)
{
    MemoryManager::FreeInline(p);
}

// 0x003551B4 | fefates:callgraph [tier C]
void* nn::nex::RootObject::operator new(unsigned int size)
{
    return MemoryManager::Allocate(size);
}

// 0x00361F9C | fefates:callgraph [tier C]
void* nn::nex::RootObject::operator new[](unsigned int size)
{
    return MemoryManager::Allocate(size);
}

} // namespace nex
} // namespace nn
