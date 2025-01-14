#ifndef _KEYHOLE_H
#define _KEYHOLE_H 1

#define KeyholeFlagFullPermissions                          0xFFFFFFFFFFFFFFFF
#define KeyholeFlagPresent                                  0x0
#define KeyholeFlagCloneable                                0x1
#define KeyholeFlagEditable                                 0x2
#define KeyholeFlagOriginal                                 0x3

/* Specific thread for types */

/* DataTypeThread */
#define KeyholeFlagDataTypeThreadIsUnpauseable              0x4
#define KeyholeFlagDataTypeThreadIsEventable                0x5
#define KeyholeFlagDataTypeThreadIsDuplicable               0x6
#define KeyholeFlagDataTypeThreadIsExecutableWithQueue      0x7
#define KeyholeFlagDataTypeThreadIsExecutableOneshot        0x8

/* DataTypeProcess */
#define KeyholeFlagDataTypeProcessMemoryAccessible          0x4
#define KeyholeFlagDataTypeProcessIsThreadCreateable        0x5

/* DataTypeEvent */
#define KeyholeFlagDataTypeEventIsBindable                  0x4
#define KeyholeFlagDataTypeEventIsTriggerable               0x5

/* DataTypeSharedMemory */


static inline bool Keyhole_GetFlag(uint64_t Flags, uint8_t Flag){
    return (Flags >> Flag) & 0x1;
}

static inline void Keyhole_SetFlag(uint64_t* Flags, uint8_t Flag, bool Status){
    if(Status){
        *Flags |= Flag;        
    }else{
        *Flags &= ~Flag;
    }

}

#endif