#pragma once

#include "decomp.h"
#include "script/dDialogCompo.h"
#include "script/dIUiRecept.h"

// The recept of the dialog texts: the tags left to the recept are written as they are (names are ours).
// RTTI N6script11DialogCompo10TextReceptE @ 0x008D378C
// vtable 0x00909D20 (vptr 0x00909D28), offset_to_top 0, 39 entries
class script::DialogCompo::TextRecept : public ::script::IUiRecept
{
public:
    // (inline)
    explicit TextRecept(DialogCompo* pCompo) : m_pCompo(pCompo) {}
    virtual ~TextRecept(); // 0x005D920C slot 0x00
    // 0x005D9208 slot 0x04 (deleting dtor)
    virtual bool ProcessTag94(const Tag* tag); // 0x005D91D8 slot 0x94
    virtual bool ProcessTag98(const Tag* tag); // 0x005D91A8 slot 0x98

    DialogCompo* m_pCompo;  // 0x8
};
