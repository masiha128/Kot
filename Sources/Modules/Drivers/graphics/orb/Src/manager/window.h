#include <kot/uisd.h>

#ifndef __ORB__WINDOW__
#define __ORB__WINDOW__

#include "monitor.h"

class Monitor;

enum FocusState : uint8_t {
    INACTIVE = 0,
    PASSIVE = 1,
    ACTIVE = 2,
};

struct Cursor {
    int32_t x;
    int32_t y;
    bool show;
};

class Window {
private:
    framebuffer_t* fb;
    ksmem_t fb_key;
    int32_t xPos;
    int32_t yPos;
    bool Sshow = false;
    bool Sborder = true;
    process_t owner;
    FocusState focus_state = INACTIVE;
public:
    Window(process_t orb, uint32_t width, uint32_t height, int32_t xPos, int32_t yPos);
    framebuffer_t* getFramebuffer();
    ksmem_t getFramebufferKey();
    uint32_t getHeight();
    uint32_t getWidth();
    uint32_t getPitch();
    int32_t getX();
    int32_t getY();
    void show(bool val);
    void border(bool val);
    void resize(uint32_t width, uint32_t height);
    void move(int32_t xPos, int32_t yPos);
    process_t getOwner();
    FocusState getFocusState();
    void setFocusState(FocusState focus_state);
    void destroy();
    bool hasBorder();
    bool isVisible();
};

#endif