#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_JobDataStoreUpdateObject.h"

namespace nn {
namespace nex {
// 0x003B4378 slot 0x00 | fefates:bytes
nn::nex::JobDataStoreUpdateObject::~JobDataStoreUpdateObject()
{
}

// 0x003B3980 slot 0x34 | virtual slot, introduced by nn::nex::JobDataStoreUpdateObject
void nn::nex::JobDataStoreUpdateObject::vf_0x34()
{
}

// 0x003B3B08 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::CompleteJob(const nn::nex::qResult&)
{
}

// 0x003B3B88 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepFileServerPostObject()
{
}

// 0x003B3EA4 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepLogicServerPrepareUpdateObject()
{
}

// 0x003B3FB8 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepLogicServerCompleteUpdateObject()
{
}

// 0x003B4130 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepWaitingLogicServerCompleteUpdateObject()
{
}

// 0x003B4204 | mk7dlp:callseq [tier A]
nn::nex::JobDataStoreUpdateObject::JobDataStoreUpdateObject()
{
}

} // namespace nex
} // namespace nn
