#ifndef _SYS_H
#define _SYS_H 1

#include <kot/arch.h>
#include <kot/types.h>
#include <kot/memory.h>
#include <kot/sys/list.h>

#define Syscall_48(syscall, arg0, arg1, arg2, arg3, arg4, arg5) (DoSyscall(syscall, (uint64_t)arg0, (uint64_t)arg1, (uint64_t)arg2, (uint64_t)arg3, (uint64_t)arg4, (uint64_t)arg5))
#define Syscall_40(syscall, arg0, arg1, arg2, arg3, arg4) (DoSyscall(syscall, (uint64_t)arg0, (uint64_t)arg1, (uint64_t)arg2, (uint64_t)arg3, (uint64_t)arg4, 0))
#define Syscall_32(syscall, arg0, arg1, arg2, arg3) (DoSyscall(syscall, (uint64_t)arg0, (uint64_t)arg1, (uint64_t)arg2, (uint64_t)arg3, 0, 0))
#define Syscall_24(syscall, arg0, arg1, arg2) (DoSyscall(syscall, (uint64_t)arg0, (uint64_t)arg1, (uint64_t)arg2, 0, 0, 0))
#define Syscall_16(syscall, arg0, arg1) (DoSyscall(syscall, (uint64_t)arg0, (uint64_t)arg1, 0, 0, 0, 0))
#define Syscall_8(syscall, arg0) (DoSyscall(syscall, (uint64_t)arg0, 0, 0, 0, 0, 0))
#define Syscall_0(syscall) (DoSyscall(syscall, 0, 0, 0, 0, 0, 0))

#if defined(__cplusplus)
extern "C" {
#endif

struct KotSpecificData_t{
    /* Memory */
    uint64_t MMapPageSize;
    /* Heap */
    uint64_t HeapLocation;
    /* UISD */
    thread UISDHandler;
    process_t UISDHandlerProcess;
    /* FreeMemorySpace */
    uintptr_t FreeMemorySpace;
}__attribute__((aligned(0x1000)));

extern struct KotSpecificData_t KotSpecificData;

struct SelfData{
    process_t threadKey;
    process_t ProcessKey;
}__attribute__((packed));

enum DataType{
    DataTypeUnknow              = 0x0,
    DataTypethread              = 0x1,
    DataTypeProcess             = 0x2,
    DataTypeEvent               = 0x3,
    DataTypeSharedMemory        = 0x4,
};

enum EventType{
    EventTypeIRQLines           = 0x0,
    EventTypeIRQ                = 0x1,
    EventTypeIPC                = 0x2,
};

enum Priviledge{
    PriviledgeDriver            = 0x1,
    PriviledgeService           = 0x2,
    PriviledgeApp               = 0x3,
};

enum MemoryFieldType{
    MemoryFieldTypeUnknow       = 0x0,
    MemoryFieldTypeShareSpaceRW = 0x1,
    MemoryFieldTypeShareSpaceRO = 0x2,
    MemoryFieldTypeSendSpaceRO  = 0x3,
};

enum ExecutionType{
    ExecutionTypeQueu           = 0x0,
    ExecutionTypeQueuAwait      = 0x1,
    ExecutionTypeOneshot        = 0x2,
    ExecutionTypeOneshotAwait   = 0x3,
};

struct ShareDataWithArguments_t{
    size_t Size;
    uintptr_t Data;
    uint8_t ParameterPosition;
};

uint64_t DoSyscall(uint64_t syscall, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);


KResult Sys_CreateMemoryField(process_t self, size_t size, uintptr_t* virtualAddressPointer, ksmem_t* keyPointer, enum MemoryFieldType type);
KResult Sys_AcceptMemoryField(process_t self, ksmem_t key, uintptr_t* virtualAddressPointer);
KResult Sys_FreeMemoryField(process_t self, ksmem_t key, uintptr_t address);
KResult Sys_GetInfoMemoryField(ksmem_t key, uint64_t* typePointer, size_t* sizePointer);
KResult SYS_ShareDataUsingStackSpace(thread self, uint64_t address, size_t size, uint64_t* clientAddress);
KResult Sys_CreateProc(process_t* key, enum Priviledge privilege, uint64_t data);
KResult Sys_Fork(process_t* src, process_t* dst);
KResult Sys_CloseProc();
KResult SYS_Close(thread self, uint64_t errorCode);
KResult SYS_Exit(thread self, uint64_t errorCode);
KResult SYS_Pause(thread self);
KResult SYS_Unpause(thread self);
KResult SYS_Map(process_t self, uint64_t* addressVirtual, bool isPhysical, uintptr_t* addressPhysical, size_t* size, bool findFree);
KResult SYS_Unmap(process_t self, uintptr_t addressVirtual, size_t size);
KResult Sys_Event_Create(kevent_t* self);
KResult Sys_Event_Bind(kevent_t self, thread task, uint8_t vector, bool IgnoreMissedEvents);
KResult Sys_Event_Unbind(kevent_t self, thread task, uint8_t vector);
KResult Sys_Event_Trigger(kevent_t self, struct arguments_t* parameters);
KResult Sys_Event_Close();
KResult Sys_Createthread(process_t self, uintptr_t entryPoint, enum Priviledge privilege, uint64_t data, thread* result);
KResult Sys_Duplicatethread(process_t parent, thread source, uint64_t data, thread* self);
KResult Sys_Execthread(thread self, struct arguments_t* parameters, enum ExecutionType type, struct ShareDataWithArguments_t* data);
KResult Sys_Keyhole_CloneModify(key_t source, key_t* destination, process_t target, uint64_t flags);
KResult Sys_Keyhole_Verify(key_t self, enum DataType type, process_t* target, uint64_t* flags);
KResult Sys_Logs(char* message, size_t size);


KResult Sys_GetthreadKey(thread* self);
KResult Sys_GetProcessKey(process_t* self);


KResult Printlog(char* message);

#if defined(__cplusplus)
} 
#endif

#endif