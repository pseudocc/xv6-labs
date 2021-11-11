#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define END 35

int is_prime(int n) {
  if (n < 2)
    return 0;
  if (n < 4)
    return 1;

  if (n % 2 == 0 || n % 3 == 0)
    return 0;
  
  for (int i = 5; i * i < n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0)
      return 0;
  return 1;
}

void try_print(int n) {
  char a[12];
  if (is_prime(n)) {
    write(1, "prime ", 6);
    write(1, a, itoa(n, a));
    write(1, "\n", 1);
  }
}

void on_receive(int n) {
  int i;
  int p[2] = { 0, 0 };

  if (++n > END)
    exit(0);

  if ((i = fork()) < 0 || pipe(p) < 0)
    exit(1);
  
  if (i) {
    close(p[1]);
    read(p[0], &n, sizeof(int));
    close(p[0]);

    try_print(n);
    on_receive(n);
  }
  else {
    close(p[0]);
    write(p[1], &n, sizeof(int));
    close(p[1]);
  }
}

int main(int argc, char *argv[]) {
  on_receive(0);
  exit(0);
}
