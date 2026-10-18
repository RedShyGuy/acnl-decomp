#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreGetObjectEventListenerTemplate.h"

namespace nn {
namespace nex {
// The state of the checks for new DataStore objects: the caller keeps it between the calls (it is
// checked with the MD5 checksum of the bytes before m_Checksum). Not in the RTTI of nex; all the
// names are ours.
struct DataStoreNewArrivedObjectsState
{
    u32 m_Unknown0x00;
    u32 m_ErrorCount;         // 0x04 (of the checks of the notification file in a row)
    u32 m_Interval;           // 0x08 (of the notification file)
    u32 m_Unknown0x0C;
    u64 m_LastNotificationId; // 0x10
    char m_Url[320];          // 0x18 (of the notification file)
    u32 m_Checksum;           // 0x158
    u32 m_Unknown0x15C;
};
ASSERT_OFFSET(DataStoreNewArrivedObjectsState, m_LastNotificationId, 0x10);
ASSERT_OFFSET(DataStoreNewArrivedObjectsState, m_Checksum, 0x158);
ASSERT_SIZE(DataStoreNewArrivedObjectsState, 0x160);

// The receiver of the new objects of JobDataStoreReceiveNewArrivedObjects (the application implements
// it; no RTTI in nex, the names are ours).
template <typename Traits>
class DataStoreNewArrivedObjectsEventListenerTemplate
{
public:
    typedef typename Traits::TDataId TDataId;
    typedef typename Traits::TVersion TVersion;

    virtual ~DataStoreNewArrivedObjectsEventListenerTemplate() {}
    // a new object with its meta information (its data follows through the listener of the type)
    virtual void BeginObject(TDataId dataId, unsigned int size, unsigned int ownerId, unsigned short dataType, TVersion version,
                             unsigned long long notificationId) = 0; // slot 0x08
    // the state changed (pListener: the listener that got the object, else null)
    virtual void UpdateState(const nn::nex::DataStoreNewArrivedObjectsState* pState, nn::nex::DataStoreGetObjectEventListenerTemplate<TDataId>* pListener,
                             unsigned long long notificationId) = 0; // slot 0x0C
};
} // namespace nex
} // namespace nn
