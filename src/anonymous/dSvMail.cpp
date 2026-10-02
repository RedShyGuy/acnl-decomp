// Classes from the anonymous namespace of the original dSvMail.cpp

#include "decomp.h"
#include "script/dPhrase.h"
#include "sead/seadIDelegate1.h"

namespace {

// vtable 0x0088F8A8 (vptr 0x0088F8B0), offset_to_top 0, 2 entries
class SimpleFuncDelegate : public sead::IDelegate1<script::Phrase*>
{
public:
    virtual void vf_0x00() {} // 0x004DC2AC slot 0x00 | virtual slot, introduced by (anonymous namespace)::SimpleFuncDelegate
    virtual void vf_0x04() {} // 0x00828348 slot 0x04 | virtual slot, introduced by (anonymous namespace)::SimpleFuncDelegate
};

} // namespace
