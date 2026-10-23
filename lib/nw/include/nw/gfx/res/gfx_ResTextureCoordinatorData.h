#pragma once

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"

namespace nw {
namespace gfx {
namespace res {
// A texture coordinator of a material: how the texture coordinates of a texture unit are made (the
// class name and its copy constructor are from the reference symbols; the members are ours, only
// the mapping method and the enable flag are understood so far).
class ResTextureCoordinatorData
{
public:
    // the mapping methods (names are ours)
    enum MappingMethod : u8
    {
        MAPPING_UV = 0,
        MAPPING_CUBE_ENV = 1,
        MAPPING_SPHERE_ENV = 2,
        MAPPING_PROJECTION = 3,
        MAPPING_SHADOW = 4,
        MAPPING_SHADOW_CUBE = 5,
    };

    // the copy (out of line; the matrix with MTX34CopyAsm)
    ResTextureCoordinatorData(const ResTextureCoordinatorData& other); // 0x004A8FCC | mk7dlp:bytes [tier B]

    MappingMethod GetMappingMethod() const { return static_cast<MappingMethod>(mappingMethod); }

    s32 sourceCoordinate;          // 0x00 the texture coordinate of the vertices (UV mapping)
    u32 mappingMethod;             // 0x04 MappingMethod
    s32 referenceCamera;           // 0x08 the camera of a projection or shadow (-1: the active one)
    u32 mappingMatrixMode;         // 0x0C ResTextureCoordinator::MappingMatrixMode
    nn::math::VEC2 scale;          // 0x10
    f32 rotate;                    // 0x18
    nn::math::VEC2 translate;      // 0x1C
    bool isDirty;                  // 0x24 the texture matrix has to be made again
    bool isEnabled;                // 0x25 (set by ResMaterial::SetupTextures)
    u16 m_Unknown0x26;             // 0x26
    nn::math::MTX34 textureMatrix; // 0x28
};
ASSERT_SIZE(ResTextureCoordinatorData, 0x58);

// the wrapper of a texture coordinator (the class name is from the reference symbols)
class ResTextureCoordinator
{
public:
    // how scale, rotation and translation make the texture matrix (named after the tools whose
    // conventions they follow; names are ours)
    enum MappingMatrixMode : u8
    {
        MAPPING_MATRIX_MODE_MAYA = 0,
        MAPPING_MATRIX_MODE_SOFTIMAGE = 1,
        MAPPING_MATRIX_MODE_MAX = 2,
    };

    // (inline)
    explicit ResTextureCoordinator(const void* pData = nullptr) : m_pData(static_cast<const ResTextureCoordinatorData*>(pData)) {}
    const ResTextureCoordinatorData* ptr() const { return m_pData; }

    const ResTextureCoordinatorData* m_pData; // 0x0
};
ASSERT_SIZE(ResTextureCoordinator, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
