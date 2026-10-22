#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// the samplers each lighting configuration uses: bit 0 sampler 0, bit 1 sampler 1, bit 2 sampler 2
// (the upper bits are not used here; name is ours)
// 0x0090C824 (name is ours)
extern const u8 s_ConfigSamplers[] = {0xC9, 0xE1, 0x99, 0xD8, 0xDF, 0xEF, 0xF9, 0xFF, 0xD1};

// (inline) whether the sampler has a lookup table (name is ours)
inline bool HasTable(ResFragmentLightingTable table, int index)
{
    return table.GetSampler(index).IsValid() && table.GetSampler(index).GetSampler().IsValid();
}
} // namespace

// 0x0013F170 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::res::ResFragmentShader::Cleanup()
{
    ResFragmentLightingTable table = GetLightingTable();
    if (table.m_pData != nullptr) {
        table.Cleanup();
    }
}

// 0x004A7984 | nintendogs:bytes [tier B]
bool nw::gfx::res::ResFragmentShader::CheckFragmentShader(ResMaterialData* pMaterial)
{
    ResFragmentLightingData& lighting = ptr()->lighting;
    ResFragmentLightingTable table = GetLightingTable();
    bool isValid = true;
    u32 flags = lighting.flags;

    if ((flags & ResFragmentLightingData::FLAG_SAMPLER3) && !HasTable(table, 3)) {
        flags &= ~ResFragmentLightingData::FLAG_SAMPLER3;
        isValid = false;
    }
    if ((flags & ResFragmentLightingData::FLAG_SAMPLER4) && !HasTable(table, 4)) {
        flags &= ~ResFragmentLightingData::FLAG_SAMPLER4;
        isValid = false;
    }
    if (static_cast<u8>(lighting.isSampler5Used) && !HasTable(table, 5)) {
        isValid = false;
        lighting.isSampler5Used = false;
    }
    if (flags & ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) {
        const u8 samplers = s_ConfigSamplers[static_cast<u8>(lighting.config)];
        bool isMissing = false;
        if ((samplers & 1) && !HasTable(table, 0)) {
            isMissing = true;
        }
        if ((samplers & 2) && !HasTable(table, 1)) {
            isMissing = true;
        }
        if (((samplers & 4) && !HasTable(table, 2)) || isMissing) {
            flags &= ~ResFragmentLightingData::FLAG_CONFIG_SAMPLERS;
            isValid = false;
        }
    }

    if (lighting.flags != flags) {
        lighting.flags = flags;
        // the lighting is sent again
        pMaterial->fragmentLightingHash = 0;
    }
    return isValid;
}

// 0x004A7BF4 | nintendogs:bytes [tier B]
u32 nw::gfx::res::ResFragmentShader::Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile, ResMaterialData* pMaterial)
{
    u32 result = RESULT_SUCCESS;
    result |= GetLightingTable().Setup(pAllocator, graphicsFile);
    if (pMaterial->flags & ResMaterialData::FLAG_FRAGMENT_LIGHTING) {
        if (!CheckFragmentShader(pMaterial)) {
            result |= RESULT_LUT_MISSING;
        }
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
