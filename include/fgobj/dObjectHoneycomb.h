#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// An object that falls out of a shaken tree and bounces once; on the neighbour unit it stays and fades out (the class
// name is from the RTTI, the rest is ours).
// RTTI N5fgobj15ObjectHoneycombE @ 0x008D2A00
// vtable 0x00908D54 (vptr 0x00908D5C), offset_to_top 0, 10 entries
class ObjectHoneycomb : public ::fgobj::ObjectFall
{
public:
    // the object of item at the unit x, z that falls from offset above it (direction: ObjectFall::m_Direction)
    static ObjectHoneycomb* Create(const SvFgName& item, s32 x, s32 z, const nn::math::VEC3* offset, u8 direction); // 0x00595C7C

    virtual ~ObjectHoneycomb(); // 0x00595FE4 slot 0x00
    // 0x00595FB4 (deleting dtor)
    s32 Execute() override; // 0x00595DDC slot 0x20

    u8 m_FadeFrame; // 0x143 the frames of the fade on the neighbour unit (alpha 4 per frame)
};
ASSERT_SIZE(ObjectHoneycomb, 0x144);
} // namespace fgobj
