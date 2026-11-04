#pragma once

#include "decomp.h"
#include "Other/dMigrationReceiver.h"
#include "net/dScanBufBase.h"

// RTTI N17MigrationReceiver7ScanBufE @ 0x008CDAFC
// vtable 0x008FBA88 (vptr 0x008FBA90), offset_to_top 0, 4 entries
class MigrationReceiver::ScanBuf : public ::net::ScanBufBase
{
public:
    ScanBuf(); // ctor candidate(s) 0x002CFA68 (unverified)
    virtual ~ScanBuf(); // 0x002CFA90 slot 0x00
    // 0x002CFA8C slot 0x04 (deleting dtor)
    virtual void* GetBuffer(); // 0x002CFA60 slot 0x08
    virtual u32 GetBufferSize(); // 0x0071FF74 slot 0x0C
};
