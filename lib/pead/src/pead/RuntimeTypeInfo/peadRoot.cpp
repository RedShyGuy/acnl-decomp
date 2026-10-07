#include "pead/RuntimeTypeInfo/peadRoot.h"

namespace pead {
namespace RuntimeTypeInfo {
// 0x00749478
bool pead::RuntimeTypeInfo::Root::isDerived(const Interface* typeInfo) const
{
    return typeInfo == this;
}

} // namespace RuntimeTypeInfo
} // namespace pead
