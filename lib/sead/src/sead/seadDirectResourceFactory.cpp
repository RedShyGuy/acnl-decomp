// The instances of DirectResourceFactory. The complete destructor of <DirectResource> is a nop that
// falls into ~ResourceFactory, the one of <GraphicsFileResNw4c> branches to it.
#include "sead/seadDirectResourceFactory.h"
#include "sead/seadDirectResource.h"
#include "sead/seadGraphicsFileResNw4c.h"

namespace sead {
// 0x0054658C slot 0x00
// 0x00812B14 (deleting dtor)
template sead::DirectResourceFactory<sead::DirectResource>::~DirectResourceFactory();
// 0x008299AC slot 0x08 (name is ours)
template bool sead::DirectResourceFactory<sead::DirectResource>::isKindOf(const sead::RuntimeTypeInfo::Interface*) const;
// 0x00829960 slot 0x0C (name is ours)
template const sead::RuntimeTypeInfo::Interface* sead::DirectResourceFactory<sead::DirectResource>::getTypeInfo() const;
// 0x00544894 slot 0x1C (name after the reference symbols)
template sead::DirectResource* sead::DirectResourceFactory<sead::DirectResource>::newResource_(sead::Heap*, s32);

// 0x00812B34 slot 0x00
// 0x00812B24 (deleting dtor)
template sead::DirectResourceFactory<sead::GraphicsFileResNw4c>::~DirectResourceFactory();
// 0x00829AFC slot 0x08 (name is ours)
template bool sead::DirectResourceFactory<sead::GraphicsFileResNw4c>::isKindOf(const sead::RuntimeTypeInfo::Interface*) const;
// 0x00829AB0 slot 0x0C (name is ours)
template const sead::RuntimeTypeInfo::Interface* sead::DirectResourceFactory<sead::GraphicsFileResNw4c>::getTypeInfo() const;
// 0x0054A16C slot 0x1C (name after the reference symbols)
template sead::DirectResource* sead::DirectResourceFactory<sead::GraphicsFileResNw4c>::newResource_(sead::Heap*, s32);
} // namespace sead
