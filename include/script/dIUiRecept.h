#pragma once

#include "decomp.h"
#include "script/dIRecept.h"

namespace script {
struct Tag;
class UiSystem;

// The recept of the texts of the user interface (UiMgr::Begin); slot and member names are ours.
// RTTI N6script9IUiReceptE @ 0x008D3E80
// vtable 0x0090BB0C (vptr 0x0090BB14), offset_to_top 0, 39 entries
class IUiRecept : public ::script::IRecept
{
public:
    // (inline in the derived classes)
    DECOMP_EMIT_COPY IUiRecept() : m_pOwner(NULL) {} // 0x00600F00
    virtual ~IUiRecept(); // 0x00600F1C slot 0x00
    // 0x00600F14 slot 0x04 (deleting dtor)
    virtual s32 GetSex(); // 0x0075DD4C slot 0x8C
    virtual s32 GetSpeakerSex(); // 0x0075DD54 slot 0x90
    // the tags UiSeq leaves to the recept (two groups; true: written)
    virtual bool ProcessTag94(const Tag* tag); // 0x00600EF8 slot 0x94
    virtual bool ProcessTag98(const Tag* tag); // 0x00600EF0 slot 0x98

    UiSystem* m_pOwner; // 0x04 (set by UiMgr::Begin)
};
ASSERT_SIZE(IUiRecept, 0x8);
} // namespace script
