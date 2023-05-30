#include "window.h"

Window::Window()
{

}

void clearWindow() {
    Imagine::drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Imagine::BLACK);
}


