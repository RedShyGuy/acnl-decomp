#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_JobDataStoreCheckNotification.h"

namespace nn {
namespace nex {
// 0x003C038C slot 0x00 | fefates:bytes
nn::nex::JobDataStoreCheckNotification::~JobDataStoreCheckNotification()
{
}

// 0x003BF818 slot 0x34 | fefates:callseq
void nn::nex::JobDataStoreCheckNotification::vf_0x34()
{
}

// 0x003BFAC4 slot 0x38 | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
void nn::nex::JobDataStoreCheckNotification::vf_0x38()
{
}

// 0x003BFBFC slot 0x3C | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
void nn::nex::JobDataStoreCheckNotification::vf_0x3C()
{
}

// 0x003BFB4C slot 0x40 | virtual slot, introduced by nn::nex::JobDataStoreCheckNotification
void nn::nex::JobDataStoreCheckNotification::vf_0x40()
{
}

// 0x003BF9D0 | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::CompleteJob(const nn::nex::qResult&)
{
}

// 0x003BFD94 | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepFileServerCheckNotificationFile()
{
}

// 0x003BFECC | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepWaitingLogicServerGetNotificationUrl()
{
}

// 0x003C006C | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepWaitingFileServerCheckNotificationFile()
{
}

// 0x003C028C | fefates:bytes [tier B]
nn::nex::JobDataStoreCheckNotification::JobDataStoreCheckNotification()
{
}

} // namespace nex
} // namespace nn
