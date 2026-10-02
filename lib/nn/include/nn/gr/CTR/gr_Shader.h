#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class Shader
{
public:
    void SetupBinary(const void*, int, int); // 0x00349E64 | nintendogs:callseq [tier A]
    void MakeOutAttrCommand_(unsigned*, int, int); // 0x00349F50 | nintendogs:callseq [tier A]
    void MakeShaderModeCommand_(unsigned*, bool, PicaDataDrawMode); // 0x0034A93C | nintendogs:bytes [tier A]
    void MakeShaderConstCommandCache_(); // 0x0034AA2C | nintendogs:bytes [tier A]
    Shader(); // 0x0034ABD0 | fefates:bytes [tier B]
    void MakeFullCommand(unsigned*) const; // 0x0072770C | nintendogs:bytes-fuzzy [tier A]
    void MakeLoadCommand_(unsigned*, unsigned, const unsigned*, unsigned) const; // 0x00727944 | nintendogs:bytes [tier A]
    void SearchBindSymbol(nn::gr::CTR::BindSymbol*, const char*) const; // 0x007279E4 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace gr
} // namespace nn
