#pragma once
#include <kot/types.h>

#define UKL_MODULE_STRING_SIZE 128

#define UKL_MMAP_USABLE                 1
#define UKL_MMAP_RESERVED               2
#define UKL_MMAP_KERNEL                 3
#define UKL_MMAP_MODULES                4
#define UKL_MMAP_FRAMEBUFFER            5

#define UKL_OLD_ACPI 0
#define UKL_NEW_ACPI 1

/* Structures */

struct ukl_initrd_t{
	uint64_t base;
	size64_t size;
}__attribute__((packed));


struct ukl_kernel_address_t{
    uint64_t virtual_base_address;
    uint64_t virtual_end_address;
}__attribute__((packed));


struct ukl_framebuffer_t{
    uint64_t framebuffer_base;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_bpp;
}__attribute__((packed));


struct ukl_module_t{
    uint64_t base;
    size64_t size;
    char string[UKL_MODULE_STRING_SIZE];
}__attribute__((packed));

struct ukl_modules_t{
    uint64_t module_count;
    struct ukl_module_t modules[];
}__attribute__((packed));


struct ukl_mmap_entry_t{
    uint32_t type;
    uint64_t base;
    size64_t size;
}__attribute__((packed));

struct ukl_memmap_t{
    uint64_t entries;
    struct ukl_mmap_entry_t memmap[];
}__attribute__((packed));


struct ukl_rsdp_t{
    uint64_t type;
    uint64_t base;
    size64_t size;
}__attribute__((packed));


struct ukl_smbios_t{
    uint64_t base;
    size64_t size;
}__attribute__((packed));



/* Boot structure */
struct ukl_boot_structure_t{
	struct ukl_kernel_address_t KernelAddress;
    struct ukl_framebuffer_t Framebuffer;
    struct ukl_modules_t Modules;
    struct ukl_memmap_t* Memory;
    uint64_t HHDMBase;
    uint64_t BootloaderSignature;
    struct ukl_rsdp_t RSDP;
    struct ukl_smbios_t SMBIOS;
    struct ukl_initrd_t* Initrd;
}__attribute__((packed));