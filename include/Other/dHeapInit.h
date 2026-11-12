#pragma once

#include "decomp.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead

// the heaps of the game made at the start (HeapInit::Create*Heap; the variable names after the heap names; only the
// ones decompiled code uses)
namespace HeapInit {
// "ModelEffectResHeap" (set by HeapInit::CreateModelEffectResHeap)
extern sead::ExpHeap* s_pModelEffectResHeap;
// "StageHeap" (set by HeapInit::CreateStageHeap)
extern sead::ExpHeap* s_pStageHeap;
// "FontHeap" (set by HeapInit::CreateFontHeap)
extern sead::ExpHeap* s_pFontHeap;
// "PhotoHeap" (set by HeapInit::CreatePhotoHeap)
extern sead::ExpHeap* s_pPhotoHeap;
// "SaveHeap" (set by HeapInit::CreateSaveHeap; the save thread lies in it)
extern sead::ExpHeap* s_pSaveHeap;
// the parent of the sound heaps 2..4 of SoundDataLoadMgr (set by game code that is not decompiled yet; name is ours)
extern sead::Heap* s_pSoundDataHeap;
// the heap of the page array of SearchList (set by game code that is not decompiled yet; name is ours)
extern sead::Heap* s_pUnknown953CC4;
// the heap of the data of exhibition.dat (else the stage heap; set by game code that is not decompiled yet; name is ours)
extern sead::Heap* s_pUnknown953CB4;
} // namespace HeapInit
