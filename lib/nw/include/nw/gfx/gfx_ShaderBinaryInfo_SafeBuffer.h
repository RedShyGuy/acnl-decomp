#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_ShaderBinaryInfo.h"

class nw::gfx::ShaderBinaryInfo::SafeBuffer
{
public:
    void Write(const unsigned*, int); // 0x0049C3F8 | nintendogs:bytes [tier A]
};
