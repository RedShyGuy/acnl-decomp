#pragma once

#include "decomp.h"
#include "Other/dBkUnitSearchCandCb.h"
#include "Other/dJingle.h"

// vtable +0x131BC in ModuleWinter.cro, offset_to_top 0, 2 entries
class Jingle::JingleBkUnitSearchCand : public ::BkUnitSearchCandCb
{
public:
    JingleBkUnitSearchCand(); // ctor address unknown
};
