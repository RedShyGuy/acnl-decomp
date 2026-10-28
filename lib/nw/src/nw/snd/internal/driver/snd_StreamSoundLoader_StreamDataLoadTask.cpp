#include "nw/snd/internal/driver/snd_StreamSoundLoader_StreamDataLoadTask.h"

#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CF67C slot 0x00
// 0x004CF66C (deleting dtor)
nw::snd::internal::driver::StreamSoundLoader::StreamDataLoadTask::~StreamDataLoadTask()
{
    // empty (in the original too)
}

// 0x004CF548 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::StreamSoundLoader::StreamDataLoadTask::Execute()
{
    m_pLoader->m_IsLoopFromStart = m_IsLoopFromStart;
    StreamSoundLoader* pLoader = m_pLoader;
    if (pLoader->m_IsLoadEnd) {
        return;
    }
    DriverCommandManager& manager = DriverCommandManager::GetInstanceForTaskThread();
    DriverCommandStreamSoundLoadData* pCommand = manager.AllocCommand<DriverCommandStreamSoundLoadData>();
    pCommand->id = DriverCommand::ID_STRMSOUND_LOAD_DATA;
    bool result;
    switch (pLoader->m_FileType) {
    case FILE_TYPE_BCSTM:
        result = pLoader->LoadData1(pCommand, m_Buffers, m_BlockIndex, m_StartOffset, m_PrefetchOffset);
        break;
    case FILE_TYPE_EXTERNAL:
        result = pLoader->LoadData2(pCommand, m_Buffers, m_BlockIndex, m_StartOffset, m_PrefetchOffset);
        break;
    default:
        result = false;
        break;
    }
    pCommand->result = result;
    pCommand->player = pLoader->m_pPlayer;
    manager.PushCommand(pCommand);
    manager.FlushCommand(true);
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
