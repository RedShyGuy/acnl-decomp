#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x66710 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpPostOffice : public ::AcNpcSp
{
public:
    class AddSaveHook;
    class DownloadHook;
    class LoadHandleHook;
    class PostOfficeTalkRecept;
    class SaveHandleHook;
    AcNpcSpPostOffice(); // ctor address unknown
    virtual ~AcNpcSpPostOffice(); // ModuleIndoor.cro +0x023DE8 slot 0x00
};
