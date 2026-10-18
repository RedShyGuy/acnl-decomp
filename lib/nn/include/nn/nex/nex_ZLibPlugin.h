#pragma once

#include "decomp.h"
#include "nn/nex/nex_CompressionPlugin.h"

namespace nn {
namespace nex {
// The plugin of the zlib compression. The names of the functions are ours.
// RTTI N2nn3nex10ZLibPluginE @ 0x008CDFE8
// vtable 0x008FC20C (vptr 0x008FC214), offset_to_top 0, 16 entries
class ZLibPlugin : public ::nn::nex::CompressionPlugin
{
public:
    // (inline)
    ZLibPlugin() {}
    virtual ~ZLibPlugin(); // 0x00356978 slot 0x00
    // 0x00356960 slot 0x04 (deleting dtor)
    virtual const wchar_t* GetGUID(); // 0x003568C0 slot 0x08
    virtual const wchar_t* GetDescription(); // 0x003568F4 slot 0x0C
    virtual const wchar_t* GetVendor(); // 0x00356948 slot 0x10
    virtual u32 GetMajorVersion(); // 0x00356878 slot 0x14
    virtual u32 GetMinorVersion(); // 0x00356880 slot 0x18
    virtual bool Load(); // 0x00356940 slot 0x20
    virtual void Unload(); // 0x0035693C slot 0x24
    virtual nn::nex::CompressionAlgorithm* CreateAlgorithm(); // 0x00356888 slot 0x38
    virtual void DeleteAlgorithm(nn::nex::CompressionAlgorithm* pAlgorithm); // 0x003568A8 slot 0x3C
};
} // namespace nex
} // namespace nn
