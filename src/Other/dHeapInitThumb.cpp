// The Thumb part of HeapInit: the functions that make the heaps of the game (names after libgarden). Each heap gets the
// name of its variable; ARMCC calls ARM copies of the SafeString constructor (0x002F6D78), of ExpHeap::create, of the
// SafeString destructor and of PtrArray::pushBack (0x00122BB0) from here.

#include "Other/dHeapInit.h"

#include "nn/os/os_MemoryBlock.h"
#include "Sc/dScStage.h"
#include "script/dMgr.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadUnitHeap.h"

namespace HeapInit {
namespace {
// an ExpHeap of parent at its front (inline)
DECOMP_ALWAYS_INLINE sead::ExpHeap* CreateExpHeap(u32 size, const char* name, sead::Heap* parent, bool enableLock)
{
    return sead::ExpHeap::create(size, name, parent, sead::Heap::HEAP_DIRECTION_FORWARD, enableLock);
}

// the heap is destroyed and the variable cleared (inline)
template <typename T>
DECOMP_ALWAYS_INLINE void DestroyHeap(T*& heap)
{
    heap->destroy();
    heap = NULL;
}

// the same for a variable that may be NULL (inline)
template <typename T>
DECOMP_ALWAYS_INLINE void DestroyHeapIfAny(T*& heap)
{
    if (heap != NULL) {
        heap->destroy();
    }
    heap = NULL;
}
} // namespace

// 0x00100400
sead::ExpHeap* CreateNoDeviceRootHeap(nn::os::MemoryBlock* block)
{
    const sead::SafeStringBase<char> name("NoDevice");
    s_pNoDeviceRootHeap =
        sead::ExpHeap::CreateRoot(reinterpret_cast<void*>(block->GetAddress()), block->GetSize(), &name, false);
    sead::HeapMgr::s_RootHeaps.pushBack(s_pNoDeviceRootHeap);
    return s_pNoDeviceRootHeap;
}

// 0x00100048
sead::ExpHeap* CreateDLLHeap(sead::Heap* parent)
{
    s_pDLLHeap = CreateExpHeap(0x1B8000, "DLLHeap", parent, false);
    return s_pDLLHeap;
}

// 0x00100088
sead::ExpHeap* CreateNetHeap(sead::Heap* parent)
{
    s_pNetHeap = CreateExpHeap(0x215800, "NetHeap", parent, true);
    return s_pNetHeap;
}

// 0x001000C4
sead::ExpHeap* CreateUgcHeap(sead::Heap* parent)
{
    s_pUgcHeap = CreateExpHeap(GetUgcHeapSize(), "UgcHeap", parent, false);
    return s_pUgcHeap;
}

// 0x00100104
sead::ExpHeap* CreateFontHeap(sead::Heap* parent)
{
    s_pFontHeap = CreateExpHeap(GetFontHeapSize(), "FontHeap", parent, false);
    return s_pFontHeap;
}

// 0x00100148
sead::ExpHeap* CreateSaveHeap(sead::Heap* parent)
{
    s_pSaveHeap = CreateExpHeap(0x8000, "SaveHeap", parent, false);
    return s_pSaveHeap;
}

// 0x0010018C
sead::ExpHeap* CreateWorkHeap(sead::Heap* parent)
{
    s_pWorkHeap = CreateExpHeap(0x19000, "WorkHeap", parent, false);
    return s_pWorkHeap;
}

// 0x001001D0
sead::ExpHeap* CreateFaderHeap(sead::Heap* parent)
{
    s_pFaderHeap = CreateExpHeap(0x400, "FaderHeap", parent, false);
    return s_pFaderHeap;
}

// 0x00100214
sead::ExpHeap* CreateFieldHeap(sead::Heap* parent)
{
    s_pFieldHeap = CreateExpHeap(0x133000, "FieldHeap", parent, false);
    return s_pFieldHeap;
}

// 0x00100260
sead::ExpHeap* CreatePhotoHeap(sead::Heap* parent)
{
    s_pPhotoHeap = CreateExpHeap(GetPhotoHeapSize(), "PhotoHeap", parent, false);
    return s_pPhotoHeap;
}

// 0x001002A4
sead::ExpHeap* CreateSoundHeap(sead::Heap* parent)
{
    s_pSoundHeap = CreateExpHeap(0x580000, "SoundHeap", parent, false);
    return s_pSoundHeap;
}

// 0x001002E8
sead::ExpHeap* CreateStageHeap(sead::Heap* parent)
{
    s_pStageHeap = CreateExpHeap(0xEB8000, "StageHeap", parent, true);
    return s_pStageHeap;
}

// 0x0010032C
sead::ExpHeap* CreateScriptHeap(sead::Heap* parent)
{
    s_pScriptHeap = CreateExpHeap(script::Mgr::GetHeapSize(), "ScriptHeap", parent, false);
    script::Mgr::CreateHeaps(s_pScriptHeap);
    return s_pScriptHeap;
}

// 0x00100378
sead::ExpHeap* CreateNetGameHeap(sead::Heap* parent)
{
    s_pNetGameHeap = CreateExpHeap(0xD4800, "NetGameHeap", parent, true);
    return s_pNetGameHeap;
}

// 0x001003BC
sead::ExpHeap* CreateWeatherHeap(sead::Heap* parent)
{
    s_pWeatherHeap = CreateExpHeap(0x7600, "WeatherHeap", parent, false);
    return s_pWeatherHeap;
}

// 0x00100448
sead::ExpHeap* CreateRealEstateHeap(sead::Heap* parent)
{
    s_pRealEstateHeap = CreateExpHeap(0xA5000, "RealEstateHeap", parent, false);
    return s_pRealEstateHeap;
}

// 0x00100490
sead::UnitHeap* CreateModelEffectUnitHeap(sead::Heap* parent)
{
    s_pModelEffectUnitHeap = sead::UnitHeap::tryCreateWithBlockNum(10000, 9, "ModelEffectUnitHeap", 4, parent, false);
    return s_pModelEffectUnitHeap;
}

// 0x001004DC
sead::ExpHeap* CreateSeadParticleHeap(sead::Heap* parent)
{
    s_pSeadParticleHeap = CreateExpHeap(0x23FC00, "SeadParticleHeap", parent, false);
    return s_pSeadParticleHeap;
}

// 0x00100528
sead::ExpHeap* CreateHumanAnimBankHeap(sead::Heap* parent)
{
    s_pHumanAnimBankHeap = CreateExpHeap(0x238333, "HumanAnimBankHeap", parent, false);
    return s_pHumanAnimBankHeap;
}

// 0x00100574
sead::ExpHeap* CreateModelEffectResHeap(sead::Heap* parent)
{
    s_pModelEffectResHeap = CreateExpHeap(0x3C000, "ModelEffectResHeap", parent, false);
    return s_pModelEffectResHeap;
}

// 0x001005C0
sead::UnitHeap* CreateParticleHandleHeap(sead::Heap* parent)
{
    s_pParticleHandleHeap = sead::UnitHeap::tryCreateWithBlockNum(24, 16, "ParticleHandleHeap", 4, parent, false);
    return s_pParticleHandleHeap;
}
// 0x0010C2E4 (name is ours)
sead::ExpHeap* CreateBgHeap(sead::Heap* parent, u32 size)
{
    s_pBgHeap = CreateExpHeap(size, "BgHeap", parent, false);
    return s_pBgHeap;
}

// 0x0010C338 (name is ours)
sead::ExpHeap* CreateStrcHeap(sead::Heap* parent, u32 size)
{
    s_pStrcHeap = CreateExpHeap(size, "StrcHeap", parent, false);
    return s_pStrcHeap;
}

// 0x0010C380 (name is ours)
sead::ExpHeap* CreateVrBoxHeap(sead::Heap* parent)
{
    s_pVrBoxHeap = CreateExpHeap(0x8C00, "VrBoxHeap", parent, false);
    return s_pVrBoxHeap;
}

// 0x0010C454 (name is ours)
sead::ExpHeap* CreateMyDesignWorkHeap(sead::Heap* parent, u32 size)
{
    s_pMyDesignWorkHeap = CreateExpHeap(size, "MyDesignWorkHeap", parent, true);
    return s_pMyDesignWorkHeap;
}

// 0x0010C4B4 (name is ours)
sead::ExpHeap* CreateSoundExtStageHeap(sead::Heap* parent, u32 size)
{
    if (size == 0) {
        size = 0x140000;
    }
    s_pSoundExtStageHeap = CreateExpHeap(size, "SoundExtStageHeap", parent, false);
    return s_pSoundExtStageHeap;
}

// 0x0010C510 (name is ours)
sead::ExpHeap* CreateVillageSeasonHeap(sead::Heap* parent)
{
    s_pVillageSeasonHeap = CreateExpHeap(0x1D800, "VillageSeasonHeap", parent, false);
    return s_pVillageSeasonHeap;
}

// 0x0010C584 (name is ours)
sead::ExpHeap* CreateExhibitionParkHeap(sead::Heap* parent)
{
    s_pExhibitionParkHeap = CreateExpHeap(0x180000, "ExhibitionParkHeap", parent, false);
    return s_pExhibitionParkHeap;
}

// 0x001007C0 (name is ours)
void DestroyDLLHeap()
{
    DestroyHeapIfAny(s_pDLLHeap);
}

// 0x001008E8 (name is ours)
void DestroyNetHeap()
{
    DestroyHeap(s_pNetHeap);
}

// 0x00100900 (name is ours)
void DestroyUgcHeap()
{
    DestroyHeap(s_pUgcHeap);
}

// 0x00100918 (name is ours)
void DestroyFontHeap()
{
    DestroyHeap(s_pFontHeap);
}

// 0x00100930 (name is ours)
void DestroySaveHeap()
{
    DestroyHeap(s_pSaveHeap);
}

// 0x00100948 (name is ours)
void DestroyWorkHeap()
{
    DestroyHeap(s_pWorkHeap);
}

// 0x00100960 (name is ours)
void DestroyFaderHeap()
{
    DestroyHeap(s_pFaderHeap);
}

// 0x00100978 (name is ours)
void DestroyFieldHeap()
{
    DestroyHeap(s_pFieldHeap);
}

// 0x00100998 (name is ours)
void DestroyPhotoHeap()
{
    DestroyHeap(s_pPhotoHeap);
}

// 0x001009B0 (name is ours)
void DestroySoundHeap()
{
    DestroyHeap(s_pSoundHeap);
}

// 0x001009C8 (name is ours)
void DestroyScriptHeap()
{
    script::Mgr::DestroyHeaps();
    DestroyHeap(s_pScriptHeap);
}

// 0x001009E4 (name is ours)
void DestroyWeatherHeap()
{
    DestroyHeap(s_pWeatherHeap);
}

// 0x001009FC (name is ours)
void DestroyRealEstateHeap()
{
    DestroyHeap(s_pRealEstateHeap);
}

// 0x00100A14 (name is ours)
void DestroyModelEffectUnitHeap()
{
    DestroyHeap(s_pModelEffectUnitHeap);
}

// 0x00100A2C (name is ours)
void DestroySeadParticleHeap()
{
    DestroyHeap(s_pSeadParticleHeap);
}

// 0x00100A44 (name is ours)
void DestroyHumanAnimBankHeap()
{
    DestroyHeap(s_pHumanAnimBankHeap);
}

// 0x00100A5C (name is ours)
void DestroyModelEffectResHeap()
{
    DestroyHeap(s_pModelEffectResHeap);
}

// 0x00100A74 (name is ours)
void DestroyParticleHandleHeap()
{
    DestroyHeap(s_pParticleHandleHeap);
}

// 0x0010C320 (name is ours)
void DestroyBgHeap()
{
    DestroyHeap(s_pBgHeap);
}

// 0x0010C3CC (name is ours)
void DestroyStrcHeap()
{
    DestroyHeap(s_pStrcHeap);
}

// 0x0010C3EC (name is ours)
void DestroyVrBoxHeap()
{
    DestroyHeap(s_pVrBoxHeap);
}

// 0x0010C49C (name is ours)
void DestroyIslandFieldHeap()
{
    DestroyHeap(s_pIslandFieldHeap);
}

// 0x0010C564 (name is ours)
void DestroyMyDesignWorkHeap()
{
    DestroyHeap(s_pMyDesignWorkHeap);
}

// 0x0010C5D8 (name is ours)
void DestroySoundExtStageHeap()
{
    DestroyHeap(s_pSoundExtStageHeap);
}

// 0x0010C5F8 (name is ours)
void DestroyVillageSeasonHeap()
{
    DestroyHeap(s_pVillageSeasonHeap);
}

// 0x0010C610 (name is ours)
void DestroyExhibitionParkHeap()
{
    DestroyHeapIfAny(s_pExhibitionParkHeap);
}
} // namespace HeapInit

// 0x0010C40C | libgarden [tier A]
sead::ExpHeap* ScStage::CreateIslandFieldHeap(sead::Heap* parent)
{
    HeapInit::s_pIslandFieldHeap =
        sead::ExpHeap::create(0x100000, "IslandFieldHeap", parent, sead::Heap::HEAP_DIRECTION_FORWARD, false);
    return HeapInit::s_pIslandFieldHeap;
}
