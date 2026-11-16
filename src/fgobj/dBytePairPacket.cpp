#include "fgobj/dBytePairPacket.h"
#include "fgobj/fgobj_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"

namespace fgobj {
// 0x00595910 (name is ours)
void fgobj::BytePairPacket::Set(u8 a, u8 b)
{
    this->a = a;
    this->b = b;
}

// 0x005958FC (name is ours)
void fgobj::BytePairPacket::Send() const
{
    netgame::EnqueuePacket(PACKET_TYPE, netgame::PLAYER_NO_HOST, this, sizeof(*this));
}

// 0x005958D4 (name is ours)
void fgobj::BytePairPacket::Process() const
{
    Func52B188(&GetGardenData()->townData.unk_0xEE0E, a, b);
}
} // namespace fgobj
