#include "event/dEventNpcBirthday.h"
#include "Sv/dSvApi.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"

namespace event {
// 0x0058F474 (name is ours)
event::EventNpcBirthday::EventNpcBirthday()
    : EventBase("NPCの誕生日", "日付は変えません", Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE},
                Info{0xFFFF, 2, EVENT_ID_NPC_BIRTHDAY, 2}, Range{Time(6, 0), Time(0, 0), 1}, Schedule())
{
}

// 0x00754340 slot 0x00 (name is ours)
bool event::EventNpcBirthday::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    if (!Func119504(GetGardenPlus()->data.npcData.npcs)) {
        return false;
    }
    return IsBeforeEnd(date);
}

// 0x007543BC slot 0x04 (name is ours)
bool event::EventNpcBirthday::IsOnDate(s32 /* year */, s32 month, s32 day) const
{
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    for (s32 i = Func118C8C(npcs) - 1; i >= 0; i--) {
        u8* npc = static_cast<u8*>(Func300928(npcs, i));
        if (npc != NULL) {
            u16 id = *reinterpret_cast<u16*>(npc + 44);
            const u8* info = g_UnknownA84AEC.table != NULL && id < 399 ? g_UnknownA84AEC.table + id * 34 : NULL;
            if (Func112FE8(info) == month && Func112788(info) == day) {
                return true;
            }
        }
    }
    return false;
}
} // namespace event
