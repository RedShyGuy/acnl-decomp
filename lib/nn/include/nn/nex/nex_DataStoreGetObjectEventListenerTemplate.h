#pragma once

#include "decomp.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The receiver of the data of a DataStore object (from the file server or the meta binary). Only
// the RTTI is in the binary (no vtable of its own); the names of BeginInternal and Begin are from the
// reference symbols of the adaptor, the name of ProcessData is ours.
// Instantiations found in the binary:
//   nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>  typeinfo 0x008CF43C
//   nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>  typeinfo 0x008CF444
template <typename TDataId>
class DataStoreGetObjectEventListenerTemplate
{
public:
    virtual ~DataStoreGetObjectEventListenerTemplate() {}
    // the start of the data with its meta information (from the file server, or the meta binary)
    virtual void BeginInternal(TDataId dataId, unsigned int size, unsigned int ownerId, unsigned short dataType, unsigned short version,
                               bool isFromFileServer) = 0; // slot 0x08
    // the start of the data
    virtual void Begin(TDataId dataId, unsigned int size) = 0; // slot 0x0C
    // the next part of the data; false: the job waits and tries again
    virtual bool ProcessData(const unsigned char* pData, unsigned int size) = 0; // slot 0x10
};

// (out of line in nex_JobDataStoreReceiveNewArrivedObjects.cpp)
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>::qVector(const qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>& other);
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>::qVector(
    const qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>& other);
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>::~qVector();
} // namespace nex
} // namespace nn

namespace std {
// (inline in JobDataStoreReceiveNewArrivedObjects::Initialize; no out-of-line instance)
template <>
inline vector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*, nn::nex::MemAllocator<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*> >&
vector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*, nn::nex::MemAllocator<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*> >::operator=(
    const vector& other)
{
    return AssignInline(other);
}
template <>
inline vector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*,
              nn::nex::MemAllocator<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*> >&
vector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*,
       nn::nex::MemAllocator<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*> >::operator=(const vector& other)
{
    return AssignInline(other);
}
} // namespace std
