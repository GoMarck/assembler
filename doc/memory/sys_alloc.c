#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
  void *brk_addr = sbrk(0);
  printf("break point is:  %p\n", brk_addr);
  // void *new_brk_addr = sbrk(1024);
  
  // printf("break point is:  %p\n", new_brk_addr);
  void *new_brk_addr = sbrk(0);
  printf("break point is:  %p\n", new_brk_addr);

  new_brk_addr = sbrk(0);
  printf("break point is:  %p\n", new_brk_addr);

  new_brk_addr = sbrk(0);
  printf("break point is:  %p\n", new_brk_addr);

  void *mmap_addr =
      mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  printf("mmap address is: %p\n", mmap_addr);
  return 0;
}