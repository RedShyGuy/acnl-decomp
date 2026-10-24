#pragma once

#include "decomp.h"

namespace nw {
namespace io {
// The runtime type of a stream: GetRuntimeTypeInfo returns the address of a static object per class
// that points to the one of the base class (like nw::gfx; the name is ours).
struct TypeInfo
{
    // (inline)
    explicit TypeInfo(const TypeInfo* parent) : pParent(parent) {}

    const TypeInfo* pParent; // 0x0
};

// The base of the streams (the class name and Read/Close are from the reference symbols). The
// queries without a name are only known by their default value; the snd streams forward them.
// RTTI N2nw2io8IOStreamE @ 0x008D0520
class IOStream
{
public:
    // (inline)
    IOStream() : m_IsAvailable(false), m_Unknown12(0), m_Unknown16(0) {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // slot 0x00 (name after nw::gfx)
    // (inline)
    virtual ~IOStream() {} // slot 0x04
    // whether the stream can be used (name is ours)
    virtual bool IsAvailable() const = 0; // slot 0x0C
    virtual bool vf_0x10() const = 0; // slot 0x10
    virtual bool vf_0x14() const = 0; // slot 0x14
    virtual bool vf_0x18() const; // 0x007375BC slot 0x18
    virtual bool vf_0x1C() const; // 0x007375AC slot 0x1C
    virtual bool vf_0x20() const; // 0x007375B4 slot 0x20
    // the bytes read (negative: an error)
    virtual s32 Read(void* pBuffer, u32 size) = 0; // slot 0x24
    virtual bool vf_0x28() const; // 0x0048B8D4 slot 0x28
    virtual bool vf_0x2C() const; // 0x0048B8CC slot 0x2C
    virtual bool vf_0x30() const; // 0x0048B8C4 slot 0x30
    virtual bool vf_0x34() const; // 0x007375C4 slot 0x34
    virtual void Close() = 0; // slot 0x38

    static const TypeInfo s_TypeInfo;

    bool m_IsAvailable;   // 0x04 (the cached stream sets it; name is ours)
    u32 m_Unknown8;       // 0x08 (not set by the constructors)
    u32 m_Unknown12;      // 0x0C
    u32 m_Unknown16;      // 0x10
};
ASSERT_SIZE(IOStream, 0x14);
} // namespace io
} // namespace nw
