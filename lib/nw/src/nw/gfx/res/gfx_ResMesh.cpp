#include <cstring>

#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters
// (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0)
{
    return reg | (mask << 16) | (extraCount << 20);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// the float uniform of the scales: c7 (and c8), 32-bit floats
const u32 SCALE_UNIFORM_INDEX = 0x80000000 | 7;

// the word of ResMeshData::scaleCommand that holds the scale of an attribute usage (negative: none).
// A float uniform comes w first: c7 = (usage 0, 1, 2, 3), c8 = (usage 4, 5, 6, 8); usage 7 has no
// scale.
inline int GetScaleCommandIndex(u8 usage)
{
    switch (usage) {
    case 0:
        return 6;
    case 1:
        return 5;
    case 2:
        return 4;
    case 3:
        return 2;
    case 4:
        return 10;
    case 5:
        return 9;
    case 6:
        return 8;
    case 7:
        return -2;
    case 8:
        return 7;
    default:
        return -1;
    }
}
} // namespace

// 0x0013B920 | nintendogs:bytes [tier A]
void nw::gfx::res::ResMesh::Cleanup()
{
    ptr()->flags &= ~ResMeshData::FLAG_SETUP_DONE;
    if (ptr()->pCommand != nullptr) {
        ptr()->pAllocator->Free(ptr()->pCommand);
        ptr()->pCommand = nullptr;
        ptr()->commandSize = 0;
        // the deactivate commands were in the same memory
        if (ptr()->pDeactivateCommand != nullptr) {
            ptr()->pDeactivateCommand = nullptr;
            ptr()->deactivateCommandSize = 0;
        }
    }
}

// 0x004A9E64 | nintendogs:callgraph [tier A]
u32 nw::gfx::res::ResMesh::Setup(ResModel model, os::IAllocator* pAllocator, ResGraphicsFile /*graphicsFile*/)
{
    u32 result = RESULT_SUCCESS;
    ResSeparateDataShape shape(model.GetShape(ptr()->shapeIndex));
    ResMaterial material = model.GetMaterial(ptr()->materialIndex);
    ResShader shader = ResShader(material.ptr()->shader.ToPtr()).Dereference();
    ResShaderProgramDescription description(
        ResBinaryShader(shader.ptr()).GetDescription(material.ptr()->shaderProgramDescriptionIndex));

    // the commands of the attributes and the ones that turn them off again, in one memory
    const u32 commandSize = internal::GetVertexAttributeCommandSize(shape, description);
    const u32 deactivateCommandSize = internal::GetDeactivateVertexAttributeCommandSize(shape);
    const u32 size = commandSize + deactivateCommandSize;
    u32* pBuffer = static_cast<u32*>(pAllocator->Alloc(size, 4));
    internal::CommandBufferInfo info(pBuffer, size);
    internal::SetupVertexAttributeCommand(info, shape, description);
    internal::SetupDeactivateVertexAttributeCommand(info, shape, description);
    ptr()->pCommand = pBuffer;
    ptr()->commandSize = commandSize;
    ptr()->pDeactivateCommand = reinterpret_cast<u32*>(reinterpret_cast<u8*>(ptr()->pCommand) + commandSize);
    ptr()->deactivateCommandSize = deactivateCommandSize;
    ptr()->pAllocator = pAllocator;

    // the scales of the attributes the program uses
    ResMeshData* pMesh = ptr();
    pMesh->flags &= ~(ResMeshData::FLAG_USAGE3_HAS_4_COMPONENTS | ResMeshData::FLAG_USAGE8_HAS_4_COMPONENTS);
    u32* pCommand = pMesh->scaleCommand;
    pCommand[0] = SCALE_UNIFORM_INDEX;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
    pCommand[2] = 0;
    pCommand[3] = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 7);
    for (int i = 4; i < ResMeshData::SCALE_COMMAND_WORDS; ++i) {
        pCommand[i] = 0;
    }
    for (internal::VertexAttributeIterator it(shape.ptr()); !it.IsEnd(); ++it) {
        const ResVertexAttributeData* pAttribute = *it;
        const u32 usage = pAttribute->usage;
        if (description.ptr()->attributeRegisters[usage] < 0) {
            continue;
        }
        u8 dimension;
        if (pAttribute->flags & ResVertexAttributeData::FLAG_CONSTANT) {
            dimension = static_cast<const ResVertexParamAttributeData*>(pAttribute)->dimension;
            const int index = GetScaleCommandIndex(usage);
            if (index >= 0) {
                const f32 scale = 1.0f;
                std::memcpy(&pCommand[index], &scale, sizeof(scale));
            }
        } else {
            const ResVertexStreamData* pStream = static_cast<const ResVertexStreamData*>(pAttribute);
            dimension = pStream->dimension;
            const f32 scale = pStream->scale;
            const int index = GetScaleCommandIndex(usage);
            if (index >= 0) {
                std::memcpy(&pCommand[index], &scale, sizeof(scale));
            }
        }
        if (dimension == 4) {
            if (static_cast<u8>(usage) == 3) {
                pMesh->flags |= ResMeshData::FLAG_USAGE3_HAS_4_COMPONENTS;
            } else if (static_cast<u8>(usage) == 8) {
                pMesh->flags |= ResMeshData::FLAG_USAGE8_HAS_4_COMPONENTS;
            }
        }
    }

    if (!(result & RESULT_FAILURE_MASK)) {
        ptr()->flags |= ResMeshData::FLAG_SETUP_DONE;
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
