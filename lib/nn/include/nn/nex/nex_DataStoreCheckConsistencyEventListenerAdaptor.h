#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreGetObjectEventListenerTemplate.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// Passes the data of an object to the listener of the caller, but only if its meta information is
// the one of the new object (size, data type and version); else the call is cancelled. The names of
// the members are ours.
// Instantiations found in the binary:
//   nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >  typeinfo 0x008CF4AC  vtable 0x008FF958
//   nn::nex::DataStoreCheckConsistencyEventListenerAdaptor<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >  typeinfo 0x008CF4D4  vtable 0x008FF974
template <typename Traits>
class DataStoreCheckConsistencyEventListenerAdaptor : public ::nn::nex::DataStoreGetObjectEventListenerTemplate<typename Traits::TDataId>,
                                                      public ::nn::nex::RootObject,
                                                      public ::nn::nex::NonCopyable
{
public:
    typedef typename Traits::TDataId TDataId;
    typedef typename Traits::TSpecificMetaInfo TSpecificMetaInfo;
    typedef DataStoreGetObjectEventListenerTemplate<TDataId> Listener;

    // (inline)
    DataStoreCheckConsistencyEventListenerAdaptor(Listener* pListener, CallContext* pContext, const TSpecificMetaInfo& metaInfo)
        : m_pListener(pListener), m_pContext(pContext), m_MetaInfo(metaInfo)
    {
    }
    virtual ~DataStoreCheckConsistencyEventListenerAdaptor();
    virtual void BeginInternal(TDataId dataId, unsigned int size, unsigned int ownerId, unsigned short dataType, unsigned short version, bool isFromFileServer);
    virtual void Begin(TDataId dataId, unsigned int size);
    virtual bool ProcessData(const unsigned char* pData, unsigned int size);

    Listener* m_pListener;          // 0x04 (of the caller)
    CallContext* m_pContext;        // 0x08 (cancelled on other meta information)
    TSpecificMetaInfo m_MetaInfo;   // 0x0C / 0x10 (of the new object)
};
} // namespace nex
} // namespace nn
