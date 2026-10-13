#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// 0x0096C94C
MemoryManager::AllocateFunction MemoryManager::s_AllocateFunction;
// 0x0096C950
MemoryManager::FreeFunction MemoryManager::s_FreeFunction;

// 0x00361F78 (name is ours)
void nn::nex::MemoryManager::Free(void* p)
{
    FreeInline(p);
}

// 0x00361FA0 | mk7dlp:callgraph [tier A]
void* nn::nex::MemoryManager::Allocate(u32 size)
{
    size = ((size + 3) & ~3) + HEADER_SIZE;
    void** pHeader;
    if (s_AllocateFunction != nullptr) {
        pHeader = static_cast<void**>(s_AllocateFunction(size));
    } else {
        pHeader = static_cast<void**>(malloc(size));
    }
    pHeader[0] = reinterpret_cast<void*>(s_FreeFunction);
    return reinterpret_cast<u8*>(pHeader) + HEADER_SIZE;
}

} // namespace nex
} // namespace nn
