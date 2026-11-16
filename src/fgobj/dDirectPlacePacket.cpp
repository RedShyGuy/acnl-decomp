#include "fgobj/dDirectPlacePacket.h"

#include "field/dFdInfo.h"
#include "field/dField.h"
#include "stage/stage_Extern.h"

namespace fgobj {
// 0x00594D88 | libgarden [tier A]
void fgobj::DirectPlacePacket::Place() const
{
    Field* field;
    if (stage == stage::NAME_NONE) {
        field = FdInfo::GetCurrentField();
    } else {
        field = stage::GetField(GetFieldName(stage));
    }
    if (field == NULL) {
        return;
    }
    field->PlaceItem(fg, pos.x, pos.y, 0);
    if (stage::GetCurrent() == stage) {
        ResetFieldBit(pos.x, pos.y);
    }
}

// 0x00594E04 | libgarden [tier A]
void fgobj::DirectPlacePacket::Send() const
{
    netgame::EnqueuePacket(0x83, netgame::PLAYER_NO_TARGET_ALL, this, sizeof(*this));
}

// 0x00594E18 (name after libgarden)
void fgobj::DirectPlacePacket::Set(const SvFgName& fg, u8 x, u8 y, stage::Name stage)
{
    this->fg = fg;
    pos.x = x;
    this->stage = stage;
    pos.y = y;
}

// 0x00594E40 (name is ours)
fgobj::DirectPlacePacket::DirectPlacePacket()
{
    stage = stage::NAME_NONE;
    pos.x = 0;
    pos.y = 0;
}

// 0x00594E60 (name is ours)
fgobj::DirectPlacePacket::~DirectPlacePacket()
{
    // (only the members are destroyed)
}
} // namespace fgobj
