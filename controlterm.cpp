#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <irrlicht/Keycodes.h>
#include <stdio.h>
#include <unistd.h>

#define RETURN 13
#define BS 8
#define SPACE 32

XKeyEvent createKeyEvent(Display *display,
                         Window &win,
                         Window &winRoot,
                         bool press,
                         int keycode,
                         int modifiers)
{
  // Largely lifted from
  // http://www.doctort.org/adam/nerd-notes/x11-fake-keypress-event.html
  XKeyEvent event;
  event.display = display;
  event.window = win;
  event.root = winRoot;
  event.subwindow = None;
  event.time = CurrentTime;
  event.x = 1;
  event.y = 1;
  event.x_root = 1;
  event.y_root = 1;
  event.same_screen = True;
  event.keycode = XKeysymToKeycode(display, keycode);
  event.state = modifiers;

  if(press)
    event.type = KeyPress;
  else
    event.type = KeyRelease;

  return event;
}

class X11Display {
  public:
    Display *display;
    Window winRoot;
    X11Display(const char* dispName);
    ~X11Display();
    int sendKeyEvent(int keycode, bool keyDown, int mod);
};

// TODO: fail out if display doesn't initialize
X11Display::X11Display(const char* dispName) :
  display(XOpenDisplay(dispName)),
  winRoot(XDefaultRootWindow(display)) {}

X11Display::~X11Display() { XCloseDisplay(display); }

int X11Display::sendKeyEvent(int keycode, bool keyDown, int mod = 0)
{
  if(-1==keycode)
    return 0;
  printf("sendKeyEvent keycode: %d\n", keycode);
  Window winFocus;
  int revert;

  XGetInputFocus(display, &winFocus, &revert);
  XKeyEvent event = createKeyEvent(display, winFocus, winRoot, keyDown, keycode, mod);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  return 0;
}

bool IsModKey(int keycode) {
  switch(keycode){
    case irr::KEY_LSHIFT :
      return true;
    default :
      return false;
  }
}

int mapKeyCode(int irrcode){
  switch(irrcode){
    //FIXME: only to get rid of arrow spam
    case 37 ... 40:
      return -1;
    case irr::KEY_BACK :
      return XK_BackSpace;
    case irr::KEY_TAB :
      return XK_Tab;
    case irr::KEY_RETURN :
      return XK_Return;
    case irr::KEY_MINUS :
      return XK_minus;
    case irr::KEY_OEM_7 :
      return XK_apostrophe;
    case irr::KEY_LSHIFT :
      return XK_Shift_L;
    default:
      return irrcode;
  }
}

