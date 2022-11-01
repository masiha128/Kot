#pragma once

#include <kot/types.h>

#include <kot-ui++/component.h>
#include <kot-ui++/context.h>

#include <kot-graphics++/context.h>

namespace WindowUi {

    class Window {
        private:
            uint32_t wid;
            Graphic::ContextGphc* ctxGrph;
            Ui::ContextUi* ctxUi;

        public:
            /* todo: add icon */
            Window(char* title, uint32_t width, uint32_t height, uint32_t xPos, uint32_t yPos);

            void setContent(Ui::Component* content);
    };

}