#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

typedef struct stat st_t;
typedef struct dirent de_t;

#define TRACE {\
  char a[12];  \
  write(2, a, itoa(__LINE__, a));\
  write(2, "\n", 1);\
}

void join(char* p, char* n, char* buf, int* l) {
  int l1, l2;
  
  l1 = strlen(p);
  l2 = strlen(n);
  strcpy(buf, p);
  if (buf[l1 - 1] != '/')
    buf[l1++] = '/';
  strcpy(buf + l1, n);
  buf[*l = l1 + l2] = '\0';
}

void find(char* p, char* n, char* t) {
  char np[128];
  int fd, l;
  st_t st;
  de_t de;

#ifdef DEBUG
  write(2, "[DEBUG]\t", 8);
  write(2, "p: \"", 4);
  write(2, p, strlen(p));
  write(2, "\"\t", 2);
  write(2, "n: \"", 4);
  write(2, n, strlen(n));
  write(2, "\"\n", 2);
#endif

  if ((fd = open(p, 0)) < 0 || fstat(fd, &st) < 0)
    return;
  
  switch (st.type) {
  case T_FILE:
    if (strcmp(n, t) == 0) {
      write(1, p, strlen(p));
      write(1, "\n", 1);
    }
    break;
  case T_DIR:
    while (read(fd, &de, sizeof(de_t)) == sizeof(de_t)) {
      if (de.inum == 0 || de.name[0] == '.')
        continue;
      join(p, de.name, np, &l);
      find(np, de.name, t);
    }
    break;
  default:
    break;
  }
  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc < 3)
    exit(1);
  find(argv[1], "", argv[2]);
  exit(0);
}
