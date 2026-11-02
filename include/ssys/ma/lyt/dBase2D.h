#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;

// something drawn by the LayoutMgr: a node of its draw lists, sorted by the layer (no vtable of its own in the binary:
// only built as a base; names after libgarden except Build, which libgarden calls BuildImpl)
// RTTI N4ssys2ma3lyt6Base2DE @ 0x008D2490
class Base2D : public ::ssys::st::ListNode
{
public:
    // (inline)
    Base2D() : m_Layer(0x80) {}

    virtual void DrawCached() = 0;                                                     // slot 0x08
    virtual void Animate() = 0;                                                        // slot 0x0C
    virtual bool Build(const char* name, const ArcResAccReader* arcReader, u32 cmdCacheSize) = 0; // slot 0x10
    virtual void DrawSave() = 0;                                                       // slot 0x14
    virtual void UpdateProj() = 0;                                                     // slot 0x18

    void SetLayer(u8 layer) { m_Layer = layer; }
    u8 GetLayer() const { return m_Layer; }

    u8 m_Layer; // 0xC the lists draw smaller layers first
};
ASSERT_SIZE(Base2D, 0x10);
} // namespace lyt
} // namespace ma
} // namespace ssys
