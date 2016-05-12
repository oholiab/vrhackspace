#include <stdio.h>
#include "controlterm.cpp"

#define KEYCODE XK_m

const char *disp;

int main(int argc, char **argv)
{
  disp=argv[1];
  printf("%d\n", sendKeyEvent(disp, KEYCODE));
  return 0;
}
