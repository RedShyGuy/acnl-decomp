#include "script/dTalkSystem.h"

// The tables of TalkSystem (a file of their own, else GCC folds them into the code).

namespace script {
// 0x00891FD0
const TalkSystem::StateFunc TalkSystem::s_EnterFuncs[STATE_NUM] = {
    &TalkSystem::EnterIdle,
    &TalkSystem::EnterLoad,
    &TalkSystem::EnterOpen,
    &TalkSystem::EnterPage,
    &TalkSystem::EnterWrite,
    &TalkSystem::EnterChoice,
    &TalkSystem::EnterClose,
    &TalkSystem::EnterClosed,
    &TalkSystem::EnterKeep,
};

// 0x00892018
const TalkSystem::StateFunc TalkSystem::s_UpdateFuncs[STATE_NUM] = {
    &TalkSystem::UpdateIdle,
    &TalkSystem::UpdateLoad,
    &TalkSystem::UpdateOpen,
    &TalkSystem::UpdatePage,
    &TalkSystem::UpdateWrite,
    &TalkSystem::UpdateChoice,
    &TalkSystem::UpdateClose,
    &TalkSystem::UpdateClosed,
    &TalkSystem::UpdateKeep,
};

// 0x00892060
const TalkSystem::StateFunc TalkSystem::s_LeaveFuncs[STATE_NUM] = {
    &TalkSystem::LeaveIdle,
    &TalkSystem::LeaveLoad,
    &TalkSystem::LeaveOpen,
    &TalkSystem::LeavePage,
    &TalkSystem::LeaveWrite,
    &TalkSystem::LeaveChoice,
    &TalkSystem::LeaveClose,
    &TalkSystem::LeaveClosed,
    &TalkSystem::LeaveKeep,
};

// 0x008916F6
const u8 TalkSystem::s_NameColors[3] = {0, 1, 2};
// 0x00895500
const u8 TalkSystem::s_WindowColors[11] = {0, 2, 1, 3, 4, 5, 6, 0, 5, 5, 5};
// 0x0089550C
const SeID TalkSystem::s_OpenSounds[5] = {SE_SYS_WIN_TEXT_OPEN, SE_SYS_WIN_TEXT_WARNING_OPEN, SE_SYS_WIN_ERROR_OPEN, SE_SYS_SILENT,
                                          SE_OBJ_CATALOG_MACHINE_ON};
// 0x00895520
const SeID TalkSystem::s_CloseSounds[3] = {SE_SYS_WIN_TEXT_CLOSE, SE_SYS_SILENT, SE_OBJ_CATALOG_MACHINE_OFF};

// 0x00891F10
const u8 TalkFile::s_RegionLanguages[10] = {0, 0, 1, 2, 0, 1, 2, 3, 4, 0};

// 0x00893220
const SpeechSoundEntry s_SpeechSounds[44] = {
    {0x30FC, 157},
    {0xFF5E, 157},
    {0x00B7, 158},
    {0x30FB, 158},
    {0x003F, 160},
    {0xFF1F, 160},
    {0x0021, 161},
    {0xFF01, 161},
    {0x3001, 162},
    {0x3002, 163},
    {0x0020, 164},
    {0x3000, 164},
    {0x002E, 165},
    {0x000A, 166},
    {0x0000, 0},
    {0x0386, 197},
    {0x0388, 198},
    {0x0389, 199},
    {0x038A, 200},
    {0x038C, 201},
    {0x038E, 202},
    {0x038F, 203},
    {0x03AA, 205},
    {0x03AB, 206},
    {0x03AC, 207},
    {0x03AD, 208},
    {0x03AE, 209},
    {0x03AF, 210},
    {0x03CC, 215},
    {0x03CD, 216},
    {0x03CE, 217},
    {0x03CA, 213},
    {0x03CB, 214},
    {0x0390, 204},
    {0x03B0, 211},
    {0x00FF, 156},
    {0x0178, 156},
    {0x00DF, 155},
    {0x0153, 130},
    {0x0152, 130},
    {0xE015, 171},
    {0xE016, 169},
    {0xE017, 168},
    {0xE018, 170},
};
} // namespace script
