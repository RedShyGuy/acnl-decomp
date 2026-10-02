#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class ShaderBinaryInfo
{
public:
    class SafeBuffer;
    void AnalyzeBinary(); // 0x0049C448 | nintendogs:bytes [tier A]
    void PutLoadCommand(nw::gfx::ShaderBinaryInfo::SafeBuffer&, unsigned, const unsigned*, unsigned) const; // 0x00739BC8 | nintendogs:bytes-fuzzy [tier A]
    void BuildCommonCommand(unsigned*, unsigned) const; // 0x00739CD4 | nintendogs:bytes [tier B]
    void SearchUniformIndex(int, const char*) const; // 0x00739DD8 | mk7dlp:bytes [tier B]
    void BuildOutAttrCommand(nw::gfx::ShaderBinaryInfo::SafeBuffer&, int, int) const; // 0x00739ECC | nintendogs:bytes-fuzzy [tier A]
    void BuildProgramCommand(nw::gfx::ShaderBinaryInfo::SafeBuffer&) const; // 0x0073A2E8 | nintendogs:bytes [tier A]
    void BuildConstRegCommand(nw::gfx::ShaderBinaryInfo::SafeBuffer&, int) const; // 0x0073A3E4 | nintendogs:bytes [tier A]
    void BuildShaderProgramCommand(int, int, unsigned*, unsigned) const; // 0x0073A684 | nintendogs:bytes [tier A]
    void GetShaderProgramCommandSize(int, int) const; // 0x0073A730 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
