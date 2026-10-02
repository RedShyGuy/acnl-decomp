#pragma once

#include "decomp.h"
#include "Other/dInOutWindowInButton.h"

// vtable +0x155CC in ModuleAmiiboCamera.cro, offset_to_top 0, 17 entries
class PhotoExplainWindow : public ::InOutWindowInButton<1>
{
public:
    PhotoExplainWindow(); // ctor address unknown
    virtual ~PhotoExplainWindow(); // ModuleAmiiboCamera.cro +0x00D774 slot 0x00
};
