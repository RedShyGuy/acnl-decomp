#include "sead/seadDirectResourceFactoryBase.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDecompressor.h"
#include "sead/seadDirectResource.h"
#include "sead/seadFileDevice.h"
#include "sead/seadFileDeviceMgr.h"

namespace sead {
// 0x0054D7E8 slot 0x10 (name is ours)
sead::Resource* sead::DirectResourceFactoryBase::tryLoad(const ResourceMgr::LoadArg& arg)
{
    DirectResource* resource = newResource_(arg.instanceHeap, arg.instanceAlignment);
    if (resource == nullptr) {
        return nullptr;
    }
    FileDevice::LoadArg loadArg;
    loadArg.path = arg.path;
    loadArg.buffer = arg.loadDataBuffer;
    loadArg.bufferSize = arg.loadDataBufferSize;
    loadArg.heap = arg.loadDataHeap;
    loadArg.divSize = arg.divSize;
    loadArg.field_0x1C = arg.field_0x2C;
    if (arg.loadDataAlignment != 0) {
        loadArg.bufferSizeAlignment = arg.loadDataAlignment;
    } else {
        s32 sign = arg.instanceAlignment >= 0 ? 1 : -1;
        loadArg.bufferSizeAlignment = sign * resource->getLoadDataAlignment();
    }
    u8* data;
    if (arg.device != nullptr) {
        data = arg.device->tryLoad(loadArg);
    } else {
        data = FileDeviceMgr::s_pInstance->tryLoad(loadArg);
    }
    if (data == nullptr) {
        delete resource;
        return nullptr;
    }
    resource->create(data, loadArg.readSize, loadArg.roundupBufferSize, loadArg.needUnload, arg.instanceHeap);
    return resource;
}

// 0x0054D640 slot 0x14 (name is ours)
sead::Resource* sead::DirectResourceFactoryBase::tryLoadWithDecomp(const ResourceMgr::LoadArg& arg,
                                                                   Decompressor* decompressor)
{
    DirectResource* resource = newResource_(arg.instanceHeap, arg.instanceAlignment);
    if (resource == nullptr) {
        return nullptr;
    }
    u32 size = 0;
    u32 allocSize = 0;
    bool needDelete = false;
    u8* data = decompressor->tryDecompFromDevice(arg, resource, &size, &allocSize, &needDelete);
    if (data == nullptr) {
        delete resource;
        return nullptr;
    }
    resource->create(data, size, allocSize, needDelete, arg.instanceHeap);
    return resource;
}

// 0x0054D728 slot 0x18 (name is ours)
sead::Resource* sead::DirectResourceFactoryBase::tryCreate(const ResourceMgr::CreateArg& arg)
{
    DirectResource* resource = newResource_(arg.heap, arg.alignment);
    if (resource == nullptr) {
        return nullptr;
    }
    // the file has to be aligned as the resource wants it
    if (reinterpret_cast<uptr>(arg.buffer) % resource->getLoadDataAlignment() != 0) {
        delete resource;
        return nullptr;
    }
    resource->create(arg.buffer, arg.fileSize, arg.bufferSize, arg.needUnload, arg.heap);
    return resource;
}
} // namespace sead

// the type information of the classes below ResourceFactory
// 0x00829054 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ResourceFactory>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
// 0x008294D8 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DirectResourceFactoryBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
