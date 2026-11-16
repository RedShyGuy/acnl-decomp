#pragma once

#include "decomp.h"
#include "fgobj/dObjectBase.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// A field object that runs every frame in the list of the Proc until it is done (no vtable of its own in the binary:
// only built as a base; the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj9ObjectExeE @ 0x008D2A50
class ObjectExe : public ::fgobj::ObjectBase
{
public:
    // the results of Execute (after what the Proc does with them)
    enum ExecuteResult
    {
        EXECUTE_CONTINUE = 0,
        EXECUTE_FINISHED = 1,               // the object ends and the place request of its unit too
        EXECUTE_FINISHED_KEEP_REQUEST = 2,  // the object ends, the place request of its unit goes on
    };

    // one frame (ExecuteResult)
    virtual s32 Execute() = 0; // slot 0x20
    // the object of item and id is kept for one more frame at position with scale (only ObjectOther; false: another
    // object)
    virtual bool Refresh(const SvFgName* item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id); // 0x005A3638 slot 0x24
};
} // namespace fgobj
