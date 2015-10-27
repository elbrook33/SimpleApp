#include <stdio.h>

typedef enum { A, B, C, D } Event;

int main() {
  double n = 3.5;
  switch((Event)n) {
    case D:
      printf("OK\n");
      break;
    default:
      printf("Hmm\n");
  }
  return 0;
}