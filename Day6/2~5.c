#include <stdio.h>

int  main(void)
{
  int  n, i, s;
  int  a[100];

  n = 100;
  for (i = 0; i < n; i++)
    a[i] = i;
  s = 0;
  for (i = 0; i < n; i++)
    s += a[i];
  printf("%d\n", s);
  return (0);
}
