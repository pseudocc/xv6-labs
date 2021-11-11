#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char say[] = "(nothing happens for a little while)\n";
  if (argc < 2)
    exit(1);
  
  i = atoi(argv[1]);
  write(1, say, sizeof(say));
  sleep(i);

  exit(0);
}
 