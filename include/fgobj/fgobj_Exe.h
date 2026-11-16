#pragma once

#include "decomp.h"
#include "fgobj/dObjectPlant.h"
#include "fgobj/dObjectScale.h"
#include "fgobj/dProc.h"
#include "fgobj/fgobj_Globals.h"
#include "field/dFdInfo.h"
#include "field/dField.h"

class SvFgName;

// (inline; copies for the other callers) the objects that run are put into the list of the Proc (names are ours)
namespace fgobj {
// a growing object of item at the unit x, z (ObjectScale::Create); false without the Proc, outside the field or without a
// free heap
DECOMP_EMIT_COPY inline bool PlaceScale(const SvFgName& item, s32 x, s32 z, bool keepRequest) // 0x0059B700
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectScale* object = ObjectScale::Create(item, x, z, keepRequest);
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}

// a planted object of item at the unit x, z (ObjectPlant::Create)
DECOMP_EMIT_COPY inline bool PlacePlant(const SvFgName& item, s32 x, s32 z, bool playSound) // 0x0059B684
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return false;
    }
    ObjectPlant* object = ObjectPlant::Create(item, x, z, playSound);
    if (object == NULL) {
        return false;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    return true;
}
} // namespace fgobj
