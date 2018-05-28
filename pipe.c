#include <stdio.h>
#include <unistd.h>

const int PIPES_READ = 0;
const int PIPES_WRITE = 1;
const int INPUT_LEN = 64;

int calculator(char input[]) {

}

void doChild(int data_pipes[]) {
  char input[INPUT_LEN];
  int rc;

  // Read from other process with PIPES_READ
  memset(input, 0, sizeof(input));
  rc = read(data_pipes[PIPES_READ], &input, sizeof(input));
  printf("\nPIPES_READ doChild: ");
  puts(input);

  // Write to other process with PIPES_WRITE
  strcat(input, "haha");
  rc = write(data_pipes[PIPES_WRITE], &input, sizeof(input));

  // Close all pipes
  close(data_pipes[PIPES_WRITE]);
  close(data_pipes[PIPES_READ]);
  exit(0);
}

void doParent(int data_pipes[]) {
  int c, rc;
  char input[INPUT_LEN];

  // Write to other process with PIPES_WRITE
  memset(input, 0, sizeof(input));
  printf("\nNhap chuoi phep tinh: ");
  gets(input);
  rc = write(data_pipes[PIPES_WRITE], &input, sizeof(input));

  // Wait for other process send data
  wait(NULL);

  // Read from other process with PIPES_READ
  memset(input, 0, sizeof(input));
  rc = read(data_pipes[PIPES_READ], &input, sizeof(input));
  printf("\nPIPES_READ doParent: ");
  puts(input);

  // Close all pipes
  close(data_pipes[PIPES_WRITE]);
  close(data_pipes[PIPES_READ]);
  exit(0);
}

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