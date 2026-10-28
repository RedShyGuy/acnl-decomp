#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResourceFactory.h"
#include "sead/seadDirectResourceFactoryBase.h"

namespace sead {
// ctor address unknown
sead::DirectResourceFactoryBase::DirectResourceFactoryBase()
{
}

} // namespace sead

// the type information of the classes below ResourceFactory
// 0x00829054 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ResourceFactory>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
