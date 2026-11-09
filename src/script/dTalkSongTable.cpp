#include "script/dTalkSystem.h"

namespace script {
// (in its own file: GCC would fold the table into the call otherwise)

// 0x00892964
const TalkSong::StateFunc TalkSong::s_StateFuncs[STATE_NUM] = {
    &TalkSong::UpdateIdle, &TalkSong::UpdateSing, &TalkSong::UpdateNext, &TalkSong::UpdateRepeat, &TalkSong::UpdateEnd,
};
} // namespace script
