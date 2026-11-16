#include "fgobj/dTramplePacket.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "Other/dTourData.h"
#include "stage/stage_Extern.h"

namespace fgobj {
// 0x00594EE0 (name after libgarden)
void fgobj::TramplePacket::Process()
{
    Field* field;
    if (stage == stage::NAME_CURRENT) {
        field = FdInfo::GetCurrentField();
    } else {
        field = stage::GetField(GetFieldName(stage));
    }
    if (field == NULL) {
        return;
    }
    const SvFgName* fg = field->GetFg(x, z, 0);
    if (fg == NULL) {
        return;
    }
    if (!Func2FE704(fg, &replace)) {
        return;
    }
    if (netgame::IsHost() && GetUnitLockHandle(x, z, stage) != -1) {
        return;
    }
    Func168E20(x, z, 0, stage, SvFgName::s_Empty);
    if (stage::GetCurrent() != stage) {
        return;
    }
    if (!Func6A7480()) {
        return;
    }
    RefreshUnit(x, z, true);
    Func1703C0(&replace, x, z, true);
}

namespace {
// the chance (in percent) that a flower is trampled
const f32 TRAMPLE_CHANCE = 12.5f;
} // namespace

// 0x00597F04 (name is ours)
void Trample(s32 x, s32 z, bool changedUnit, bool isUnknown1, bool isUnknown2)
{
    if (!stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) || !(changedUnit & isUnknown2)) {
        return;
    }
    const SvFgName* fg = FdInfo::GetCurrentField()->GetFg(x, z, 0);
    if (fg == NULL) {
        return;
    }
    const u8 fgKind = fg->GetFgKind();
    if (fgKind == SvFgName::FG_KIND_INVALID) {
        return;
    }
    SwayFieldObject(x, z);
    const bool isTourKind1 = TourData::s_pInstance->Func76F6E8() == 1;
    if (!SvFgName::IsFlowerFgKind(fgKind) && fgKind != SvFgName::FG_KIND_WILTED_FLOWER) {
        return;
    }
    if (!(changedUnit && isUnknown1) || isTourKind1) {
        return;
    }
    bool isTrampled = false;
    if (GetRandomF(100.0f) <= TRAMPLE_CHANCE) {
        isTrampled = true;
    }
    Func1703C0(fg, x, z, isTrampled);
    if (!isTrampled) {
        return;
    }
    if (netgame::IsMultiplePlayer()) {
        TramplePacket packet;
        const stage::Name stage = stage::GetCurrent();
        packet.replace = *fg;
        packet.stage = stage;
        packet.x = x;
        packet.z = z;
        packet.Send();
    }
    Func168E20(x, z, 0, stage::GetCurrent(), SvFgName::s_Empty);
    RefreshUnit(x, z, true);
}
} // namespace fgobj
