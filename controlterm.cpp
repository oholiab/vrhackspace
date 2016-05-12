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
    default:
      return irrcode;
  }
}

int sendKeyEvent(const char* disp, int keycode)
{
  if(-1==keycode)
    return 0;
  printf("sendKeyEvent keycode: %d\n", keycode);
  Window winFocus;
  int revert;

  Display *display = XOpenDisplay(disp);
  if(NULL == display)
    return -1;
  Window winRoot = XDefaultRootWindow(display);
  //XSetInputFocus(display, 1, revert, CurrentTime);
  XGetInputFocus(display, &winFocus, &revert);
  XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, keycode, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  event = createKeyEvent(display, winFocus, winRoot, false, keycode, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  XCloseDisplay(display);
  return 0;
}
