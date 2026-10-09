#pragma once

// Types of the boss library (SpotPass). The type names are from the binary (mangled signatures of
// nn::boss); like all ARMCC enums they are as small as their values allow. The property ids and
// the layouts of the configurations are after 3dbrew "BOSS Services" and the code (Send*/Verify*
// in boss_Api.cpp); the member names are ours.

#include "decomp.h"
#include "nn/Handle.h"

namespace nn {
namespace boss {

// the properties of a task that are sent to / read from the service (3dbrew PropertyID)
enum PropertyType : u16 {
    PROPERTY_PRIORITY = 0x00,
    PROPERTY_SCHEDULING_POLICY = 0x01,
    PROPERTY_TARGET_DURATION = 0x02,
    PROPERTY_INTERVAL = 0x03,
    PROPERTY_COUNT = 0x04,
    PROPERTY_PERMISSION = 0x05,
    PROPERTY_ACTION_CODE = 0x06,
    PROPERTY_URL = 0x07,
    PROPERTY_08 = 0x08,
    PROPERTY_ACTION_DATA_TYPE = 0x09,
    PROPERTY_ACTION_DATA_0A = 0x0A,
    PROPERTY_ACTION_DATA_0B = 0x0B,
    PROPERTY_FILE_HANDLE = 0x0C,
    PROPERTY_HEADER_FIELDS = 0x0D,
    PROPERTY_CLIENT_CERTS = 0x0E,
    PROPERTY_ROOT_CAS = 0x0F,
    PROPERTY_FS_CLIENT_CERT = 0x10,
    PROPERTY_FS_ROOT_CA = 0x11,
    PROPERTY_AP_INFO_TYPE = 0x12,
    PROPERTY_ROOT_CA_COUNT = 0x13,
    PROPERTY_CLIENT_CERT_COUNT = 0x14,
    PROPERTY_15 = 0x15,
    PROPERTY_16 = 0x16,
    PROPERTY_RESERVED = 0x17,
    PROPERTY_18 = 0x18,
    PROPERTY_19 = 0x19,
    PROPERTY_1A = 0x1A,
    PROPERTY_1B = 0x1B,
    PROPERTY_1C = 0x1C,
    PROPERTY_TASK_STATE_CODE = 0x1D,
    PROPERTY_1E = 0x1E,
    PROPERTY_1F = 0x1F,
    PROPERTY_TASK_RESULT_CODE = 0x20,
    PROPERTY_TASK_SERVICE_STATUS = 0x21,
    PROPERTY_22 = 0x22,
    PROPERTY_COMM_ERROR_CODE = 0x23,
    PROPERTY_24 = 0x24,
    PROPERTY_25 = 0x25,
    PROPERTY_26 = 0x26,
    PROPERTY_27 = 0x27,
    PROPERTY_LAST_SUCCESSFUL_TIMESTAMP = 0x28,
    PROPERTY_29 = 0x29,
    PROPERTY_2A = 0x2A,
    PROPERTY_2B = 0x2B,
    PROPERTY_2C = 0x2C,
    PROPERTY_2D = 0x2D,
    PROPERTY_2E = 0x2E,
    PROPERTY_LAST_MODIFIED_HEADER = 0x2F,
    PROPERTY_3B = 0x3B,
    PROPERTY_DATA_STORE_DOWNLOAD_ACTION_DATA = 0x3E,
    PROPERTY_CFG_INFO_TYPE = 0x3F,
    // the fields of the DataStore download action (only read through the action object)
    PROPERTY_DATA_STORE_40 = 0x40,
    PROPERTY_DATA_STORE_41 = 0x41,
    PROPERTY_DATA_STORE_42 = 0x42,
    PROPERTY_DATA_STORE_43 = 0x43,
    PROPERTY_DATA_STORE_44 = 0x44,
    PROPERTY_DATA_STORE_45 = 0x45,
    PROPERTY_DATA_STORE_46 = 0x46,
};

// what the NsData header info asks for (3dbrew "BOSS:GetNsDataHeaderInfo"; 3 = the size)
enum HeaderInfoType : u8 {
    HEADER_INFO_TYPE_SIZE = 3,
};

// where the downloaded data is stored (0: on the SD card, else in NAND; names are ours)
enum StorageType : u8 {
    STORAGE_TYPE_SD = 0,
    STORAGE_TYPE_NAND = 1,
};

// the result of a task (3dbrew TaskResultCode)
enum TaskResultCode : u8 {
};

// whether the service runs the task (3dbrew TaskServiceStatus)
enum TaskServiceStatus : u8 {
};

// the codes ChangeBossRetCodeToResult turns into results (the description of the result)
enum ResultCode : u16 {
    RESULT_CODE_SUCCESS = 0,
};

// a task policy (TaskPolicy, the properties 0x00 - 0x05)
struct TaskPolicyConfig
{
    u8 priority;          // 0x00, PROPERTY_PRIORITY
    u8 schedulingPolicy;  // 0x01, PROPERTY_SCHEDULING_POLICY (always 1)
    u8 permission;        // 0x02, PROPERTY_PERMISSION
    u32 targetDuration;   // 0x04, PROPERTY_TARGET_DURATION
    u32 interval;         // 0x08, PROPERTY_INTERVAL (seconds)
    u32 count;            // 0x0C, PROPERTY_COUNT
};
ASSERT_SIZE(TaskPolicyConfig, 0x10);

// the options of a task (the properties 0x18 - 0x1C)
struct TaskOptionConfig
{
    u8 property18;  // 0x00 (must be 1)
    u8 property19;  // 0x01
    u8 property1A;  // 0x02
    u32 property1B; // 0x04
    u32 property1C; // 0x08
};
ASSERT_SIZE(TaskOptionConfig, 0xC);

// the action of a task (TaskActionBase and its subclasses)
struct TaskActionConfig
{
    u8 actionCode;          // 0x000, PROPERTY_ACTION_CODE (2: NSA download, 10: DataStore download)
    u8 fsRootCa;            // 0x001, PROPERTY_FS_ROOT_CA
    u8 fsClientCert;        // 0x002, PROPERTY_FS_CLIENT_CERT
    u8 apInfoType;          // 0x003, PROPERTY_AP_INFO_TYPE
    u8 actionDataType;      // 0x004, PROPERTY_ACTION_DATA_TYPE (which of the action data is used)
    u8 cfgInfoType;         // 0x005, PROPERTY_CFG_INFO_TYPE
    u32 property16;         // 0x008, PROPERTY_16
    u32 property08;         // 0x00C, PROPERTY_08
    u32 property3B;         // 0x010, PROPERTY_3B
    nn::Handle fileHandle;  // 0x014, PROPERTY_FILE_HANDLE (action data type 2)
    u8 actionData[0x200];   // 0x018, PROPERTY_ACTION_DATA_0A / 0B / DATA_STORE_DOWNLOAD_ACTION_DATA
    char url[0x200];        // 0x218, PROPERTY_URL
    u8 headerFields[0x360]; // 0x418, PROPERTY_HEADER_FIELDS
    u32 rootCas[3];         // 0x778, PROPERTY_ROOT_CAS
    u32 clientCerts[1];     // 0x784, PROPERTY_CLIENT_CERTS
    u32 rootCaCount;        // 0x788, PROPERTY_ROOT_CA_COUNT
    u32 clientCertCount;    // 0x78C, PROPERTY_CLIENT_CERT_COUNT
    u32 reserved790;        // 0x790 (not a property)
    u8 property15[0x40];    // 0x794, PROPERTY_15
};
ASSERT_SIZE(TaskActionConfig, 0x7D4);

// the status of a task (TaskStatus, the properties 0x1D - 0x2F)
struct TaskStatusInfo
{
    u64 lastSuccessfulTimestamp; // 0x00, PROPERTY_LAST_SUCCESSFUL_TIMESTAMP
    u64 property29;              // 0x08
    u8 stateCode;                // 0x10, PROPERTY_TASK_STATE_CODE
    u8 property1E;               // 0x11
    u8 property1F;               // 0x12
    u8 serviceStatus;            // 0x13, PROPERTY_TASK_SERVICE_STATUS
    u8 property22;               // 0x14
    u8 resultCode;               // 0x15, PROPERTY_TASK_RESULT_CODE
    u32 commErrorCode;           // 0x18, PROPERTY_COMM_ERROR_CODE
    u32 property25;              // 0x1C
    u32 property26;              // 0x20
    u32 property27;              // 0x24
    u32 property2A;              // 0x28
    u32 property2B;              // 0x2C
    u16 property2D;              // 0x30
    u16 property2E;              // 0x32
    u8 property2C;               // 0x34
    u8 property24;               // 0x35
    char lastModified[0x40];     // 0x38, PROPERTY_LAST_MODIFIED_HEADER
};
ASSERT_SIZE(TaskStatusInfo, 0x78);

// the action data of a DataStore download (actionData of the action, action code 10)
struct DataStoreDownloadData
{
    u32 gameId;           // 0x000, PROPERTY_DATA_STORE_40
    wchar_t key[9];       // 0x004, PROPERTY_DATA_STORE_41 (18 bytes)
    u8 data42[0x40];      // 0x016, PROPERTY_DATA_STORE_42
    u8 data43[0x12C];     // 0x056, PROPERTY_DATA_STORE_43
    u8 data44[8];         // 0x182, PROPERTY_DATA_STORE_44
    u8 data45;            // 0x18A, PROPERTY_DATA_STORE_45
    u32 data46;           // 0x18C, PROPERTY_DATA_STORE_46
};
ASSERT_SIZE(DataStoreDownloadData, 0x190);

} // namespace boss
} // namespace nn
