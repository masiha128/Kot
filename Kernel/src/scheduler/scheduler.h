#pragma once
#include <lib/types.h>
#include <lib/limits.h>
#include <event/event.h>
#include <keyhole/keyhole.h>
#include <memory/heap/heap.h>
#include <arch/x86-64/tss/tss.h>
#include <memory/paging/pageTableManager.h>
#include <arch/x86-64/userspace/userspace.h>

struct TaskQueuNode;
struct Task;
class TaskManager;
class SelfData;

#define KernelStackSize 0x10000

#define StackTop GetVirtualAddress(0x100, 0, 0, 0)
#define StackBottom GetVirtualAddress(0xff, 0, 0, 0) 
#define LockAddress GetVirtualAddress(0xfe, 0, 0, 0) 
#define SelfDataStartAddress StackBottom  
#define SelfDataEndAddress StackBottom + sizeof(SelfData)
#define DefaultFlagsKey 0xff

struct SelfData{
    key_t ThreadKey;
    key_t ProcessKey;
}__attribute__((packed));

struct Parameters{
    uint64_t Parameter0;
    uint64_t Parameter1;
    uint64_t Parameter2;
    uint64_t Parameter3;
    uint64_t Parameter4;
    uint64_t Parameter5;
}__attribute__((packed));

struct threadInfo_t{
    uint64_t SyscallStack;
    uint64_t CS;
    uint64_t SS;
    thread_t* Thread;
}__attribute__((packed));

struct StackInfo{
    /* stack is also use for TLS */
    uint64_t StackStart;
    uint64_t StackEndMax;
}__attribute__((packed));

struct StackData{
    size_t size;
}__attribute__((packed));

struct thread_t;

struct process_t{
    /* ID infos */
    uint64_t PID;

    /* Priviledge */
    uint8_t DefaultPriviledge:3;

    /* Memory */
    struct PageTableManager* SharedPaging;

    /* Childs */
    Node* Childs;
    uint64_t TID;
    uint64_t NumberOfThread;

    /* Time info */
    uint64_t CreationTime;

    /* Keyhole */
    void* Locks;
    uint64_t LockIndex;
    uint64_t LockLimit;

    /* parent */
    Node* NodeParent;
    TaskManager* TaskManagerParent;

    /* external data */
    void* externalData;

    thread_t* CreatThread(uint64_t entryPoint, void* externalData);
    thread_t* CreatThread(uint64_t entryPoint, uint8_t priviledge, void* externalData);
    thread_t* DuplicateThread(thread_t* source);
}__attribute__((packed));  

struct thread_t{
    /* ID infos */
    uint64_t TID;

    /* Thread infos */
    threadInfo_t* Info;
    void* EntryPoint;

    /* Memory */
    struct PageTableManager* Paging;
    uint64_t MemoryAllocated;

    /* Context info */
    ContextStack* Regs; 
    StackInfo* Stack; 
    uint64_t CoreID;
    bool IsBlock;

    /* Time info */
    uint64_t TimeAllocate;
    uint64_t CreationTime;

    /* Privledge */
    uint8_t RingPL:3;
    uint8_t IOPL:3;
    

    /* Process */
    process_t* Parent;
    Node* ThreadNode;
    
    /* Fork */
    bool IsForked;
    thread_t* ForkedThread;

    /* Event */
    bool IsEvent;
    struct event_t* Event;

    /* Schedule queue */
    bool IsInQueue;
    thread_t* Last;
    thread_t* Next;

    /* external data */
    void* externalData;

    void SaveContext(struct ContextStack* Registers, uint64_t CoreID);
    void CreatContext(struct ContextStack* Registers, uint64_t CoreID);

    void SetParameters(Parameters* FunctionParameters);

    void SetupStack();
    void CopyStack(thread_t* source);
    bool ExtendStack(uint64_t address);
    void* ShareDataInStack(void* data, size_t size);

    bool Fork(struct ContextStack* Registers, uint64_t CoreID, thread_t* thread, Parameters* FunctionParameters);
    bool Fork(struct ContextStack* Registers, uint64_t CoreID, thread_t* thread);

    bool Launch(Parameters* FunctionParameters);  
    bool Launch();  
    bool Pause(ContextStack* Registers, uint64_t CoreID);   
    bool Exit(ContextStack* Registers, uint64_t CoreID);  

    bool SetIOPriviledge(ContextStack* Registers, uint8_t IOPL);
}__attribute__((packed));  

class TaskManager{
    public:
        void Scheduler(struct ContextStack* Registers, uint64_t CoreID);
        void SwitchTask(struct ContextStack* Registers, uint64_t CoreID, thread_t* task);

        void EnqueueTask(struct thread_t* task);
        void DequeueTask(struct thread_t* task);
        void DequeueTaskWithoutLock(struct thread_t* task);

        // threads
        thread_t* GetTread();
        uint64_t CreatThread(thread_t** self, process_t* proc, uint64_t entryPoint, void* externalData);
        uint64_t DuplicateThread(thread_t** self, process_t* proc, thread_t* source);
        uint64_t ExecThread(thread_t* self, Parameters* FunctionParameters);
        uint64_t Pause(ContextStack* Registers, uint64_t CoreID, thread_t* task); 
        uint64_t Unpause(thread_t* task); 
        uint64_t Exit(ContextStack* Registers, uint64_t CoreID, thread_t* task); 

        // process
        uint64_t CreatProcess(process_t** key, uint8_t priviledge, void* externalData);

        void CreatIddleTask();   

        void InitScheduler(uint8_t NumberOfCores); 
        void EnabledScheduler(uint64_t CoreID);
        thread_t* GetCurrentThread(uint64_t CoreID);

        bool IsSchedulerEnable[MAX_PROCESSORS];
        uint64_t TimeByCore[MAX_PROCESSORS];
        thread_t* ThreadExecutePerCore[MAX_PROCESSORS];

        bool TaskManagerInit;  

        uint64_t NumberProcessTotal = 0;
        uint64_t CurrentTaskExecute = 0;
        uint64_t IddleTaskNumber = 0;
        uint64_t PID = 0;

        thread_t* FirstNode;
        thread_t* LastNode;

        Node* ProcessList = NULL;
        //iddle
        process_t* IddleProc = NULL;
        thread_t* IdleNode[MAX_PROCESSORS];    
        Node* GlobalProcessNode;
};



extern TaskManager* globalTaskManager;