#pragma once

#include "decomp.h"

namespace script {
class RenderGarden
{
public:
    RenderGarden(); // TODO: default ctor added so derived stubs compile - may not exist
    void SetTextBoxes(nw::lyt::TextBox*, nw::lyt::TextBox*); // 0x005E3768 | libgarden [tier A]
    void Clear(); // 0x005E38D4 | libgarden [tier A]
    void CommitText(); // 0x005E3A64 | libgarden [tier A]
    RenderGarden(unsigned int); // 0x005E3BF8 | libgarden [tier A]
    ~RenderGarden(); // 0x005E3C64 | libgarden [tier A]
};
} // namespace script
