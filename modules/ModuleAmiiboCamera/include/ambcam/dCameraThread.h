#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace ambcam {
// vtable +0x15E48 in ModuleAmiiboCamera.cro, offset_to_top 0, 18 entries
// vtable +0x15E98 in ModuleAmiiboCamera.cro, offset_to_top -24, 1 entries
class CameraThread : public ::sead::Thread
{
public:
    CameraThread(); // ctor address unknown
    virtual ~CameraThread(); // ModuleAmiiboCamera.cro +0x010DFC slot 0x00
};
} // namespace ambcam
