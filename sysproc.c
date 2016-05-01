#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stdio.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit(0);
  return 0;  // not reached
}

int
sys_wait(void)
{
  char* value = 0;

  argptr(0, &value, 4);

  int* status = (int*)value;

  return wait(status);
}

int
sys_waitpid(void)
{
  int PID = 0;
  char* value;
  int options = 0;
  if( argint(0, &PID) < 0 || argptr(1, &value, 4) < 0 || argint(2, &options) < 0) {
    return -1;
  }

  int* status = (int*)value;

  return waitpid(PID, status, options);

} 

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_count(void)
{
  cprintf("number of system calls: %d\n", sys_ncount());
  return sys_ncount();
}

int
sys_change_priority(void)
{
  int this_priority;

  if(argint(0, &this_priority) < 0) {
    return -1;
  }
  proc->priority = this_priority;

  return 0;
}
