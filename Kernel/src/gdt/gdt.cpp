#include "gdt.h"
#include "../tss/tss.h"


static __attribute__((aligned(0x1000)))GDTEntry GDTEntries[GDT_MAX_DESCRIPTORS];
static GDTDescriptor gdtBaseInfo;

int GDTIndexTable = 0;
int GDTKernelSelector = 0;
int GDTUserSelector = 0;

const uint8_t BASE_DESC = GDT_DESC_PRESENT | GDT_DESC_READWRITE | GDT_DESC_CODEDATA;
const uint8_t BASE_GRAN = GDT_GRAN_64BIT | GDT_GRAN_4K;

void gdtInit(){
    TSS DefaultTSS;
    gdtBaseInfo.Size = (sizeof(GDTEntry) * GDT_MAX_DESCRIPTORS) - 1; //get the total size where gdt entries are
    gdtBaseInfo.Offset = (uint64_t)&GDTEntries[0]; //get adress of the table where gdt entries are

    gdtInstallDescriptor(0, 0, 0x00, 0x00); // kernel null
    GDTKernelSelector = gdtInstallDescriptor(0, 0, BASE_DESC | GDT_DESC_EXECUTABLE, BASE_GRAN); // kernel code segment
    gdtInstallDescriptor(0, 0, BASE_DESC, BASE_GRAN); // kernel data segment


    gdtInstallDescriptor(0, 0, 0x00, 0x00); // user null
    GDTUserSelector = gdtInstallDescriptor(0, 0, BASE_DESC | GDT_DESC_EXECUTABLE | GDT_DESC_DPL, BASE_GRAN); // user code segment
    gdtInstallDescriptor(0, 0, BASE_DESC | GDT_DESC_DPL, BASE_GRAN); // user data segment    
    TSSInit();
    
    LoadGDT(&gdtBaseInfo);    
    
    for(int i = 0; i < MAX_PROCESSORS; i++){
        if(TSSdescriptorsLocation[i] != 0){
            //pass the location of tr            
            asm("movw %%ax, %w0\n\t"
                "ltr %%ax" :: "a" (TSSdescriptorsLocation[i]));
        }else{
            break;
        }
    }
}

int gdtInstallDescriptor(uint64_t base, uint64_t limit, uint8_t access, uint8_t flags){
    if(GDTIndexTable >= GDT_MAX_DESCRIPTORS) {
        return 0;
    }

    GDTEntries[GDTIndexTable].Base0 = (uint16_t)(base & 0xFFFF);
    GDTEntries[GDTIndexTable].Base1 = (uint8_t)((base >> 16) & 0xFF);
    GDTEntries[GDTIndexTable].Base2 = (uint8_t)((base >> 24) & 0xFF);
    GDTEntries[GDTIndexTable].Limit0 = (uint16_t)(limit & 0xFFFF);
    GDTEntries[GDTIndexTable].Other = (uint8_t)((limit >> 16) & 0x0F);

    GDTEntries[GDTIndexTable].AccessByte = access;
    GDTEntries[GDTIndexTable].Other |= flags & 0xF0;

    GDTIndexTable++;
    return (GDTIndexTable - 1) * sizeof(GDTEntry);
}

uint16_t gdtInstallTSS(uint64_t base, uint64_t limit){ 
    if(GDTIndexTable >= GDT_MAX_DESCRIPTORS) {
        return 0;
    }

    uint16_t TSSType = GDT_DESC_ACCESS | GDT_DESC_EXECUTABLE | GDT_DESC_PRESENT;
    gdtTSSEntry* gdt_desc = (gdtTSSEntry*)&GDTEntries[GDTIndexTable];

    gdt_desc->Type = (uint16_t)(TSSType & 0x00FF);

    gdt_desc->Base0 = base & 0xFFFF;
    gdt_desc->Base1 = (base & 0xFF0000) >> 16;
    gdt_desc->Base2 = (base & 0xFF000000) >> 24;
    gdt_desc->Base3 = base >> 32;

    gdt_desc->Limit0 = limit & 0xFFFF;
    gdt_desc->Limit1 = (limit & 0xF0000) >> 16;

    gdt_desc->Reserved = 0;

    GDTIndexTable += 2;

    return (uint16_t)((GDTIndexTable - 2) * sizeof(GDTEntry));
}