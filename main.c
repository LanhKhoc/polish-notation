#include "balan.h"
#include <stdio.h>
#include<conio.h>

int main() {
  char str[] = "((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1)) + cos(60) + sin(30)";
//  double res = calculator(str);
  printf(">> %s = %f", str, calculator(str));
  getch();
  return 0;
}
