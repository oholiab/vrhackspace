#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <irrlicht/Keycodes.h>
XKeyEvent createKeyEvent(Display *display,
                         Window &win,
                         Window &winRoot,
                         bool press,
                         int keycode,
                         int modifiers);

class X11Display {
  public:
    Display *display;
    Window winRoot;
    X11Display(const char* dispName);
    ~X11Display();
    int sendKeyEvent(int keycode, bool keyDown, int mod);
};

bool IsModKey(int keycode);

int mapKeyCode(int irrcode);

int getNextAvailableXDisplayNumber();
