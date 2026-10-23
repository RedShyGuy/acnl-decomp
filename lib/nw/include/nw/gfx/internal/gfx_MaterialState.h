#pragma once

#include "decomp.h"
#include "nn/math/math_MTX44.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResTextureCoordinatorData.h"

namespace nw {
namespace gfx {
class RenderContext;
class SceneEnvironment;
class ShaderProgram;

namespace internal {
// The commands of the parts of a material that the material activators share (the class name and
// the functions with sources are from the reference symbols, the rest is ours).
class MaterialState
{
public:
    // the texture matrix of a coordinator from its scale, rotation (radians) and translation
    static nn::math::MTX44* SetupTextureMatrix(nn::math::MTX44* pMatrix, res::ResTextureCoordinator::MappingMatrixMode mode, f32 scaleS,
                                               f32 scaleT, f32 rotate, f32 translateS, f32 translateT); // 0x004B0A94 | nintendogs:bytes-fuzzy [tier A]
    // the colors of the material with the lights of the environment (isSpecular1Raw: the second
    // specular color of a light as it is)
    static void ActivateMaterialColor(const SceneEnvironment& environment, const ShaderProgram* pProgram,
                                      res::ResMaterialColor color, bool isSpecular1Raw); // 0x004B0D2C | nintendogs:bytes [tier A]
    // the user uniforms of the material and the defaults of the other symbols of the program (name is ours)
    static void ActivateUserUniforms(ShaderProgram* pProgram, res::ResMaterial material); // 0x004B1108 (name is ours)
    // the texture coordinators: the texture unit configuration, the texture matrices (c11, c14, c17)
    // and the coordinate sources (c10; name is ours)
    static void ActivateTextureCoordinators(RenderContext* pRenderContext, ShaderProgram* pProgram, res::ResMaterial material); // 0x004B16B4 (name is ours)
    // the lookup tables of the fragment lighting and their inputs
    static void ActivateFragmentLightingTable(res::ResFragmentLighting lighting, res::ResFragmentLightingTable table); // 0x004B1D14 | nintendogs (C)
    // the first texture coordinator of a particle material
    static void ActivateParticleTextureCoordinators(RenderContext* pRenderContext, const ShaderProgram* pProgram, res::ResMaterial material); // 0x004B2134 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace gfx
} // namespace nw
