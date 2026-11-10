#pragma once

#include "decomp.h"
#include "sead/seadVector2.h"

namespace sead {
class Controller;
}

// The input of the game: the first sead controller, nothing while the error/EULA applet blocks the input. Class,
// Pad bits and the functions IsTrig/IsHold/IsRelease/IsRepeat/GetCirclePad after libgarden; the other names are ours.
class Controller
{
public:
    // the buttons (libgarden)
    enum Pad : u32
    {
        A = 1 << 0,
        B = 1 << 1,
        X = 1 << 3,
        Y = 1 << 4,

        START = 1 << 11,
        SELECT = 1 << 12,

        L = 1 << 13,
        R = 1 << 14,
        TOUCH = 1 << 15,
        DPAD_UP = 1 << 16,
        DPAD_DOWN = 1 << 17,
        DPAD_LEFT = 1 << 18,
        DPAD_RIGHT = 1 << 19,

        CPAD_UP = 1 << 20,
        CPAD_DOWN = 1 << 21,
        CPAD_LEFT = 1 << 22,
        CPAD_RIGHT = 1 << 23,

        RIGHT = DPAD_RIGHT | CPAD_RIGHT,
        LEFT = DPAD_LEFT | CPAD_LEFT,
        UP = DPAD_UP | CPAD_UP,
        DOWN = DPAD_DOWN | CPAD_DOWN,

        DPAD = DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN,
        CPAD = CPAD_RIGHT | CPAD_LEFT | CPAD_UP | CPAD_DOWN,

        DIR = RIGHT | LEFT | UP | DOWN,
    };

    // the circle pad of the CTR controller is scaled with 1.0 and threshold 141
    static void SetupCirclePad(); // 0x005222F4
    static bool IsRelease(u32 mask); // 0x005223A0 (name after libgarden)
    static bool IsTrig(u32 mask); // 0x005223E8 | libgarden [tier A]
    // A was pressed and Func5C37C4 allows it (what that checks is not known yet)
    static bool IsTrigAIfAllowed(); // 0x00522430
    static void Initialize(); // 0x00522480
    static void Finalize(); // 0x00522484
    // the touch position (s_InvalidPointer while not touched)
    static const sead::Vector2<f32>& GetPointer(); // 0x00522488
    static const sead::Vector2<f32>& GetCirclePad(); // 0x00522520 | libgarden [tier A]
    static const sead::Vector2<s32>& GetPointerS32(); // 0x005225A4
    // pressed or repeated
    static bool IsRepeat(u32 mask); // 0x00522638 (name after libgarden)
    static bool IsHold(u32 mask); // 0x00304A14 | libgarden [tier A]

    // (inline) the first controller of the manager (NULL: none)
    static sead::Controller* GetController();
};

// game code that is not decompiled yet (name after the address)
bool Func5C37C4(); // 0x005C37C4
