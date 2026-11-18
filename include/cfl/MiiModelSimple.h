#pragma once

#include "decomp.h"
#include "cfl/MiiModelBase.h"

namespace cfl {
// a Mii model with its CFLCharModel and the model buffer in the allocator of the model (class name from the RTTI; the
// member names are ours)
// RTTI N3cfl14MiiModelSimpleE @ 0x008D0C4C
// vtable 0x009038E8 (vptr 0x009038F0), offset_to_top 0, 9 entries
class MiiModelSimple : public ::cfl::MiiModelBase
{
public:
    MiiModelSimple(); // 0x004DFDE8
    virtual ~MiiModelSimple() override;

    virtual bool InitModel() override; // 0x004DFBB0 slot 0x08
    virtual bool IsReady() override; // 0x004DFDE0 slot 0x0C
    virtual CFLCharModel* GetCharModel() override; // 0x004DFD10 slot 0x10
    virtual bool AllocateBuffers() override; // 0x004DFD18 slot 0x18
    virtual void FreeBuffers() override; // 0x004DFC80 slot 0x1C
    virtual bool SetExpression(const s32& expression) override; // 0x004DFD9C slot 0x20

    CFLCharModel* m_pCharModel; // 0x88
    void* m_pBuffer;            // 0x8C (CFL_GetModelBufferSize)
};
ASSERT_SIZE(MiiModelSimple, 0x90);
} // namespace cfl
