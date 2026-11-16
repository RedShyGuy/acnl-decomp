#include "fgobj/dWaterPacket.h"
#include "Ac/dAcPlayer.h"
#include "demo/demo_Extern.h"
#include "fgobj/fgobj_Extern.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "script/dScriptExtern.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPlayer.h"

namespace fgobj {
namespace {
// the number of units of an area
inline s32 GetWaterUnitNum(s32 area)
{
    s32 num = 1;
    if (area == WATER_AREA_SQUARE) {
        num = 9;
    } else if (area == WATER_AREA_CROSS) {
        num = 5;
    }
    return num;
}

// the height difference from which a unit is not watered
const f32 WATER_HEIGHT_LIMIT = 8.0f;
// the initiative of a newly watered flower (meaning of the value unknown)
const s32 INITIATIVE_WATER = 6;
} // namespace

// 0x00595884 (name is ours)
void fgobj::WaterPacket::Process() const
{
    WaterFlowers(x, z, stage, area, skipMask, false);
}

// 0x005970A8 (name is ours)
u16 GetHeightDifferenceMask(s32 x, s32 z, stage::Name stage, s32 area, f32 height)
{
    const FieldName field = GetFieldName(stage);
    const s32 num = GetWaterUnitNum(area);
    u16 mask = 0;
    for (s32 i = 0; i < num; i++) {
        const f32 difference = FdInfo::GetUnitHeight(x + s_HeightCheckUnitOffsets[0][i], z + s_HeightCheckUnitOffsets[1][i], field) - height;
        if (difference >= WATER_HEIGHT_LIMIT || difference <= -WATER_HEIGHT_LIMIT) {
            mask |= 1 << i;
        }
    }
    return mask;
}

// 0x00597CCC (name is ours)
void WaterFlowers(s32 x, s32 z, stage::Name stage, s32 area, u16 skipMask, bool isMine)
{
    const s32 num = GetWaterUnitNum(area);
    for (s32 i = 0; i < num; i++) {
        if (skipMask & (1 << i)) {
            continue;
        }
        const s32 unitX = x + s_WaterUnitOffsets[0][i];
        const s32 unitZ = z + s_WaterUnitOffsets[1][i];
        Field* field;
        if (stage == stage::NAME_CURRENT) {
            field = FdInfo::GetCurrentField();
        } else {
            field = stage::GetField(GetFieldName(stage));
        }
        if (field == NULL) {
            continue;
        }
        const SvFgName* fg = field->GetFg(unitX, unitZ, 0);
        if (fg == NULL) {
            continue;
        }
        SvFgName watered(*fg);
        if (isMine && Func5C3750() && !fg->IsWatered()) {
            if (fg->IsFlower() || fg->GetFgKind() == SvFgName::FG_KIND_WILTED_FLOWER) {
                SvPlayer* player = SvGetCurrentPlayer();
                if (player != NULL) {
                    Func20C714(player);
                }
                Func2A83B0();
            }
        }
        // the golden can makes gold roses of black roses
        if (area == WATER_AREA_SQUARE && fg->IsName(SvFgName::NAME_BLACK_ROSES_WILTED)) {
            watered = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_GOLD_ROSES_WILTED));
        }
        const bool wasWatered = watered.IsWatered();
        watered.SetWatered();
        field->PlaceItem(watered, unitX, unitZ, 0);
        if (isMine && watered.IsFlowerOrWilted() && !wasWatered && watered.IsWatered()) {
            GetCensus()->AddFlowersWatered(1);
            Func24BA04(&SvGetCurrentPlayer()->initiative, INITIATIVE_WATER, 1);
        }
    }
}

// 0x00598F4C (name is ours)
void Water(s32 x, s32 z, s32 area, bool isUnknown)
{
    const stage::Name stage = stage::GetCurrent();
    u16 skipMask;
    AcPlayer* player = demo::PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
    if (player == NULL) {
        skipMask = 0;
    } else {
        skipMask = GetHeightDifferenceMask(x, z, stage, area, static_cast<Actor*>(player)->m_Position.y);
    }
    WaterFlowers(x, z, stage::GetCurrent(), area, skipMask, !isUnknown);
    if (netgame::IsMultiplePlayer()) {
        WaterPacket packet;
        packet.stage = stage::GetCurrent();
        packet.x = x;
        packet.z = z;
        packet.area = area;
        packet.skipMask = skipMask;
        netgame::EnqueuePacket(WaterPacket::PACKET_TYPE, netgame::PLAYER_NO_TARGET_ALL, &packet, sizeof(packet));
    }
}
} // namespace fgobj
