#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// A shader symbol (uniform or input) found with Shader::SearchBindSymbol. The derived classes
// only set the shader and symbol type; member names are ours.
class BindSymbol
{
public:
    // shader type: 0 vertex, 1 geometry
    // symbol type: 1 input, 2 float uniform, 3 integer uniform, 4 bool uniform
    BindSymbol(u8 shaderType, u8 symbolType)
        : m_ShaderType(shaderType), m_SymbolType(symbolType), m_Start(0xFF), m_End(0xFF), m_Name(NULL)
    {
    }

    u8 m_ShaderType;    // 0x00
    u8 m_SymbolType;    // 0x01
    u8 m_Start;         // 0x02, first register of the symbol (relative to its register file)
    u8 m_End;           // 0x03, last register
    const char* m_Name; // 0x04
};
ASSERT_SIZE(BindSymbol, 0x8);

class BindSymbolVSInput : public BindSymbol
{
public:
    BindSymbolVSInput(); // 0x00349C30 (not in the symbols)
};
} // namespace CTR
} // namespace gr
} // namespace nn
