#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char ping = 11;
const char pong = 29;

void on_receive(char signal) {
  const char rbody[] = ": received ";
  char a[12];
  write(1, a, itoa(getpid(), a));
  write(1, rbody, sizeof(rbody));

#define handle(type)        \
  case type:                \
    write(1, #type"\n", 5); \
    break;

  switch (signal) {
    handle(ping)
    handle(pong)
    default:
      write(1, "something\n", 10);
      break;
  }
#undef handle
}

int main(int argc, char *argv[]) {
  int ps[2];
  int pr[2];
  int pid;
  char signal;

  if (pipe(ps) < 0 || pipe(pr) < 0)
    exit(1);

  pid = fork();
  if (pid < 0)
    exit(1);

  if (pid) {
    close(ps[0]);
    close(pr[1]);

    write(ps[1], &ping, 1);
    close(ps[1]);
    
    read(pr[0], &signal, 1);
    close(pr[0]);

    on_receive(signal);
  }
  else {
    close(ps[1]);
    close(pr[0]);

    read(ps[0], &signal, 1);
    close(ps[0]);

    on_receive(signal);

    write(pr[1], &pong, 1);
    close(pr[1]);
  }
  
  exit(0);
}
