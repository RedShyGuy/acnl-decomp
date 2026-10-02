#pragma once

#include "decomp.h"
#include "lyt/dObject.h"

namespace lyt {
// RTTI N3lyt8ObjectExE @ 0x008D0F90
// vtable 0x0090455C (vptr 0x00904564), offset_to_top 0, 4 entries
class ObjectEx : public ::lyt::Object
{
public:
    ObjectEx(); // ctor address unknown
    virtual ~ObjectEx(); // 0x0050A3AC slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x0050A7EC slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
};
} // namespace lyt
