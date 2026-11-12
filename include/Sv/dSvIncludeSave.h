#pragma once

#include "decomp.h"

// Functions of ModuleIncSave (the import of the save data of the game before Welcome amiibo, BsIncludeSave) that the save
// thread calls through import thunks (static.crs: anonymous imports of the module; the default target 0x007B2C88 is the
// handler for unresolved imports). The module is not decompiled yet; the names are after the addresses.

// a step of the import (flags & SvStepInner::FLAG_INCLUDE_SAVE_MASK); the result of the request (2: go on)
s32 Func14FDF8(u64 step); // 0x0014FDF8 (ModuleIncSave.cro +0x01427C)
// asked when garden_plus.dat is missing: true if there is something to import; else *isBroken / *isSecureValueWrong
bool Func14FE00(bool* isBroken, bool* isSecureValueWrong); // 0x0014FE00 (ModuleIncSave.cro +0x013B84)
// the loads of the file kinds 14 and 18 (a result of SvSaveIo)
s32 Func14FE08(); // 0x0014FE08 (ModuleIncSave.cro +0x013C88)
s32 Func14FE10(); // 0x0014FE10 (ModuleIncSave.cro +0x013FC0)
// the target of the import thunks while the module is not loaded (passes "App" to an empty function; also in the vtables
// of classes of modules; symbols.json calls it AcStrcCampingCar::vf_0x50)
s32 Func7B2C88(); // 0x007B2C88
