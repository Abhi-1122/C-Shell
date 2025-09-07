#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int
main(int argc, char *argv[])
{
  char buf[100];
  int fd;

  printf("Initial readcount = %d\n", getreadcount());

  fd = open("README", 0);   // open some file (README exists in xv6)
  if(fd < 0){
    printf("cannot open file\n");
    exit(1);
  }

  if(read(fd, buf, sizeof(buf)) < 0){
    printf("read failed\n");
    close(fd);
    exit(1);
  }
  close(fd);

  printf("After reading 100 bytes, readcount = %d\n", getreadcount());

  exit(0);
}
