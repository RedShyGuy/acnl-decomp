#include "nn/pia/common/common_HeapManager.h"
#include "pead/peadThreadMgr.h"

namespace nn {
namespace pia {
namespace common {
// 0x0097E3C8
bool HeapManager::s_IsInitialized = false;
// 0x0097E3C9
u8 HeapManager::s_CurrentHeapIndex = HEAP_INDEX_NONE;
// 0x00AE6C00
pead::ExpHeap* HeapManager::s_pHeaps[HEAP_NUM];
// 0x00AE6C34
pead::Arena HeapManager::s_Arena;

// 0x00426960 | fefates:bytes-fuzzy [tier B]
bool nn::pia::common::HeapManager::Initialize(void* pMemory, unsigned int size)
{
    if (s_IsInitialized) {
        return false;
    }
    s_Arena.mStart = pMemory;
    s_Arena.mSize = size;
    s_Arena.mInitialized = true;
    pead::HeapMgr::initialize(&s_Arena);
    pead::Heap* rootHeap = pead::HeapMgr::getRootHeap(0);
    pead::ThreadMgr::createInstance(rootHeap);
    pead::ThreadMgr::instance()->initialize(rootHeap);
    s_CurrentHeapIndex = HEAP_INDEX_NONE;
    s_IsInitialized = true;
    return true;
}

// 0x004269E4 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::SetCurrentHeap(nn::pia::ModuleType module)
{
    s_CurrentHeapIndex = convert(module);
}

// 0x004269FC | fefates:callgraph [tier C]
void nn::pia::common::HeapManager::ClearCurrentHeap()
{
    s_CurrentHeapIndex = HEAP_INDEX_NONE;
}

// 0x00426A10 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Setup(nn::pia::ModuleType module, unsigned int size, const pead::SafeStringBase<char>& name)
{
    u8 index = convert(module);
    s_pHeaps[index] = pead::ExpHeap::tryCreate(size, name, pead::HeapMgr::getRootHeap(0),
                                               pead::Heap::HEAP_DIRECTION_FORWARD, false);
}

// 0x00426A64 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Cleanup(nn::pia::ModuleType module)
{
    u8 index = convert(module);
    // blocks that were not freed
    if (s_pHeaps[index]->getUseCount() != 0) {
        s_pHeaps[index]->dump();
    }
    s_pHeaps[index]->destroy();
    s_pHeaps[index] = nullptr;
}

// 0x00426AB0 | fefates:callgraph [tier C]
pead::ExpHeap* nn::pia::common::HeapManager::GetHeap(nn::pia::ModuleType module)
{
    return s_pHeaps[convert(module)];
}

// 0x00426AC8 | fefates:bytes [tier B]
pead::ExpHeap* nn::pia::common::HeapManager::GetHeap()
{
    return s_pHeaps[s_CurrentHeapIndex];
}

// 0x00426AE4 | fefates:callgraph [tier C]
u8 nn::pia::common::HeapManager::convert(nn::pia::ModuleType module)
{
    switch (module) {
    case MODULE_TYPE_COMMON:
        return 0;
    case MODULE_TYPE_LOCAL:
        return 1;
    case MODULE_TYPE_TRANSPORT:
        return 3;
    case 5:
        return 5;
    case MODULE_TYPE_INET:
        return 2;
    case MODULE_TYPE_SESSION:
        return 4;
    case 8:
        return 6;
    case 9:
        return 7;
    case 10:
        return 8;
    case 11:
        return 9;
    case 12:
        return 10;
    case 13:
        return 11;
    case 14:
        return 12;
    default:
        return HEAP_INDEX_NONE;
    }
}

// 0x00426B98 | fefates:bytes [tier B]
void nn::pia::common::HeapManager::Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    s_IsInitialized = false;
    pead::ThreadMgr::instance()->destroy();
    pead::ThreadMgr::deleteInstance();
    pead::HeapMgr::destroy();
}

} // namespace common
} // namespace pia
} // namespace nn
