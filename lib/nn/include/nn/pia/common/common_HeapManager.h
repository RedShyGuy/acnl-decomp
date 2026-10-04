#pragma once

#include "decomp.h"
#include "nn/pia/pia_Types.h"
#include "pead/peadExpHeap.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadSafeStringBase.h"

namespace nn {
namespace pia {
namespace common {
// The heaps of the pia modules: one ExpHeap per module in the memory the application passes to
// common::Initialize. RootObject::operator new allocates on the heap of the module that is set
// up at the moment (SetCurrentHeap in BeginSetup). Member names are ours.
class HeapManager
{
public:
    // number of heaps; HEAP_INDEX_NONE (= HEAP_NUM) is the current one outside of a setup
    static const int HEAP_NUM = 13;
    static const u8 HEAP_INDEX_NONE = 13;

    // false if already initialized
    static bool Initialize(void* pMemory, unsigned int size); // 0x00426960 | fefates:bytes-fuzzy [tier B]
    static void Finalize(); // 0x00426B98 | fefates:bytes [tier B]
    // creates the heap of the module
    static void Setup(nn::pia::ModuleType module, unsigned int size, const pead::SafeStringBase<char>& name); // 0x00426A10 | fefates:bytes [tier B]
    static void Cleanup(nn::pia::ModuleType module); // 0x00426A64 | fefates:bytes [tier B]
    static void SetCurrentHeap(nn::pia::ModuleType module); // 0x004269E4 | fefates:bytes [tier B]
    static void ClearCurrentHeap(); // 0x004269FC | fefates:callgraph [tier C]
    static pead::ExpHeap* GetHeap(); // 0x00426AC8 | fefates:bytes [tier B]
    static pead::ExpHeap* GetHeap(nn::pia::ModuleType module); // 0x00426AB0 | fefates:callgraph [tier C]

private:
    // index of the heap of a module
    DECOMP_NOINLINE static u8 convert(nn::pia::ModuleType module); // 0x00426AE4 | fefates:callgraph [tier C]

    static bool s_IsInitialized; // 0x0097E3C8
    static u8 s_CurrentHeapIndex; // 0x0097E3C9
    static pead::ExpHeap* s_pHeaps[HEAP_NUM]; // 0x00AE6C00
    static pead::Arena s_Arena; // 0x00AE6C34
};
} // namespace common
} // namespace pia
} // namespace nn
