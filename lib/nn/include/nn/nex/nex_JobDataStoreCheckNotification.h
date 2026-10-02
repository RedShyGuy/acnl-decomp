#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29JobDataStoreCheckNotificationE @ 0x008CF0FC
// vtable 0x008FF168 (vptr 0x008FF170), offset_to_top 0, 17 entries
// vtable 0x008FF1B4 (vptr 0x008FF1BC), offset_to_top -104, 6 entries
class JobDataStoreCheckNotification : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    virtual ~JobDataStoreCheckNotification(); // 0x003C038C slot 0x00 | fefates:bytes
    // 0x003C0374 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x34(); // 0x003BF818 slot 0x34 | fefates:callseq
    virtual void vf_0x38(); // 0x003BFAC4 slot 0x38 | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
    virtual void vf_0x3C(); // 0x003BFBFC slot 0x3C | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
    virtual void vf_0x40(); // 0x003BFB4C slot 0x40 | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
    void CompleteJob(const nn::nex::qResult&); // 0x003BF9D0 | fefates:bytes [tier B]
    void StepFileServerCheckNotificationFile(); // 0x003BFD94 | fefates:bytes [tier B]
    void StepWaitingLogicServerGetNotificationUrl(); // 0x003BFECC | fefates:bytes [tier B]
    void StepWaitingFileServerCheckNotificationFile(); // 0x003C006C | fefates:bytes [tier B]
    JobDataStoreCheckNotification(); // 0x003C028C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
