#ifndef _SRV_STORAGE_H
#define _SRV_STORAGE_H 1

#include <kot/sys.h>
#include <kot/uisd.h>
#include <kot/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define Serial_Number_Size              0x14
#define Drive_Model_Number_Size         0x28

#define File_Permissions_Super_User     0x0
#define File_Permissions_Read           0x1
#define File_Permissions_Write          0x2
#define File_Permissions_Create_File    0x3


#define Client_VFS_Function_Count       0x6

#define Client_VFS_File_Remove          0x0
#define Client_VFS_File_OPEN            0x1
#define Client_VFS_Rename               0x2
#define Client_VFS_Dir_Create           0x3
#define Client_VFS_Dir_Remove           0x4
#define Client_VFS_Dir_Open             0x5


typedef KResult (*StorageCallbackHandler)(KResult Status, struct srv_storage_callback_t* Callback, uint64_t GP0, uint64_t GP1, uint64_t GP2, uint64_t GP3);

struct srv_storage_space_info_t{
    thread_t CreateProtectedDeviceSpaceThread;
    thread_t ReadWriteDeviceThread;
    ksmem_t BufferRWKey;
    uint64_t BufferRWAlignement;
    uint64_t BufferRWUsableSize;
    process_t DriverProc;
    uint64_t SpaceSize;
};

struct srv_storage_device_info_t{
    struct srv_storage_space_info_t MainSpace;
    uint64_t DeviceSize;
    uint8_t SerialNumber[Serial_Number_Size];
    uint8_t DriveModelNumber[Drive_Model_Number_Size];
};

struct srv_storage_fs_server_functions_t{
    thread_t ChangeUserData;

    thread_t Removefile;

    thread_t Openfile;

    thread_t Rename;

    thread_t Mkdir;
    thread_t Rmdir;

    thread_t Opendir;
};

struct srv_storage_fs_server_open_file_data_t{
    thread_t Dispatcher;
    process_t FSDriverProc;
};

struct srv_storage_fs_server_open_dir_data_t{
    thread_t Dispatcher;
    process_t FSDriverProc;    
};

struct srv_storage_fs_server_rename_t{
    uint64_t OldPathPosition;
    uint64_t NewPathPosition;
};

struct srv_storage_callback_t{
    thread_t Self;
    uint64_t Data;
    size64_t Size;
    bool IsAwait;
    KResult Status;
    StorageCallbackHandler Handler;
};

typedef struct {

} file_t;

typedef struct {

} directory_t;

typedef uint64_t mode_t;
typedef uint64_t permissions_t;

void Srv_Storage_Initialize();

void Srv_Storage_Callback(KResult Status, struct srv_storage_callback_t* Callback, uint64_t GP0, uint64_t GP1, uint64_t GP2, uint64_t GP3);

struct srv_storage_callback_t* Srv_Storage_AddDevice(struct srv_storage_device_info_t* Info, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_RemoveDevice(uint64_t Index, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_NotifyOnNewPartitionByGUIDType(struct GUID_t* PartitionTypeGUID, thread_t ThreadToNotify, process_t ProcessToNotify, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_MountPartition(thread_t VFSMountThread, struct srv_storage_fs_server_functions_t* FSServerFunctions, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_UnmountPartition(thread_t VFSMountThread, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_VFSLoginApp(process_t Process, permissions_t Permissions, uint64_t PID, char* Path, bool IsAwait);

#if defined(__cplusplus)
}
#endif

#endif