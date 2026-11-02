#pragma once

#include "decomp.h"

namespace graphic {
// an object of a CRO module that draws before the scenes of g3d (the class name and all names are ours)
class ModuleRenderer
{
public:
    // (an import thunk into the module)
    void Draw(u32 drawMode); // 0x001501B8

    // the instance (NULL: no module)
    static ModuleRenderer* s_pInstance;
};
} // namespace graphic
