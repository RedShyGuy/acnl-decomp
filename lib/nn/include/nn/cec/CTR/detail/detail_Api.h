#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace cec {
namespace CTR {
namespace detail {
// The commands of cecd (3dbrew "CECD Services") on the session of cecd:u (CecdClient) or on the
// second session (CecdClient2, never opened by this program); both classes have their own copies
// of the same code. The class names are from the symbols, the method names from the symbols or
// after 3dbrew, the parameter names are ours.
class CecdClient
{
public:
    static nn::Result Open(u32 programId, u32 path, u32 flags, u32* pSize); // 0x0034FB9C | tier B
    static nn::Result Read(u32* pReadSize, void* pBuffer, u32 size); // 0x0034FBEC (name after the command)
    static nn::Result ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size); // 0x0034F858 | tier B
    static nn::Result ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey); // 0x0034FA4C | tier B
    static nn::Result Write(const void* pBuffer, u32 size); // 0x0034FC74 | tier B
    static nn::Result WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size); // 0x0034F914 (name after the command)
    static nn::Result WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey); // 0x0034FAE0 (name after the command)
    static nn::Result Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize); // 0x0034FCBC | tier B
    static nn::Result SetData(u32 programId, const u8* pData, u32 size, u32 option); // 0x0034FD18 | tier B
    static nn::Result ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize); // 0x0034FD60 | tier B
    static nn::Result Start(u32 command); // 0x00144B80 | tier B
    static nn::Result Stop(u32 command); // 0x0034FC3C | tier B
    static nn::Result GetCecdState(u32* pState); // 0x0034F8D8 (name after the command)
    static nn::Result GetChangeStateEventHandle(nn::Handle* pEvent); // 0x0034FB60 (name after the command)
    static nn::Result OpenAndWrite(const void* pBuffer, u32 size, u32 programId, u32 path, u32 flags); // 0x0034F9F4 | tier B
    static nn::Result OpenAndRead(void* pBuffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags); // 0x0034F984 | tier B
};

class CecdClient2
{
public:
    static nn::Result Open(u32 programId, u32 path, u32 flags, u32* pSize); // 0x003501F0 | tier B
    static nn::Result Read(u32* pReadSize, void* pBuffer, u32 size); // 0x00350240 (name after the command)
    static nn::Result ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size); // 0x0034FEAC | tier B
    static nn::Result ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey); // 0x003500A0 | tier B
    static nn::Result Write(const void* pBuffer, u32 size); // 0x003502C8 | tier B
    static nn::Result WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size); // 0x0034FF68 (name after the command)
    static nn::Result WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey); // 0x00350134 (name after the command)
    static nn::Result Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize); // 0x00350310 | tier B
    static nn::Result SetData(u32 programId, const u8* pData, u32 size, u32 option); // 0x0035036C | tier B
    static nn::Result ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize); // 0x003503B4 | tier B
    static nn::Result Start(u32 command); // 0x00144BB8 (name after the copy in CecdClient)
    static nn::Result Stop(u32 command); // 0x00350290 | tier B
    static nn::Result GetCecdState(u32* pState); // 0x0034FF2C (name after the command)
    static nn::Result GetChangeStateEventHandle(nn::Handle* pEvent); // 0x003501B4 (name after the command)
    static nn::Result OpenAndWrite(const void* pBuffer, u32 size, u32 programId, u32 path, u32 flags); // 0x00350048 | tier B
    static nn::Result OpenAndRead(void* pBuffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags); // 0x0034FFD8 | tier B
};

// on the second session when it is open, else on cecd:u (a failure when neither is open)
nn::Result Start(unsigned command); // 0x00143694 | nintendogs:callgraph [tier A]
nn::Result GetCecdState(unsigned int* pState); // 0x0034F54C | tier C
nn::Result ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size); // 0x0034F4E4 (name after the command)
nn::Result WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size); // 0x0034F588 (name after the command)
nn::Result OpenAndReadFile(unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned programId, unsigned path, unsigned flags); // 0x0034F5F0 | nintendogs:callgraph [tier A]
nn::Result OpenAndWriteFile(const unsigned char* pBuffer, unsigned size, unsigned programId, unsigned path, unsigned flags); // 0x0034F658 | nintendogs:callgraph [tier A]
nn::Result FinalizeCecControl(); // 0x0034F6A4 | nintendogs:bytes [tier A]
nn::Result ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey); // 0x0034F6C8 (name after the command)
// waits until cecd:u is open (polling every 10 ms)
nn::Result WaitForSessionValid(); // 0x0034F73C | nintendogs:bytes [tier A]
nn::Result InitializeCecControl(); // 0x0034F778 | nintendogs:bytes [tier A]
nn::Result WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey); // 0x0034F7B4 (name after the command)
nn::Result GetChangeStateEventHandle(nn::Handle* pEvent); // 0x0034F81C | tier C
nn::Result Open(unsigned programId, unsigned path, unsigned flags, unsigned* pSize); // 0x0034FDBC | nintendogs:callgraph [tier A]
// (symbols.json names 0x0034FDF8 GetCecInfoBuffer; it sends the command Read)
nn::Result Read(u32* pReadSize, void* pBuffer, u32 size); // 0x0034FDF8 (name after the command)
nn::Result Stop(unsigned int command); // 0x0034FE34 | tier C
nn::Result Write(const void* pBuffer, u32 size); // 0x0034FE70 (name after the command)
nn::Result Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize); // 0x00350410 (name after the command)
nn::Result SetData(unsigned programId, const unsigned char* pData, unsigned size, unsigned option); // 0x0035045C | nintendogs:callgraph [tier A]
nn::Result ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize); // 0x00350498 (name after the command)

// the sessions (cecd:u and the second one)
extern nn::Handle s_CecdSession;
extern nn::Handle s_CecdSession2;
} // namespace detail
} // namespace CTR
} // namespace cec
} // namespace nn
