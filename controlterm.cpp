#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <unistd.h>

#define KEYCODE XK_m
const char *disp;

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

int sendKeyEvent(const char* disp)
{
  Window winFocus;
  int revert;

  Display *display = XOpenDisplay(disp);
  if(NULL == display)
    return -1;
  Window winRoot = XDefaultRootWindow(display);
  printf("winRoot: %d\n", winRoot);
  //XSetInputFocus(display, 1, revert, CurrentTime);
  XGetInputFocus(display, &winFocus, &revert);
  printf("winFocus: %d\n", winFocus);
  XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, KEYCODE, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  event = createKeyEvent(display, winFocus, winRoot, false, KEYCODE, 0);
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);
  XCloseDisplay(display);
  return 0;
}

int main(int argc, char **argv)
{
  disp=argv[1];
  printf("%d\n", sendKeyEvent(disp));
  return 0;
}
