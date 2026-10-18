#include "nn/nex/nex_DataStoreCheckConsistencyEventListenerAdaptor.h"

namespace nn {
namespace nex {
template <typename Traits>
nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<Traits>::~DataStoreCheckConsistencyEventListenerAdaptor()
{
    // nothing to do
}

template <typename Traits>
void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<Traits>::BeginInternal(TDataId dataId, unsigned int size, unsigned int ownerId, unsigned short dataType,
                                                                                   unsigned short version, bool isFromFileServer)
{
    if (size == m_MetaInfo.m_Size && dataType == m_MetaInfo.m_DataType && version == m_MetaInfo.m_Version) {
        m_pListener->BeginInternal(dataId, size, ownerId, dataType, version, isFromFileServer);
    } else {
        // the object changed since its meta information
        m_pContext->Cancel(CallContext::STATE_CANCELLED);
    }
}

template <typename Traits>
void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<Traits>::Begin(TDataId dataId, unsigned int size)
{
    m_pListener->Begin(dataId, size);
}

template <typename Traits>
bool nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<Traits>::ProcessData(const unsigned char* pData, unsigned int size)
{
    return m_pListener->ProcessData(pData, size);
}

// 0x003CD6D8 slot 0x00
// 0x003CD6C8 (deleting dtor)
template nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<
    nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::~DataStoreCheckConsistencyEventListenerAdaptor();
// 0x007E4004 slot 0x08 | fefates:bytes [tier B]
template void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::BeginInternal(
    unsigned int, unsigned int, unsigned int, unsigned short, unsigned short, bool);
// 0x007E407C slot 0x0C
template void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::Begin(unsigned int,
                                                                                                                                                    unsigned int);
// 0x007E406C slot 0x10
template bool nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::ProcessData(
    const unsigned char*, unsigned int);

// 0x003CD6F8 slot 0x00
// 0x003CD6E8 (deleting dtor)
template nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<
    nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::~DataStoreCheckConsistencyEventListenerAdaptor();
// 0x007E408C slot 0x08 | fefates:bytes [tier B]
template void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::BeginInternal(
    unsigned long long, unsigned int, unsigned int, unsigned short, unsigned short, bool);
// 0x007E4114 slot 0x0C | fefates:bytes [tier B]
template void nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::Begin(unsigned long long,
                                                                                                                                                    unsigned int);
// 0x007E4104 slot 0x10
template bool nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::ProcessData(
    const unsigned char*, unsigned int);

} // namespace nex
} // namespace nn
