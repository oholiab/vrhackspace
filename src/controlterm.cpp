#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include "controlterm.h"

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

int getNextAvailableXDisplayNumber()
{
  DIR *dir;
  struct dirent *ent;
  size_t largest = 0;
  if ((dir = opendir("/tmp/.X11-unix/")) != NULL) {
    while((ent = readdir(dir)) != NULL){
      char *entry = ent->d_name;
      if(entry[0] == 'X') { 
        int dispNum = std::stoi(ent->d_name);
        if(dispNum > largest) largest = dispNum;
      }
    }
  } else return -1;
  return largest;
};

X11Display::X11Display(const char* dispName) 
{
  display = XOpenDisplay(dispName);
  if(NULL == display) {
    printf("invalid X11 display: %s\n", dispName);
    throw;
  }
  winRoot = XDefaultRootWindow(display);
}

X11Display::~X11Display() { XCloseDisplay(display); }

int X11Display::sendKeyEvent(int keycode, bool keyDown, int mod = 0)
{
  if(-1==keycode)
    return 0;
  Window winFocus;
  int revert;

  XGetInputFocus(display, &winFocus, &revert);
  XKeyEvent event = createKeyEvent(display, winFocus, winRoot, keyDown, keycode, mod);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  return 0;
}

bool IsModKey(int keycode) {
  switch(keycode){
    //shift, control and menu (both left and right) are consecutive
    case irr::KEY_LSHIFT ... irr::KEY_RMENU :
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
    //FIXME: Should escape be a modifier too?
    case irr::KEY_ESCAPE :
      return XK_Escape;
    case irr::KEY_RETURN :
      return XK_Return;
    case irr::KEY_MINUS :
      return XK_minus;
    case irr::KEY_PLUS :
      return XK_equal;
    case irr::KEY_OEM_1 :
      return XK_semicolon;
    case irr::KEY_OEM_2 :
      return XK_slash;
    case irr::KEY_OEM_4 :
      return XK_bracketleft;
    case irr::KEY_OEM_6 :
      return XK_bracketright;
    case irr::KEY_OEM_7 :
      return XK_apostrophe;
    case irr::KEY_PERIOD :
      return XK_period;
    case irr::KEY_COMMA :
      return XK_comma;
    case irr::KEY_OEM_5 :
      return XK_backslash;
    case irr::KEY_LSHIFT :
      return XK_Shift_L;
    case irr::KEY_RSHIFT :
      return XK_Shift_R;
    case irr::KEY_LCONTROL :
      return XK_Control_L;
    case irr::KEY_RCONTROL :
      return XK_Control_R;
    default:
      return irrcode;
  }
}

