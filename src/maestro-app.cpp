#include <stdio.h>
#include <stdlib.h>
#include "maestro.h"

int main(int argc, char* argv[]) {
  int pw = 6000; // pulse width
  if (argc > 1) {
    pw = atoi(argv[1]);
  }
  
  int s = 100; // speed
  if (argc > 2) {
    s = atoi(argv[2]);
  }

  printf("speed: %d * 0.25us / 10ms\n", s);
  printf("target: %d * 0.25us\n", pw);

  Maestro maestro;
  maestro.setSpeed(0, s);
  maestro.setTarget(0, pw);
}
