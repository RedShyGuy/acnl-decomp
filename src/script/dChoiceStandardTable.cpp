#include "script/dChoiceStandard.h"
#include "script/dChoiceStandardWin.h"
#include "Sound/dSoundLabel.h"

// The state tables of ChoiceStandard (a file of their own, else GCC folds them into the code).

namespace script {
// 0x00892800
const ChoiceStandard::StateFunc ChoiceStandard::s_EnterFuncs[STATE_NUM] = {
    &ChoiceStandard::EnterIdle,
    &ChoiceStandard::EnterSetup,
    &ChoiceStandard::EnterSelect,
    &ChoiceStandard::EnterDecide,
    NULL,
};

// 0x00892828
const ChoiceStandard::StateFunc ChoiceStandard::s_UpdateFuncs[STATE_NUM] = {
    &ChoiceStandard::UpdateIdle,
    &ChoiceStandard::UpdateSetup,
    &ChoiceStandard::UpdateSelect,
    &ChoiceStandard::UpdateDecide,
    NULL,
};
// 0x008927D4
const u32 ChoiceStandard::s_KindSounds[11] = {
    SE_SYS_DECIDE, SE_SYS_CANCEL, SE_SYS_ENTER_TITLE, SE_SYS_WIN_ATM_MENU_DECIDE, SE_SYS_WIN_ATM_MENU_CANCEL, SE_SYS_DECIDE_L,
    SE_SYS_RETURN_TITLE, SE_SYS_WIN_SELECT_ITEM_DECIDE, SE_SYS_ERASE, SE_SYS_COPY, SE_SYS_PUBLIC_SET_PLACE,
};

// 0x00892850
const f32 ChoiceStandardWin::s_SliderSteps[7] = {0x1.24924ap-4f, 0x1.b6db6ep-3f, 0x1.6db6dcp-2f, 0.5f, 0x1.492492p-1f,
                                                 0x1.924924p-1f, 0x1.db6db6p-1f};
} // namespace script
