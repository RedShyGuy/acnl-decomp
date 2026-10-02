#pragma once

#include "decomp.h"
#include "Bs/dBsAmiiboPhotoMgr.h"
#include "Other/dICameraUpdater.h"

// vtable +0x15D68 in ModuleAmiiboCamera.cro, offset_to_top 0, 1 entries
class BsAmiiboPhotoMgr::CstmCamera : public ::ICameraUpdater
{
public:
    CstmCamera(); // ctor address unknown
};
