#include <srv/srv.h>

uint64_t SrvLock;
uisd_storage_t* StorageHandler;

void SrvAddDevice(Device* Device){
    atomicAcquire(&SrvLock, 0);

    // Add device to external handler
    srv_storage_device_info_t Info;

    StorageHandler = (uisd_storage_t*)FindControllerUISD(ControllerTypeEnum_Storage);

    // Create threads handler
    process_t Proc = Sys_GetProcess();

    /* CreateProtectedDeviceSpaceThread */
    thread_t SrvCreateProtectedSpaceThread = NULL;
    Sys_Createthread(Proc, (uintptr_t)&SrvCreateProtectedSpace, PriviledgeDriver, (uint64_t)Device->DefaultSpace, &SrvCreateProtectedSpaceThread);
    Info.MainSpace.CreateProtectedDeviceSpaceThread = MakeShareableThreadToProcess(SrvCreateProtectedSpaceThread, StorageHandler->ControllerHeader.Process);

    /* ReadWriteDeviceThread */
    thread_t SrvReadWriteHandlerThread = NULL;
    Sys_Createthread(Proc, (uintptr_t)&SrvReadWriteHandler, PriviledgeApp, (uint64_t)Device->DefaultSpace, &SrvReadWriteHandlerThread);
    Info.MainSpace.ReadWriteDeviceThread = MakeShareableThreadToProcess(SrvReadWriteHandlerThread, StorageHandler->ControllerHeader.Process);

    Info.MainSpace.BufferRWKey = Device->DefaultSpace->BufferKey;
    Info.MainSpace.SpaceSize = Device->DefaultSpace->Size;
    Info.MainSpace.BufferRWAlignement = Device->BufferAlignement;
    Info.MainSpace.BufferRWUsableSize = Device->BufferUsableSize;
    Info.DeviceSize = Device->GetSize();

    memcpy(&Info.SerialNumber, Device->GetSerialNumber(), SerialNumberSize);
    memcpy(&Info.DriveModelNumber, Device->GetSerialNumber(), DriveModelNumberSize);
    
    Info.DriverProc = ShareProcessKey(Proc);

    srv_storage_callback_t* callback = Srv_Storage_AddDevice(&Info, true);
    Device->ExternalID = (uint64_t)callback->Data;

    free(callback);
    atomicUnlock(&SrvLock, 0);
}

void SrvRemoveDevice(thread_t Callback, uint64_t CallbackArg){
    atomicAcquire(&SrvLock, 0);
    Space_t* Space = (Space_t*)Sys_GetExternalDataThread();
    // Remove device to external handler
    srv_storage_callback_t* callback = Srv_Storage_RemoveDevice(Space->StorageDevice->ExternalID, true);
    atomicUnlock(&SrvLock, 0);
}

void SrvCreateProtectedSpace(thread_t Callback, uint64_t CallbackArg, uint64_t Start, uint64_t Size){
    KResult Statu = KFAIL;

    Space_t* ActualSpaceLocalInfo = (Space_t*)Sys_GetExternalDataThread();

    if(ActualSpaceLocalInfo->Start > Start){
        Start = ActualSpaceLocalInfo->Start;
    }

    if(ActualSpaceLocalInfo->Size > Size){
        Size = ActualSpaceLocalInfo->Size;
    }
    Space_t* SpaceLocalInfo = ActualSpaceLocalInfo->StorageDevice->CreateSpace(Start, Size);

    srv_storage_space_info_t SpaceInfo;

    process_t Proc = Sys_GetProcess();

    /* CreateProtectedDeviceSpaceThread */
    thread_t SrvCreateProtectedSpaceThread = NULL;
    Sys_Createthread(Proc, (uintptr_t)&SrvCreateProtectedSpace, PriviledgeApp, (uint64_t)SpaceLocalInfo, &SrvCreateProtectedSpaceThread);
    SpaceInfo.CreateProtectedDeviceSpaceThread = MakeShareableSpreadThreadToProcess(SrvCreateProtectedSpaceThread, StorageHandler->ControllerHeader.Process);

    /* ReadWriteDeviceThread */
    thread_t SrvReadWriteHandlerThread = NULL;
    Sys_Createthread(Proc, (uintptr_t)&SrvReadWriteHandler, PriviledgeApp, (uint64_t)SpaceLocalInfo, &SrvReadWriteHandlerThread);
    SpaceInfo.ReadWriteDeviceThread = MakeShareableSpreadThreadToProcess(SrvReadWriteHandlerThread, StorageHandler->ControllerHeader.Process);

    SpaceInfo.BufferRWKey = SpaceLocalInfo->BufferKey;
    SpaceInfo.BufferRWAlignement = ActualSpaceLocalInfo->StorageDevice->BufferAlignement;
    SpaceInfo.BufferRWUsableSize = ActualSpaceLocalInfo->StorageDevice->BufferUsableSize;
    SpaceInfo.SpaceSize = Size;

    ShareDataWithArguments_t data{
        .Data = &SpaceInfo,
        .Size = sizeof(srv_storage_space_info_t),
        .ParameterPosition = 0x2,
    };
    
    arguments_t arguments{
        .arg[0] = Statu,            /* Status */
        .arg[1] = CallbackArg,      /* CallbackArg */
        .arg[2] = NULL,             /* SpaceInfo */
        .arg[3] = NULL,             /* GP1 */
        .arg[4] = NULL,             /* GP2 */
        .arg[5] = NULL,             /* GP3 */
    };

    Sys_Execthread(Callback, &arguments, ExecutionTypeQueu, &data);
    Sys_Close(KSUCCESS);
}

void SrvReadWriteHandler(thread_t Callback, uint64_t CallbackArg, uint64_t Start, size64_t Size, bool IsWrite){
    KResult Statu = KFAIL;

    Space_t* Space = (Space_t*)Sys_GetExternalDataThread();
    if(IsWrite){
        Statu = SrvWrite(Space, Space->StorageDevice, Start, Size);
    }else{
        Statu = SrvRead(Space, Space->StorageDevice, Start, Size);
    }

    arguments_t arguments{
        .arg[0] = Statu,            /* Status */
        .arg[1] = CallbackArg,      /* CallbackArg */
        .arg[2] = NULL,             /* GP0 */
        .arg[3] = NULL,             /* GP1 */
        .arg[4] = NULL,             /* GP2 */
        .arg[5] = NULL,             /* GP3 */
    };

    Sys_Execthread(Callback, &arguments, ExecutionTypeQueu, NULL);
    Sys_Close(KSUCCESS);
}

KResult SrvRead(Space_t* Space, Device* Device, uint64_t Start, size64_t Size){
    if(Size <= Device->BufferUsableSize){
        return Device->Read(Space, Start, Size);
    }
    return KFAIL;
}

KResult SrvWrite(Space_t* Space, Device* Device, uint64_t Start, size64_t Size){
    if(Size <= Device->BufferUsableSize){
        return Device->Write(Space, Start, Size);
    }
    return KFAIL;
}