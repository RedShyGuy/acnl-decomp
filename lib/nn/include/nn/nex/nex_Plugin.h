#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A plugin (the compression plugin of zlib in this build): its identification and hooks. The
// names of the functions are ours; the strings are from the binary.
// RTTI N2nn3nex6PluginE @ 0x008CF5AC
class Plugin : public ::nn::nex::RootObject
{
public:
    // the types of the plugins
    static const u32 TYPE_COMPRESSION = 2;

    // (inline)
    Plugin() {}
    virtual ~Plugin() {}
    virtual const wchar_t* GetGUID() = 0;          // slot 0x08
    virtual const wchar_t* GetDescription() = 0;   // slot 0x0C
    virtual const wchar_t* GetVendor() = 0;        // slot 0x10
    virtual u32 GetMajorVersion() = 0;          // slot 0x14
    virtual u32 GetMinorVersion() = 0;          // slot 0x18
    virtual u32 GetType() = 0;                  // slot 0x1C
    virtual bool Load() = 0;                    // slot 0x20
    virtual void Unload() = 0;                  // slot 0x24
    virtual bool IsCompatible(); // 0x003D05E8 slot 0x28
    virtual void Hook0x2C(); // 0x003D05F0 slot 0x2C
    virtual void Hook0x30(); // 0x003D05F4 slot 0x30
    virtual void Hook0x34(); // 0x003D05F8 slot 0x34
};
} // namespace nex
} // namespace nn
