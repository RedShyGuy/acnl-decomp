// Classes from the anonymous namespace of the original seadTextWriter.cpp

#include "decomp.h"
#include "sead/seadFontBase.h"
#include "sead/seadGraphicsContext.h"

namespace {

// vtable 0x008C3098 (vptr 0x008C30A0), offset_to_top 0, 10 entries
class DummyFont : public sead::FontBase
{
public:
    virtual void vf_0x00() {} // 0x00528F08 slot 0x00 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x04() {} // 0x00528F04 slot 0x04 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x08() {} // 0x00748CFC slot 0x08 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x0C() {} // 0x00748CF0 slot 0x0C | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x10() {} // 0x00748CD0 slot 0x10 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x14() {} // 0x00748CC8 slot 0x14 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x18() {} // 0x00748CDC slot 0x18 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x1C() {} // 0x00748CE8 slot 0x1C | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x20() {} // 0x00748CE4 slot 0x20 | virtual slot, introduced by (anonymous namespace)::DummyFont
    virtual void vf_0x24() {} // 0x00748CEC slot 0x24 | virtual slot, introduced by (anonymous namespace)::DummyFont
};

// vtable 0x008C30C8 (vptr 0x008C30D0), offset_to_top 0, 2 entries
class GraphicsContextForTextWriter : public sead::GraphicsContext
{
public:
    virtual void vf_0x00() {} // 0x00528EF4 slot 0x00 | virtual slot, introduced by sead::GraphicsContext
    virtual void vf_0x04() {} // 0x00528EE4 slot 0x04 | virtual slot, introduced by sead::GraphicsContext
};

} // namespace
