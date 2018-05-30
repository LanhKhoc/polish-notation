#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pipe.h"

int main() {
  int data_pipes[2];
  int pid;
  int rc;

  rc = pipe(data_pipes);
  if(rc == -1) {
    perror("Pipe not created!");
    exit(1);
  }

  pid = fork();
  switch(pid) {
    case -1: {
      perror("Child process not created!");
      exit(1);
    }

    case 0: {
      doChild(data_pipes);
    }

    default: {
      doParent(data_pipes);
    }
  }
}
