#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>

#define KEYCODE XK_A
const char *disp = "1";

int getDisplay(const char* disp){
  Display *display = XOpenDisplay(disp);
  if(NULL == display)
    return -1;
  return 0;
}

int main()
{
  printf("%d\n", getDisplay(disp));
  return 0;
}
