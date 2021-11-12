#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#ifdef DEBUG
static char a[12];
#endif

void _exec(char* cmd, int xargc, char* xargv[]) {
  char* strp;
  int i;

  if (*cmd == '\0')
    return;
  
  i = xargc;
  strp = cmd;
  while (strp != (void*)0)
    xargv[i++] = strsep(&strp, " \t");

#ifdef DEBUG
  int l = i;
  for (i = 0; i < l; i++) {
    write(2, a, itoa(i, a));
    write(2, ": \"", 3);
    write(2, xargv[i], strlen(xargv[i]));
    write(2, "\"\n", 2);
  }
#endif

  i = fork();
  if (i < 0)
    exit(1);

  if (i == 0)
    exit(exec(xargv[0], xargv));
  wait(0);
}

int main(int argc, char* argv[]) {
  char* xargv[MAXARG];
  char* strp;
  char buf[2048];
  int i;
  
  if (argc < 2)
    exit(1);

  i = read(0, buf, sizeof(buf));

#ifdef DEBUG
  write(2, "read: ", 6);
  write(2, a, itoa(i, a));
  write(2, " bytes\n", 7);
#endif

  if (i < 1 || i == sizeof(buf))
    exit(1);

#ifdef DEBUG
  write(2, "\"", 1);
  write(2, buf, strlen(buf));
  write(2, "\"\n", 2);
#endif

  for (i = 0; i < argc - 1; i++)
    xargv[i] = argv[i + 1];
  strp = buf;
  while (strp != (void*)0)
    _exec(strsep(&strp, "\n"), i, xargv);

  exit(0);
}