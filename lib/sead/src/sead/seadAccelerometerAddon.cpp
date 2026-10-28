#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerAddon.h"
#include "sead/seadAccelerometerAddon.h"

namespace sead {
// ctor address unknown
sead::AccelerometerAddon::AccelerometerAddon()
{
}

} // namespace sead

// the type information of the classes below ControllerAddon
// 0x00828F6C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControllerAddon>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
