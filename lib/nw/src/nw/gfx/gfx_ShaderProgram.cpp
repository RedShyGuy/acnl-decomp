#include "nw/gfx/gfx_ShaderProgram.h"

#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/detail/gx_Shader.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/gfx_ShaderBinaryInfo.h"
#include "nw/gfx/internal/internal_Api.h"

namespace nw {
namespace gfx {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_VSH_COM_MODE = 0x244;
const u32 REG_GSH_BOOLUNIFORM = 0x280;
const u32 REG_VSH_BOOLUNIFORM = 0x2B0;

// the upper half of a BOOLUNIFORM value
const u32 BOOL_UNIFORM_HIGH = 0x7FFF0000;
// the type of a bool constant of a DVLE
const u16 CONSTANT_TYPE_BOOL = 0;

// the bool constants of a shader as the bits of its bool uniforms
inline u32 GetBoolConstants(const nn::gx::detail::DvleHeader* pShader)
{
    const nn::gx::detail::DvleConstant* pConstants = reinterpret_cast<const nn::gx::detail::DvleConstant*>(
        reinterpret_cast<const u8*>(pShader) + pShader->m_ConstantOffset);
    u32 bits = 0;
    for (int i = 0; i < static_cast<int>(pShader->m_ConstantCount); ++i) {
        if (pConstants[i].m_Type == CONSTANT_TYPE_BOOL) {
            bits |= (pConstants[i].m_Values[0] & 1) << pConstants[i].m_Id;
        }
    }
    return bits;
}

// the commands copied to the current command list
inline void PutCommands(const u32* pCommand, u32 size)
{
    nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, pCommand, size);
    nn::gx::detail::s_pCommandBufferCurrent =
        reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + (size & ~3));
}
} // namespace

// 0x004975B8 | nintendogs:bytes [tier B]
void nw::gfx::ShaderProgram::ActivateDescription(res::ResShaderProgramDescription description)
{
    const res::ResBinaryShaderData* pBinary = description.ptr()->owner.To<res::ResBinaryShaderData>();
    const bool isGeometryShaderEnabled = description.ptr()->geometryShaderIndex >= 0;
    bool isSameMode = false;
    bool isSameBinary = false;
    if (m_ActiveDescription.ptr() != nullptr) {
        if (isGeometryShaderEnabled == (m_ActiveDescription.ptr()->geometryShaderIndex >= 0)) {
            isSameMode = true;
        }
        if (pBinary == m_ActiveDescription.ptr()->owner.To<res::ResBinaryShaderData>()) {
            isSameBinary = true;
        }
    }
    if (!isSameMode) {
        internal::SetupShaderProgramMode(isGeometryShaderEnabled);
        m_GeometryMode = isGeometryShaderEnabled ? GEOMETRY_MODE_GEOMETRY : GEOMETRY_MODE_VERTEX;
        m_ProgramModeCommand[0] = isGeometryShaderEnabled;
    }
    if (!isSameBinary) {
        PutCommands(pBinary->pCommonCommand, pBinary->commonCommandSize);
    }
    PutCommands(description.ptr()->pCommand, description.ptr()->commandSize);

    // the bool constants of the shaders
    const ShaderBinaryInfo* pInfo = pBinary->pBinaryInfo;
    m_VertexBoolUniformCommand[0] |= GetBoolConstants(
        static_cast<const nn::gx::detail::DvleHeader*>(pInfo->m_pShaders[description.ptr()->vertexShaderIndex]));
    if (isGeometryShaderEnabled) {
        m_GeometryBoolUniformCommand[0] |= GetBoolConstants(
            static_cast<const nn::gx::detail::DvleHeader*>(pInfo->m_pShaders[description.ptr()->geometryShaderIndex]));
    }
    m_ActiveDescription = description;
}

// 0x0049777C | nintendogs:bytes [tier A]
nw::gfx::ShaderProgram::ShaderProgram(os::IAllocator* pAllocator)
    : GfxObject(pAllocator), m_ActiveDescription(nullptr), m_Unknown0xC(0), m_GeometryMode(GEOMETRY_MODE_NONE),
      m_VertexBoolUniformCommand{BOOL_UNIFORM_HIGH, Header(REG_VSH_BOOLUNIFORM, 0xF, 4, true), 0, 0, 0, 0},
      m_GeometryBoolUniformCommand{BOOL_UNIFORM_HIGH, Header(REG_GSH_BOOLUNIFORM, 0xF, 4, true), 0, 0, 0, 0},
      m_ProgramModeCommand{0, Header(REG_VSH_COM_MODE, 0x1)}
{
}

// 0x00497804 slot 0x00
// 0x00497800 (deleting dtor)
nw::gfx::ShaderProgram::~ShaderProgram()
{
    // empty (in the original too)
}

} // namespace gfx
} // namespace nw
