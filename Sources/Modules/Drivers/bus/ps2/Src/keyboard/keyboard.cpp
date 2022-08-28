#include <keyboard/keyboard.h>


PS2Port_t* KeyboardPS2Port;
arguments_t* KeyboardEventParameters;
KeyboardData_t* KeyboardData;

uint8_t LedStateSaver = 0;

static uint64_t KeyboardLock;

KResult KeyboardInitialize(){
    KeyboardData = (KeyboardData_t*)malloc(sizeof(KeyboardData_t));
    for(int i = 0; i < PS2_PORT_NUMBER; i++){
        if(PS2Ports[i].Type == PS2_TYPE_KEYBOARD && PS2Ports[i].IsPresent){
            Printlog("[PS2] Keyboard device found");

            Sys_Event_Create(&KeyboardData->onKeyboardStateChanged);

            KeyboardEventParameters = (arguments_t*)malloc(sizeof(arguments_t));
            KeyboardPS2Port = &PS2Ports[i];
            IRQRedirectionsArray[KeyboardPS2Port->PortNumber] = KeyboardHandler;

            Srv_System_BindIRQLine(KeyboardPS2Port->IRQ, InterruptthreadHandler, false, true);

            KeyboardData->IsInitialized = true;
            
            break;
        }
    }
    
    return KSUCCESS;
}

KResult KeyboardHandler(uint8_t data){
    KeyboardEventParameters->arg[0] = (uint64_t)data;
    Printlog("Keyboard");
    Sys_kevent_trigger(KeyboardData->onKeyboardStateChanged, KeyboardEventParameters);

    return KSUCCESS;
}

KResult KeyboardSetLedState(enum KeyboardLEDS LEDID, bool IsOn){
    atomicAcquire(&KeyboardLock, 0);
    BIT_SETV(LedStateSaver, LEDID, IsOn);
    KeyboardPS2Port->PS2SendDataPort(0xED);
    KeyboardPS2Port->PS2SendDataPort(LedStateSaver);
    atomicUnlock(&KeyboardLock, 0);
}