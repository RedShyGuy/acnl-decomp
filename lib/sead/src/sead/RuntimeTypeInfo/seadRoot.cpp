#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
namespace RuntimeTypeInfo {
// 0x0074C770
bool sead::RuntimeTypeInfo::Root::isDerived(const Interface* typeInfo) const
{
    return typeInfo == this;
}

} // namespace RuntimeTypeInfo
} // namespace sead
