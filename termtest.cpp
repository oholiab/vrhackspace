#include <stdio.h>
//TODO: Do we really need stdlib? using for atoi.
#include <stdlib.h>
#include "controlterm.cpp"

#define KEYCODE XK_m

const char *disp;

X11Display xdisp(disp);

int main(int argc, char **argv)
{
  if(argc < 2)
    return 1;
  disp=argv[1];
  int keycode=atoi(argv[2]);
  printf("%d\n", xdisp.sendKeyEvent(keycode, true));
  printf("%d\n", xdisp.sendKeyEvent(keycode, false));
  return 0;
}
