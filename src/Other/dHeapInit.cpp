#include "Other/dHeapInit.h"

#include "Other/dHeapUtil.h"
#include "Other/dRegion.h"
#include "nn/os/os_MemoryBlock.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "Sv/dSvApi.h"

namespace HeapInit {
// 0x00953C48
sead::ExpHeap* s_pNoDeviceRootHeap = NULL;
// 0x00953C4C
sead::ExpHeap* s_pStageHeap = NULL;
// 0x00953C50
sead::ExpHeap* s_pVillageSeasonHeap = NULL;
// 0x00953C54
sead::ExpHeap* s_pVrBoxHeap = NULL;
// 0x00953C58
sead::ExpHeap* s_pWeatherHeap = NULL;
// 0x00953C5C
sead::ExpHeap* s_pFieldHeap = NULL;
// 0x00953C60
sead::ExpHeap* s_pIslandFieldHeap = NULL;
// 0x00953C64
sead::ExpHeap* s_pRealEstateHeap = NULL;
// 0x00953C68
sead::ExpHeap* s_pBgHeap = NULL;
// 0x00953C6C
sead::ExpHeap* s_pStrcHeap = NULL;
// 0x00953C70
sead::ExpHeap* s_pMyDesignWorkHeap = NULL;
// 0x00953C74
sead::ExpHeap* s_pHumanAnimBankHeap = NULL;
// 0x00953C78
sead::ExpHeap* s_pSeadParticleHeap = NULL;
// 0x00953C7C
sead::UnitHeap* s_pParticleHandleHeap = NULL;
// 0x00953C80
sead::UnitHeap* s_pModelEffectUnitHeap = NULL;
// 0x00953C84
sead::ExpHeap* s_pModelEffectResHeap = NULL;
// 0x00953C8C
sead::ExpHeap* s_pFaderHeap = NULL;
// 0x00953C90
sead::ExpHeap* s_pUgcHeap = NULL;
// 0x00953C94
sead::ExpHeap* s_pScriptHeap = NULL;
// 0x00953C98
sead::ExpHeap* s_pFontHeap = NULL;
// 0x00953C9C
sead::ExpHeap* s_pPhotoHeap = NULL;
// 0x00953CA0
sead::ExpHeap* s_pSaveHeap = NULL;
// 0x00953CA4
sead::ExpHeap* s_pSoundHeap = NULL;
// 0x00953CA8
sead::ExpHeap* s_pSoundExtStageHeap = NULL;
// 0x00953CAC
sead::ExpHeap* s_pNetHeap = NULL;
// 0x00953CB0
sead::ExpHeap* s_pNetGameHeap = NULL;
// 0x00953CB4
sead::ExpHeap* s_pExhibitionParkHeap = NULL;
// 0x00953CB8
sead::ExpHeap* s_pDLLHeap = NULL;
// 0x00953CBC
sead::ExpHeap* s_pWorkHeap = NULL;
// 0x00953CC4
sead::Heap* s_pUnknown953CC4 = NULL;

// 0x00A1967C
nn::os::MemoryBlock s_MemoryBlock;
// the memory block of the root heap is constructed and its destructor registered
// 0x007A0ACC (static initializer)

namespace {
// the size of the memory block of the root heap
const u32 ROOT_MEMORY_SIZE = 0x3CD800;
// the size of the heap "SaveAllocator"
const u32 SAVE_ALLOCATOR_SIZE = 0xBB800;
} // namespace

// 0x0011D5C0
void CreateAllHeaps()
{
    s_MemoryBlock.AllocateBlock(ROOT_MEMORY_SIZE);
    sead::ExpHeap* root = CreateNoDeviceRootHeap(&s_MemoryBlock);
    sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
    CreateDLLHeap(root);
    CreateWorkHeap(heap);
    CreateStageHeap(heap);
    CreateFieldHeap(heap);
    CreateRealEstateHeap(heap);
    CreateHumanAnimBankHeap(heap);
    CreateWeatherHeap(heap);
    CreateSeadParticleHeap(heap);
    CreateParticleHandleHeap(s_pSeadParticleHeap);
    // (armlink replaced a call with NULL here by its result 0)
    CreateModelEffectUnitHeap(heap);
    CreateModelEffectResHeap(heap);
    CreateFaderHeap(heap);
    CreateUgcHeap(heap);
    CreateScriptHeap(heap);
    CreateFontHeap(heap);
    // (armlink replaced a call with heap here by its result 0)
    CreateSoundHeap(heap);
    CreateSaveHeap(heap);
    CreateSaveAllocator(SAVE_ALLOCATOR_SIZE, heap);
    CreateNetHeap(root);
    CreateNetGameHeap(heap);
    CreatePhotoHeap(heap);
}

// 0x0012C00C (name is ours)
void DestroyAllHeaps()
{
    DestroyWorkHeap();
    DestroyFieldHeap();
    DestroyRealEstateHeap();
    DestroyHumanAnimBankHeap();
    DestroyWeatherHeap();
    DestroyParticleHandleHeap();
    DestroySeadParticleHeap();
    // (armlink removed a call here)
    DestroyModelEffectUnitHeap();
    DestroyModelEffectResHeap();
    DestroyFaderHeap();
    DestroyUgcHeap();
    DestroyScriptHeap();
    DestroyFontHeap();
    // (armlink removed a call here)
    DestroySoundHeap();
    DestroySaveHeap();
    DestroySaveAllocator();
    if (g_UnknownAC2930.unk_0x3) {
        DestroyNetHeap();
    }
    DestroyPhotoHeap();
}

// 0x00133C14 (name is ours)
void DestroySaveAllocator()
{
    ssys::ma::Allocator* allocator = g_SvSaveMemory.pAllocator;
    if (allocator == NULL) {
        return;
    }
    sead::Heap* heap = allocator->m_pHeap;
    if (heap != NULL) {
        allocator->SetHeap(NULL);
        heap->destroy();
    }
    g_SvSaveMemory.pAllocator = NULL;
}

// 0x0011EEC8
bool CreateSaveAllocator(u32 size, sead::Heap* parent)
{
    sead::ExpHeap* heap = HeapUtil::CreateExpHeapWithCName(size, parent, "SaveAllocator", 128);
    g_SvSaveMemory.pAllocator = new (heap, 4) ssys::ma::Allocator();
    return g_SvSaveMemory.pAllocator->SetHeap(heap);
}

// 0x00120760 (name is ours)
u32 GetFontHeapSize()
{
    return GetRegionIndex() == 0 ? 0x145000 : 0xF0000;
}
} // namespace HeapInit

// (a variable of another file of the game)
// 0x00AC2930
UnknownAC2930 g_UnknownAC2930;
