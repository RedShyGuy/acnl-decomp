#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreCheckConsistencyEventListenerAdaptor.h"
#include "nn/nex/nex_DataStoreClientInterface.h"
#include "nn/nex/nex_DataStoreNewArrivedObjects.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStoreLogicServerClientInterface;

// Receives the new DataStore objects of a principal: checks the notification file (CheckNotification
// of the client), gets the new notifications and the meta information of their objects, then each
// object with GetObject of the client into the listener of its type (through a
// DataStoreCheckConsistencyEventListenerAdaptor). The state of the caller is updated on the way. With
// the second Initialize only the notification file is checked. The names of the members and of the
// steps (except StepGetObject) are ours.
// Instantiations found in the binary:
//   nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >  typeinfo 0x008CF390  vtable 0x008FF734
//   nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >  typeinfo 0x008CF3B0  vtable 0x008FF778
template <typename Traits>
class JobDataStoreReceiveNewArrivedObjects : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    typedef typename Traits::TDataId TDataId;
    typedef typename Traits::TPrepareGetParam TPrepareGetParam;
    typedef typename Traits::TNotification TNotification;
    typedef typename Traits::TSpecificMetaInfo TSpecificMetaInfo;
    typedef typename Traits::TGetSpecificMetaParam TGetSpecificMetaParam;
    typedef DataStoreGetObjectEventListenerTemplate<TDataId> Listener;
    typedef DataStoreNewArrivedObjectsEventListenerTemplate<Traits> EventListener;
    typedef DataStoreCheckConsistencyEventListenerAdaptor<Traits> Adaptor;
    typedef bool (DataStoreClientInterface::*GetObjectMethod)(ProtocolCallContext* pContext, const TPrepareGetParam& param, Listener* pListener);

    // GetObject of the client for this version
    static GetObjectMethod s_GetObject;

    JobDataStoreReceiveNewArrivedObjects();
    virtual ~JobDataStoreReceiveNewArrivedObjects();
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreClientInterface* pClient, nn::nex::DataStoreLogicServerClientInterface* pLogicClient,
                                        unsigned int threadPriority, bool usesHttps, unsigned int principalId, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                        nn::nex::qVector<Listener*> listeners, EventListener* pEventListener, bool* pHasNext); // slot 0x34
    // only the check of the notification file
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreClientInterface* pClient, unsigned int threadPriority, unsigned int principalId,
                                        const nn::nex::DataStoreNewArrivedObjectsState* pState, EventListener* pEventListener,
                                        bool* pHasNewObjects); // slot 0x38
    void CompleteJob(const nn::nex::qResult& result);
    void StepCheckNotification();
    void StepWaitingCheckNotification();
    void StepGetNewArrivedNotifications();
    void StepWaitingGetNewArrivedNotifications();
    void StepGetSpecificMeta();
    void StepWaitingGetSpecificMeta();
    void StepGetObject();
    void StepWaitingGetObject();

    u32 m_CallContextId;                                // 0x068
    DataStoreClientInterface* m_pClient;                // 0x06C
    DataStoreLogicServerClientInterface* m_pLogicClient; // 0x070
    bool m_UsesHttps;                                   // 0x074
    u32 m_ThreadPriority;                               // 0x078
    ProtocolCallContext m_ProtocolCallContext;          // 0x080
    u64 m_LastDataId;                                   // 0x0F8 (of CheckNotification)
    String m_Url;                                       // 0x100 (of CheckNotification)
    u32 m_Interval;                                     // 0x108 (of CheckNotification)
    u32 m_PrincipalId;                                  // 0x10C
    qVector<TNotification> m_Notifications;             // 0x110
    DataStoreNewArrivedObjectsState m_State;            // 0x120 (of the caller)
    DataStoreNewArrivedObjectsState m_NewState;         // 0x280
    qVector<Listener*> m_Listeners;                     // 0x3E0 (one per notification)
    EventListener* m_pEventListener;                    // 0x3EC
    u32 m_ListenerIndex;                                // 0x3F0
    u32 m_NotificationIndex;                            // 0x3F4
    bool* m_pHasNext;                                   // 0x3F8 (of the caller)
    qVector<TSpecificMetaInfo> m_MetaInfos;             // 0x3FC
    bool m_IsLogicServerError;                          // 0x408 (of CheckNotification)
    Adaptor* m_pAdaptor;                                // 0x40C
    bool m_ChecksOnly;                                  // 0x410 (only the notification file)
    bool* m_pHasNewObjects;                             // 0x414 (of the caller)
    bool m_IsNewState;                                  // 0x418 (no state of the caller)
};
} // namespace nex
} // namespace nn
