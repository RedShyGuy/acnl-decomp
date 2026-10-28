#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAccelerometerAddon.h"
#include "sead/seadCtrAccelerometerAddon.h"

namespace sead {
// ctor candidate(s) 0x0054B400 (unverified)
sead::CtrAccelerometerAddon::CtrAccelerometerAddon()
{
}

// 0x0054B450 slot 0x08 | virtual slot, introduced by sead::CtrAccelerometerAddon
void sead::CtrAccelerometerAddon::vf_0x08()
{
}

// 0x0054B44C slot 0x0C | virtual slot, introduced by sead::CtrAccelerometerAddon
void sead::CtrAccelerometerAddon::vf_0x0C()
{
}

// 0x0054B31C slot 0x10 | virtual slot, introduced by sead::CtrAccelerometerAddon
void sead::CtrAccelerometerAddon::vf_0x10()
{
}

// 0x0054B400 | nintendogs:bytes [tier A]
sead::CtrAccelerometerAddon::CtrAccelerometerAddon(sead::Controller*)
{
}

} // namespace sead

// the type information of the classes below AccelerometerAddon
// 0x0082913C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AccelerometerAddon>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
