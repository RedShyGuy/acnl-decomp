#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector4.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_IMaterialActivator.h"
#include "nw/gfx/gfx_SceneEnvironment.h"
#include "nw/gfx/gfx_ShaderProgram.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"

namespace nw {
namespace gfx {
class Camera;
class FragmentLight;
class Material;
class Model;
class SkeletalModel;

// RTTI N2nw3gfx13RenderContextE @ 0x008D0610
// vtable 0x00902544 (vptr 0x0090254C), offset_to_top 0, 2 entries
// What the renderer draws with: the shader program, the material, model and camera sent last, the
// scene environment and the hashes of the material parts sent last (the class, MaterialHash and
// Builder names and the functions are from the reference symbols, the rest is ours).
class RenderContext : public GfxObject
{
public:
    class Builder;

    // the hashes of the material parts sent last (one per Material::BufferFlag; -1: send again)
    class MaterialHash
    {
    public:
        // (inline) all hashes reset
        MaterialHash() { ResetMaterialHash(0x1FFF); }
        // the hashes of the parts in flags (Material::BufferFlag) reset
        void ResetMaterialHash(int flags); // 0x00495DB0 | nintendogs:bytes [tier A]

        u32 m_UserUniformsHash;         // 0x00
        u32 m_ShadingHash;                    // 0x04
        u32 m_MaterialColorHash;        // 0x08
        u32 m_RasterizationHash;                    // 0x0C
        u32 m_TextureCoordinatorsHash;  // 0x10
        u32 m_TextureMappersHash;       // 0x14
        u32 m_FragmentLightingHash;                    // 0x18
        u32 m_FragmentLightingTableHash; // 0x1C
        u32 m_TextureCombinersHash;                    // 0x20
        u32 m_FragmentOperationHash;    // 0x24
        u32 m_CombinerBufferHash;                   // 0x28
    };

    // parts of the state the context does not send (m_RenderModeFlags)
    enum RenderModeFlag
    {
        RENDER_MODE_NO_MATERIAL = 1 << 0,
        RENDER_MODE_NO_SHADER_PROGRAM = 1 << 1,
        RENDER_MODE_NO_SCENE_ENVIRONMENT = 1 << 2,
    };
    // what ResetState forgets
    enum ResetFlag
    {
        RESET_MODEL = 1 << 0,
        RESET_MATERIAL = 1 << 1,
        RESET_ACTIVE_MATERIAL = 1 << 2,
        RESET_SCENE_CAMERA = 1 << 3,
        RESET_CAMERA = 1 << 4,
        RESET_SCENE_ENVIRONMENT = 1 << 5,
        RESET_SHADER_PROGRAM = 1 << 6,
        RESET_LOOKUP_TABLES = 1 << 7,
        RESET_MATERIAL_HASH = 1 << 8,
        RESET_VERTEX_ATTRIBUTE = 1 << 9,
    };

    virtual ~RenderContext(); // 0x00497454 slot 0x00 | nintendogs:bytes

    // the parts in flags forgotten (the hashes in hashFlags), so that they are sent again
    void ResetState(int flags, int hashFlags); // 0x00495AFC | nintendogs:bytes [tier B]
    void ResetState(); // 0x00495BBC | nintendogs:bytes [tier B]
    void ActivateFog(); // 0x00495C10 | nintendogs:bytes [tier A]
    // the material m_pMaterial sent: its shader program, the scene environment and the material
    // itself with the activator (NULL: the one of its model)
    void ActivateContext(IMaterialActivator* pActivator); // 0x00495E6C | nintendogs:callseq [tier A]
    void RenderPrimitive(res::ResPrimitive primitive); // 0x00495F6C | nintendogs:bytes [tier B]
    // the view and projection matrices of the camera as uniforms (nothing if it was sent already)
    void SetCameraMatrix(Camera* pCamera, bool isForced); // 0x0049605C | nintendogs:bytes [tier A]
    void ActivateFragmentLight(int lightIdx, const FragmentLight* pLight); // 0x00496140 | nintendogs:callgraph [tier A]
    void SetModelMatrixForModel(Model* pModel); // 0x00496858 | nintendogs:bytes-fuzzy [tier A]
    void ActivateHemiSphereLight(); // 0x00496944 | nintendogs:bytes [tier B]
    void ActivateVertexAttribute(res::ResMesh mesh); // 0x00496AE4 | nintendogs:bytes [tier A]
    void ActivateSceneEnvironment(); // 0x00496B8C | nintendogs:bytes [tier A]
    // the position (w 1) or direction (w 0) in view space
    void TransformToViewCoordinate(nn::math::VEC4* pOut, const nn::math::MTX34* pViewMatrix, const nn::math::VEC4* pIn); // 0x00496CB4 | nintendogs:bytes [tier A]
    void SetModelMatrixForSkeletalModel(SkeletalModel* pModel); // 0x00496D30 | nintendogs:bytes-fuzzy [tier A]

    ShaderProgram* GetShaderProgram() const { return m_ShaderProgram.Get(); }
    void SetMaterial(const Material* pMaterial) { m_pMaterial = pMaterial; }
    Model* GetModel() const { return m_pModel; }

    // (inline in Builder::Create)
    RenderContext(os::IAllocator* pAllocator, GfxPtr<ShaderProgram> shaderProgram, const SceneEnvironment::Description& description,
                  GfxPtr<IMaterialActivator> particleMaterialActivator)
        : GfxObject(pAllocator), m_ShaderProgram(shaderProgram), m_Unknown0x10(false), m_Unknown0x11(false), m_IsVertexAttributeDirty(true),
          m_IsShaderProgramChanged(true), m_RenderModeFlags(0), m_Unknown0x18(0), m_pModel(nullptr), m_pMaterial(nullptr),
          m_pActiveMaterial(nullptr), m_pCamera(nullptr), m_SceneEnvironment(description), m_MaterialHash(),
          m_ParticleMaterialActivator(particleMaterialActivator)
    {
    }

    GfxPtr<ShaderProgram> m_ShaderProgram;                        // 0x008
    bool m_Unknown0x10;                                           // 0x010
    bool m_Unknown0x11;                                           // 0x011
    bool m_IsVertexAttributeDirty;                                // 0x012 all attributes are cleared first
    bool m_IsShaderProgramChanged;                                // 0x013
    u32 m_RenderModeFlags;                                        // 0x014 RenderModeFlag
    u32 m_Unknown0x18;                                            // 0x018
    Model* m_pModel;                                              // 0x01C the model whose matrices were sent
    const Material* m_pMaterial;                                  // 0x020 the material to send
    const Material* m_pActiveMaterial;                            // 0x024 the material sent last
    Camera* m_pCamera;                                            // 0x028 the camera whose matrices were sent
    u32 m_Unknown0x2C[5];                                         // 0x02C
    SceneEnvironment m_SceneEnvironment;                          // 0x040
    u8 m_Unknown0xF0[0x10];                                       // 0x0F0
    MaterialHash m_MaterialHash;                                  // 0x100
    GfxPtr<IMaterialActivator> m_ParticleMaterialActivator;       // 0x12C
    u32 m_Unknown0x134[3];                                        // 0x134
};
ASSERT_OFFSET(RenderContext, m_SceneEnvironment, 0x40);
ASSERT_OFFSET(RenderContext, m_MaterialHash, 0x100);
ASSERT_SIZE(RenderContext, 0x140);
} // namespace gfx
} // namespace nw
