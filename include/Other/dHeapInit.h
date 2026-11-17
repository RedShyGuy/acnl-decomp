#pragma once

#include "decomp.h"

namespace nn {
namespace os {
class MemoryBlock;
} // namespace os
} // namespace nn
namespace sead {
class ExpHeap;
class Heap;
class UnitHeap;
} // namespace sead

// The heaps of the game (namespace and the Create*Heap names of CreateAllHeaps after libgarden, the other names are
// ours; the variable names after the heap names). CreateAllHeaps makes the heaps of the whole game at the start: the
// root heap "NoDevice" in a memory block of its own, "DLLHeap" and "NetHeap" in it and all other heaps in the current
// heap of sead; DestroyAllHeaps frees them again. The heaps of the stages (Bg, Strc, VrBox, IslandField, ...) are made
// and freed by the scenes of the stages. The Create/Destroy functions are Thumb code (dHeapInitThumb.cpp); each Create
// function returns its heap, each Destroy function destroys the heap and clears the variable.
namespace HeapInit {
// the memory block of the root heap (name is ours)
extern nn::os::MemoryBlock s_MemoryBlock;

extern sead::ExpHeap* s_pNoDeviceRootHeap;
extern sead::ExpHeap* s_pStageHeap;
extern sead::ExpHeap* s_pVillageSeasonHeap;
extern sead::ExpHeap* s_pVrBoxHeap;
extern sead::ExpHeap* s_pWeatherHeap;
extern sead::ExpHeap* s_pFieldHeap;
extern sead::ExpHeap* s_pIslandFieldHeap;
extern sead::ExpHeap* s_pRealEstateHeap;
extern sead::ExpHeap* s_pBgHeap;
extern sead::ExpHeap* s_pStrcHeap;
extern sead::ExpHeap* s_pMyDesignWorkHeap;
extern sead::ExpHeap* s_pHumanAnimBankHeap;
extern sead::ExpHeap* s_pSeadParticleHeap;
extern sead::UnitHeap* s_pParticleHandleHeap;
extern sead::UnitHeap* s_pModelEffectUnitHeap;
extern sead::ExpHeap* s_pModelEffectResHeap;
extern sead::ExpHeap* s_pFaderHeap;
extern sead::ExpHeap* s_pUgcHeap;
extern sead::ExpHeap* s_pScriptHeap;
extern sead::ExpHeap* s_pFontHeap;
extern sead::ExpHeap* s_pPhotoHeap;
// (the save thread lies in it)
extern sead::ExpHeap* s_pSaveHeap;
extern sead::ExpHeap* s_pSoundHeap;
// (the parent of the sound heaps 2..4 of SoundDataLoadMgr)
extern sead::ExpHeap* s_pSoundExtStageHeap;
extern sead::ExpHeap* s_pNetHeap;
extern sead::ExpHeap* s_pNetGameHeap;
// (the data of exhibition.dat)
extern sead::ExpHeap* s_pExhibitionParkHeap;
extern sead::ExpHeap* s_pDLLHeap;
extern sead::ExpHeap* s_pWorkHeap;
// the heap of the page array of SearchList (set by game code that is not decompiled yet; name is ours)
extern sead::Heap* s_pUnknown953CC4;

// all heaps of the game (see above; called once at the start of the game)
void CreateAllHeaps(); // 0x0011D5C0
// all heaps of CreateAllHeaps but the root heap and "DLLHeap" (DestroyDLLHeap follows)
void DestroyAllHeaps(); // 0x0012C00C
// the heap "SaveAllocator" of size in parent and the allocator on it (g_SvSaveMemory.pAllocator) / the heap is
// destroyed (the allocator lies in it)
bool CreateSaveAllocator(u32 size, sead::Heap* parent); // 0x0011EEC8
void DestroySaveAllocator(); // 0x00133C14

// the heaps (Thumb)
sead::ExpHeap* CreateNoDeviceRootHeap(nn::os::MemoryBlock* block); // 0x00100400
sead::ExpHeap* CreateDLLHeap(sead::Heap* parent); // 0x00100048
sead::ExpHeap* CreateNetHeap(sead::Heap* parent); // 0x00100088
sead::ExpHeap* CreateUgcHeap(sead::Heap* parent); // 0x001000C4
sead::ExpHeap* CreateFontHeap(sead::Heap* parent); // 0x00100104
sead::ExpHeap* CreateSaveHeap(sead::Heap* parent); // 0x00100148
sead::ExpHeap* CreateWorkHeap(sead::Heap* parent); // 0x0010018C
sead::ExpHeap* CreateFaderHeap(sead::Heap* parent); // 0x001001D0
sead::ExpHeap* CreateFieldHeap(sead::Heap* parent); // 0x00100214
sead::ExpHeap* CreatePhotoHeap(sead::Heap* parent); // 0x00100260
sead::ExpHeap* CreateSoundHeap(sead::Heap* parent); // 0x001002A4
sead::ExpHeap* CreateStageHeap(sead::Heap* parent); // 0x001002E8
sead::ExpHeap* CreateScriptHeap(sead::Heap* parent); // 0x0010032C
sead::ExpHeap* CreateNetGameHeap(sead::Heap* parent); // 0x00100378
sead::ExpHeap* CreateWeatherHeap(sead::Heap* parent); // 0x001003BC
sead::ExpHeap* CreateRealEstateHeap(sead::Heap* parent); // 0x00100448
sead::UnitHeap* CreateModelEffectUnitHeap(sead::Heap* parent); // 0x00100490
sead::ExpHeap* CreateSeadParticleHeap(sead::Heap* parent); // 0x001004DC
sead::ExpHeap* CreateHumanAnimBankHeap(sead::Heap* parent); // 0x00100528
sead::ExpHeap* CreateModelEffectResHeap(sead::Heap* parent); // 0x00100574
sead::UnitHeap* CreateParticleHandleHeap(sead::Heap* parent); // 0x001005C0
sead::ExpHeap* CreateBgHeap(sead::Heap* parent, u32 size); // 0x0010C2E4
sead::ExpHeap* CreateStrcHeap(sead::Heap* parent, u32 size); // 0x0010C338
sead::ExpHeap* CreateVrBoxHeap(sead::Heap* parent); // 0x0010C380
sead::ExpHeap* CreateMyDesignWorkHeap(sead::Heap* parent, u32 size); // 0x0010C454
// (a size of 0: 0x140000)
sead::ExpHeap* CreateSoundExtStageHeap(sead::Heap* parent, u32 size); // 0x0010C4B4
sead::ExpHeap* CreateVillageSeasonHeap(sead::Heap* parent); // 0x0010C510
sead::ExpHeap* CreateExhibitionParkHeap(sead::Heap* parent); // 0x0010C584

void DestroyDLLHeap(); // 0x001007C0 (also without a heap)
void DestroyNetHeap(); // 0x001008E8
void DestroyUgcHeap(); // 0x00100900
void DestroyFontHeap(); // 0x00100918
void DestroySaveHeap(); // 0x00100930
void DestroyWorkHeap(); // 0x00100948
void DestroyFaderHeap(); // 0x00100960
void DestroyFieldHeap(); // 0x00100978
void DestroyPhotoHeap(); // 0x00100998
void DestroySoundHeap(); // 0x001009B0
// (the heaps of the script first)
void DestroyScriptHeap(); // 0x001009C8
void DestroyWeatherHeap(); // 0x001009E4
void DestroyRealEstateHeap(); // 0x001009FC
void DestroyModelEffectUnitHeap(); // 0x00100A14
void DestroySeadParticleHeap(); // 0x00100A2C
void DestroyHumanAnimBankHeap(); // 0x00100A44
void DestroyModelEffectResHeap(); // 0x00100A5C
void DestroyParticleHandleHeap(); // 0x00100A74
void DestroyBgHeap(); // 0x0010C320
void DestroyStrcHeap(); // 0x0010C3CC
void DestroyVrBoxHeap(); // 0x0010C3EC
void DestroyIslandFieldHeap(); // 0x0010C49C
void DestroyMyDesignWorkHeap(); // 0x0010C564
void DestroySoundExtStageHeap(); // 0x0010C5D8
void DestroyVillageSeasonHeap(); // 0x0010C5F8
void DestroyExhibitionParkHeap(); // 0x0010C610 (also without a heap)

// the sizes of some heaps (inline; ARMCC made ARM copies for the Thumb code; names are ours)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline u32 GetUgcHeapSize() // 0x00120758
{
    return 0x11400;
}
// (the Japanese fonts need more memory)
u32 GetFontHeapSize(); // 0x00120760
DECOMP_EMIT_COPY DECOMP_NOINLINE inline u32 GetPhotoHeapSize() // 0x00120E24
{
    return 0x180000;
}
} // namespace HeapInit

// (game code that is not decompiled yet; name after the address) DestroyAllHeaps frees "NetHeap" only if unk_0x3 is set
struct UnknownAC2930
{
    u8 unk_0x0[3];
    u8 unk_0x3;
    u8 unk_0x4;
    u8 _pad[3];
    s64 unk_0x8;
    s64 unk_0x10;
};
extern UnknownAC2930 g_UnknownAC2930;
