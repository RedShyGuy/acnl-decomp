#include "nn/nex/nex_MemoryManager.h"
#include <cstdlib>

namespace nn {
namespace nex {
namespace {
// the function in the header of a block that frees it (name is ours)
typedef void (*FreeFunction)(void* pHeader);
} // namespace

// 0x00361F78 (name is ours)
void nn::nex::MemoryManager::Free(void* p)
{
    if (p == nullptr) {
        return;
    }
    void** pHeader = reinterpret_cast<void**>(static_cast<u8*>(p) - 8);
    FreeFunction function = reinterpret_cast<FreeFunction>(pHeader[0]);
    if (function != nullptr) {
        function(pHeader);
    } else {
        free(pHeader);
    }
}

// 0x00361FA0 | mk7dlp:callgraph [tier A]
void* nn::nex::MemoryManager::Allocate(u32)
{
}

} // namespace nex
} // namespace nn
