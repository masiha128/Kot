#pragma once
#include <kot/types.h>
#include <event/event.h>
#include <kot/sys/list.h>
#include <memory/memory.h>
#include <scheduler/scheduler.h>
#include <arch/x86-64/interrupts/interrupts.h>

typedef uint64_t (*SyscallHandler)(ContextStack* Registers, struct kthread_t* thread);

extern "C" void SyscallEnable(uint16_t KernelSegment, uint16_t UserSegment);

extern "C" void SyscallDispatch(struct ContextStack* Registers, struct kthread_t* Self);