#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sysinfo.h"
#include "proc.h"

uint64
sys_info(void)
{
  struct proc* p = myproc();
  struct sysinfo si;
  uint64 addr;

  if (argaddr(0, &addr) < 0)
    return -1;

  si.freemem = kremain();
  si.nproc = procalive();

  if (copyout(p->pagetable, addr, (char*)&si, sizeof(si)) < 0)
    return -1;
  return 0;
}