#pragma once

#include "decomp.h"
#include "nn/pia/pia_Types.h"

namespace net {
class ScanBufBase;
class SearchDataResultList;
class TagWriter;

namespace nex {
// A request of the game to the network thread: the Framework fills it, the System works on it in its
// state (names after libgarden: Command, ID, Status, NONE, CONNECT_LOCAL, CONNECT_NEX, STUB_*, param,
// processState, status; the other names are ours)
struct Command
{
    enum ID : u8
    {
        NONE = 0,
        CONNECT_LOCAL = 1,
        CONNECT_NEX = 2,
        CREATE_SESSION = 3,
        SEARCH_SESSIONS = 4,
        JOIN_SESSION = 5,
        JOIN_RANDOM_SESSION = 6,
        CREATE_RANDOM_SESSION = 7,
        JOIN_SESSION_BY_OWNER = 8,
        CLOSE_SESSION = 9,
        OPEN_SESSION = 10,
        MODIFY_SESSION_ATTRIBUTE = 11,
        LEAVE_SESSION = 12,
        CMD_13 = 13,
        LEAVE_SESSION_14 = 14,
        DISCONNECT = 15,
        SET_SESSION_DATA = 16,
        GET_FRIEND_CODE = 17,
        SEND_NOTIFICATION_102 = 18,
        SEND_TEXT_MESSAGE = 19,
        SEND_NOTIFICATION_103 = 20,
        MATCHMAKE_METHOD_25 = 21,
        MATCHMAKE_METHOD_26 = 22,
        CONNECT_DATASTORE = 23,
        DISCONNECT_DATASTORE = 24,
        UPLOAD_DATA = 25,
        UPLOAD_META_BINARY = 26,
        SEARCH_DATA = 27,
        DOWNLOAD_DATA = 28,
        RESET_PERMISSION = 29,
        RATE_OBJECT = 30,
        UPLOAD_DATA_2 = 31,
        CONNECT_INDEPENDENT = 32,
        DISCONNECT_INDEPENDENT = 33,
        REQUEST_SERVICE_TOKEN = 34,
        RESET_SYNC_CLOCK = 35,
        DELETE_OWN_DATA = 36,
        STUB_0 = 37,
        STUB_1 = 38,
        STUB_2 = 39,
        STUB_3 = 40,
        STUB_4 = 41,
        STUB_5 = 42,
    };

    enum Status : u8
    {
        NOT_DONE = 0,
        DONE = 1,
        FAILED = 2,
    };

    // the parameters of the commands
    struct CreateSessionParam
    {
        const char16* text; // 0x00 (local: the application data, nex: the text of the notification)
        u32 length;         // 0x04
        u16 param;          // 0x08
        u8 mode;            // 0x0A (local)
    };
    struct SearchSessionsParam
    {
        ScanBufBase** results; // 0x00
        u32 resultNum;         // 0x04
        u32* foundNum;         // 0x08
        u8 mode;               // 0x0C
        s8 withData;           // 0x0D
    };
    struct OpenSessionParam
    {
        u16 param;   // 0x00
        s8 setParam; // 0x02
    };
    struct SessionDataParam
    {
        const char16* data; // 0x00
        u32 length;         // 0x04
    };
    struct FriendCodeParam
    {
        nn::pia::StationId stationId; // 0x00
        u64* friendCode;              // 0x08
    };
    struct TextParam
    {
        u32 param1;          // 0x00 (a principal id for the messages)
        const wchar_t* text; // 0x04
    };
    struct Notification103Param
    {
        u32 param1;          // 0x00
        u32 param2;          // 0x04
        const wchar_t* text; // 0x08
    };
    struct UploadParam
    {
        const void* data;   // 0x00
        u32 size;           // 0x04
        void* work;         // 0x08 (the encrypted data, size + 32 bytes)
        u64* dataId;        // 0x0C
        const void* info;   // 0x10 (Impl::UploadInfo)
    };
    struct SearchDataParam
    {
        u64 dataId;                      // 0x00 (0: a search)
        SearchDataResultList* results;   // 0x08
        void (*tagFunction)(TagWriter*); // 0x0C
        u8 order;                        // 0x10
        u16 dataTypes[8];                // 0x12 (unused ones 0xFFFF)
    };
    struct DownloadParam
    {
        u64 dataId;    // 0x00
        void* buffer;  // 0x08 (the decrypted data)
        u32 size;      // 0x0C
        void* work;    // 0x10 (the encrypted data, size + 32 bytes)
        u32* length;   // 0x14
    };
    struct PermissionParam
    {
        u64 dataId;                      // 0x00
        void (*tagFunction)(TagWriter*); // 0x08
        u16 dataType;                    // 0x0C
    };
    struct RateParam
    {
        u64 dataId;  // 0x00
        s8 rating0;  // 0x08
        s8 rating1;  // 0x09
    };

    union Param
    {
        // (cleared)
        Param() : words() {}

        u32 words[10];
        u32 value;
        CreateSessionParam createSession;
        SearchSessionsParam searchSessions;
        OpenSessionParam openSession;
        SessionDataParam sessionData;
        FriendCodeParam friendCode;
        TextParam text;
        Notification103Param notification103;
        UploadParam upload;
        SearchDataParam searchData;
        DownloadParam download;
        PermissionParam permission;
        RateParam rate;
    };

    Command() : id(NONE), param(), processState(0), status(NOT_DONE) {}

    void SetDone() { status = DONE; }
    void SetFailed() { status = FAILED; }

    ID id;           // 0x00
    u32 unk_0x4;
    Param param;     // 0x08
    u8 processState; // 0x30
    Status status;   // 0x31
};
ASSERT_SIZE(Command, 0x38);
} // namespace nex
} // namespace net
