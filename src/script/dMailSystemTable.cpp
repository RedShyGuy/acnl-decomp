#include "script/dMailSystem.h"

// The table of MailSystem (a file of its own, else GCC folds it into the code).

namespace script {
// 0x008926B4
const MailSystem::StateFunc MailSystem::s_StateFuncs[STATE_NONE] = {
    &MailSystem::State0,  &MailSystem::State1,  &MailSystem::State2,  &MailSystem::State3,  &MailSystem::State4,
    &MailSystem::State5,  &MailSystem::State6,  &MailSystem::State7,  &MailSystem::State8,  &MailSystem::State9,
    &MailSystem::State10, &MailSystem::State11, &MailSystem::State12, &MailSystem::State13, &MailSystem::State14,
    &MailSystem::State15, &MailSystem::State16, &MailSystem::State17, &MailSystem::State18, &MailSystem::State19,
    &MailSystem::State20, &MailSystem::State21, &MailSystem::State22, &MailSystem::State23, &MailSystem::State24,
    &MailSystem::State25, &MailSystem::State26, &MailSystem::State27, &MailSystem::State28, &MailSystem::State29,
    &MailSystem::State30, &MailSystem::State31, &MailSystem::State32, &MailSystem::State33, &MailSystem::State34,
    &MailSystem::State35,
};
} // namespace script
