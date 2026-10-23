#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResShader.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13ShaderProgramE @ 0x008D061C
// vtable 0x00902554 (vptr 0x0090255C), offset_to_top 0, 2 entries
// The shader program the render context uses: the active program description, the geometry stage
// mode and the commands of the bool uniforms (the bool constants of the shaders are set as uniforms).
// Member names are ours.
class ShaderProgram : public GfxObject
{
public:
    // the geometry stage set last (m_GeometryMode)
    enum GeometryMode : u8
    {
        GEOMETRY_MODE_NONE = 0,     // not set yet
        GEOMETRY_MODE_VERTEX = 1,   // the vertex shader alone
        GEOMETRY_MODE_GEOMETRY = 2, // with a geometry shader
    };
    // the words of a bool uniform command: the bools, then the four int uniforms
    static const int BOOL_UNIFORM_COMMAND_WORDS = 6;
    // bool uniforms the renderer sets (bits of the first word of m_VertexBoolUniformCommand; names are ours)
    static const u32 BOOL_SMOOTH_SKINNING = 1 << 1;
    static const u32 BOOL_RIGID_SKINNING = 1 << 2;
    static const u32 BOOL_HEMISPHERE_LIGHTING = 1 << 5;
    static const u32 BOOL_HEMISPHERE_OCCLUSION = 1 << 6;
    static const u32 BOOL_USAGE3_HAS_4_COMPONENTS = 1 << 7;
    static const u32 BOOL_USAGE8_HAS_4_COMPONENTS = 1 << 8;
    static const u32 BOOL_FRAGMENT_LIGHTING = 1 << 15;

    explicit ShaderProgram(os::IAllocator* pAllocator); // 0x0049777C | nintendogs:bytes [tier A]
    virtual ~ShaderProgram();

    // the program of the description activated: the geometry stage, the commands of its shader binary
    // (if it changed) and of the program, and the bool constants of its shaders
    void ActivateDescription(res::ResShaderProgramDescription description); // 0x004975B8 | nintendogs:bytes [tier B]

    res::ResShaderProgramDescription GetActiveDescription() const { return m_ActiveDescription; }
    u32* GetVertexBoolUniformCommand() { return m_VertexBoolUniformCommand; }
    u32* GetGeometryBoolUniformCommand() { return m_GeometryBoolUniformCommand; }

    res::ResShaderProgramDescription m_ActiveDescription;      // 0x08
    u32 m_Unknown0xC;                                           // 0x0C
    GeometryMode m_GeometryMode;                                // 0x10
    u32 m_VertexBoolUniformCommand[BOOL_UNIFORM_COMMAND_WORDS];   // 0x14 VSH_BOOLUNIFORM, VSH_INTUNIFORM_I0-I3
    u32 m_GeometryBoolUniformCommand[BOOL_UNIFORM_COMMAND_WORDS]; // 0x2C GSH_BOOLUNIFORM, GSH_INTUNIFORM_I0-I3
    u32 m_ProgramModeCommand[2];                                // 0x44 whether a geometry shader runs (VSH_COM_MODE)
};
ASSERT_SIZE(ShaderProgram, 0x4C);
} // namespace gfx
} // namespace nw
