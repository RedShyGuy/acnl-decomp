// Classes from the anonymous namespace of the original dBsShowMgr.cpp

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace {

// vtable 0x0085527C (vptr 0x00855284), offset_to_top 0, 4 entries
class InsectFdParamAccess : public sead::hostio::Node
{
public:
    virtual void vf_0x00() {} // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04() {} // 0x004DD554 slot 0x04 | virtual slot, introduced by (anonymous namespace)::InsectFdParamAccess
    virtual void vf_0x08() {} // 0x004DD4B0 slot 0x08 | virtual slot, introduced by (anonymous namespace)::InsectFdParamAccess
    virtual void vf_0x0C() {} // 0x0057EB0C slot 0x0C | virtual slot, introduced by (anonymous namespace)::InsectFdParamAccess
};

// vtable 0x00855268 (vptr 0x00855270), offset_to_top 0, 3 entries
class ShowParamAccess : public sead::hostio::Node
{
public:
    virtual void vf_0x00() {} // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04() {} // 0x004DD484 slot 0x04 | virtual slot, introduced by (anonymous namespace)::ShowParamAccess
    virtual void vf_0x08() {} // 0x004DD458 slot 0x08 | virtual slot, introduced by (anonymous namespace)::ShowParamAccess
};

} // namespace
