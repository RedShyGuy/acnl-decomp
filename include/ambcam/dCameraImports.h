#pragma once

#include "decomp.h"

namespace ambcam {
// Functions of ModuleAmiiboCamera that code.bin calls through import thunks (static.crs: anonymous
// imports of the module). They control the camera thread of the module around a library applet; the
// names are ours.

// stops the camera (polled until true)
bool StopCamera(); // 0x0014FD48 (ModuleAmiiboCamera.cro +0x00AD0C)
// ends and deletes the camera thread
void DestroyCameraThread(); // 0x0014FD50 (ModuleAmiiboCamera.cro +0x00AD74)
// starts the camera again (polled until true)
bool RestartCamera(); // 0x001501C0 (ModuleAmiiboCamera.cro +0x00AE44)
} // namespace ambcam
