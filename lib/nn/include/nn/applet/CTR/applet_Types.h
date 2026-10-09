#pragma once

// Types of the applet library. The type names are from the binary (mangled signatures of
// nn::applet::CTR); like all ARMCC enums they are one byte. Values after 3dbrew "NS and APT
// Services" where it lists them, otherwise from the code (names are ours).

#include "decomp.h"

namespace nn {
namespace applet {
namespace CTR {

// how the application runs (APT command 0x0103); nn::os::CTR::IsRunningAsExtApplication checks
// for 2 and 4
enum ApplicationRunningMode : u8 {
};

// the answer to a sleep query (3dbrew "QueryReply")
enum QueryReply : u8 {
    REPLY_REJECT = 0,
    REPLY_ACCEPT = 1,
    REPLY_LATER = 2,
};

// the slots of the applet manager (3dbrew "AppletPos")
enum AppletPos : u8 {
    POS_APP = 0,
    POS_APPLIB = 1,
    POS_SYS = 2,
    POS_SYSLIB = 3,
    POS_RESIDENT = 4,
    POS_NONE = 0xFF,
};

// the kind of program jumped to by PrepareToJumpApplication (values from RestartApplication;
// names are ours)
enum AppJumpType : u8 {
    APP_JUMP_TYPE_RESTART = 2,
};

// what the program is doing when it waits for its wakeup (names are ours)
enum TransitionType : u8 {
    TRANSITION_NONE = 0,
    TRANSITION_CANCEL_LIBRARY_APPLET = 3,
    TRANSITION_START_LIBRARY_APPLET = 4,
    TRANSITION_START_SYSTEM_APPLET = 5,
    TRANSITION_CLOSE_APPLICATION = 9,
    TRANSITION_JUMP_TO_HOME_MENU = 14,
    TRANSITION_APPLICATION_JUMP = 18,
    TRANSITION_FIRST_WAKEUP = 98, // Enable of an application: wait for its first wakeup
    TRANSITION_NO_WAIT = 99,      // WaitForStarting returns at once
};

// why the program was woken up (from WaitForStarting: the wakeup commands of 3dbrew "Command"
// mapped to these values; names are ours)
enum WakeupState : u8 {
    WAKEUP_STATE_NONE = 0,
    WAKEUP_STATE_WAKEUP = 1,                   // COMMAND_WAKEUP and unknown commands
    WAKEUP_STATE_BY_EXIT = 2,                  // COMMAND_WAKEUP_BY_EXIT
    WAKEUP_STATE_BY_PAUSE = 3,                 // COMMAND_WAKEUP_BY_PAUSE
    WAKEUP_STATE_BY_CANCEL = 4,                // COMMAND_WAKEUP_BY_CANCEL
    WAKEUP_STATE_BY_CANCEL_ALL = 5,            // COMMAND_WAKEUP_BY_CANCELALL
    WAKEUP_STATE_BY_POWER_BUTTON_CLICK = 6,    // COMMAND_WAKEUP_BY_POWER_BUTTON_CLICK
    WAKEUP_STATE_TO_JUMP_HOME = 7,             // COMMAND_WAKEUP_TO_JUMP_HOME
    WAKEUP_STATE_TO_LAUNCH_APPLICATION = 9,    // COMMAND_WAKEUP_TO_LAUNCH_APPLICATION
    WAKEUP_STATE_COMMAND_18 = 10,              // command 18
    WAKEUP_STATE_COMMAND_64 = 64,              // commands 0x40 / 0x41 are passed on
    WAKEUP_STATE_COMMAND_65 = 65,
};

// the HOME button presses that are not handled yet (names are ours)
enum HomeButtonState : u8 {
    HOME_BUTTON_NONE = 0,
    HOME_BUTTON_SINGLE = 1, // NOTIFICATION_HOME_BUTTON_1
    HOME_BUTTON_DOUBLE = 2, // NOTIFICATION_HOME_BUTTON_2
};

// how far a sleep query has been answered (names are ours)
enum SleepNotificationState : u8 {
    SLEEP_NOTIFICATION_NONE = 0,
    SLEEP_NOTIFICATION_LATER = 1,    // answered REPLY_LATER, ReplySleepQuery has to follow
    SLEEP_NOTIFICATION_ACCEPTED = 2,
    SLEEP_NOTIFICATION_REJECTED = 3,
    SLEEP_NOTIFICATION_SLEEPING = 4, // NOTIFICATION_SLEEP_ACCEPTED
    SLEEP_NOTIFICATION_AWAKE = 5,    // NOTIFICATION_SLEEP_AWAKE
};

// set by NOTIFICATION_POWER_BUTTON_CLICK, cleared by NOTIFICATION_POWER_BUTTON_CLEAR (names are ours)
enum PowerButtonState : u8 {
    POWER_BUTTON_NONE = 0,
    POWER_BUTTON_CLICKED = 1,
};

// set by NOTIFICATION_SHUTDOWN / NOTIFICATION_ORDER_TO_CLOSE (names are ours)
enum OrderToCloseState : u8 {
    ORDER_TO_CLOSE_NONE = 0,
    ORDER_TO_CLOSE_ORDERED = 1,
};

// the framebuffers of both screens, copied from nn::gxlow::CTR::DisplayCaptureInfo (the member
// names are ours; the elements have an empty constructor, 0x0047F9F0)
struct AppletDisplayInfo
{
    struct Screen
    {
        Screen() {}

        uptr leftAddress;  // 0x0
        uptr rightAddress; // 0x4, == leftAddress without 3D
        u32 format;        // 0x8
        u32 stride;        // 0xC
    };

    Screen screens[2]; // 0x00 top, 0x10 bottom
};
ASSERT_SIZE(AppletDisplayInfo, 0x20);

// where the screen captures are in the buffer handed to the next applet (3dbrew
// "CaptureBufferInfo"; member names are ours; the elements have an empty constructor, 0x0047F9F4)
struct CaptureBufferInfo
{
    struct Screen
    {
        Screen() {}

        u32 leftOffset;  // 0x0
        u32 rightOffset; // 0x4
        u32 format;      // 0x8
    };

    u32 size;          // 0x00
    bool is3D;         // 0x04
    Screen screens[2]; // 0x08 top, 0x14 bottom
};
ASSERT_SIZE(CaptureBufferInfo, 0x20);

namespace detail {
// the word offsets of the four 2x2 pixel blocks of an 8x8 tile, read by ConvertL16ToB16 /
// ConvertL24ToB24 from index 4 down to 1 (the name is from the binary, the member is ours)
struct OffsetTable
{
    u32 offsets[5];
};
} // namespace detail

} // namespace CTR
} // namespace applet
} // namespace nn
