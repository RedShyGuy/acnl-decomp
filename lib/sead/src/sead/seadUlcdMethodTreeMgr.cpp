#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"
#include "sead/seadUlcdMethodTreeMgr.h"

namespace sead {
// 0x005492A0 slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
sead::UlcdMethodTreeMgr::~UlcdMethodTreeMgr()
{
}

// 0x0054CB44 slot 0x10 | nintendogs:callseq
void sead::UlcdMethodTreeMgr::attachMethod(int, sead::MethodTreeNode*)
{
}

// 0x0054CD3C slot 0x14 | virtual slot, introduced by sead::MethodTreeMgr
void sead::UlcdMethodTreeMgr::vf_0x14()
{
}

// 0x00548F8C slot 0x18 | nintendogs:bytes
void sead::UlcdMethodTreeMgr::pauseAll(bool)
{
}

// 0x00549018 | nintendogs:bytes [tier A]
sead::UlcdMethodTreeMgr::UlcdMethodTreeMgr()
{
}

} // namespace sead

// the type information of the classes below DualScreenMethodTreeMgr
// 0x008293F0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DualScreenMethodTreeMgr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
