#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED

#include "util.h"
#include "balan.h"

const int PIPES_READ = 0;
const int PIPES_WRITE = 1;
const int INPUT_LEN = 64;

void doChild(int data_pipes[]) {
  char input[INPUT_LEN];
  char output[INPUT_LEN];
  int rc;

  // NOTE: Read from other process with PIPES_READ
  memset(input, 0, sizeof(input));
  rc = read(data_pipes[PIPES_READ], &input, sizeof(input));
  printf("\n>> Ket qua phep tinh %s = ", trim(input));
  // puts(input);

  // NOTE: Write to other process with PIPES_WRITE
  memset(output, 0, sizeof(output));
  double res = calculator(input);
  snprintf(output, INPUT_LEN, "%f", res);
  rc = write(data_pipes[PIPES_WRITE], &output, sizeof(output));

  // NOTE: Close all pipes
  close(data_pipes[PIPES_WRITE]);
  close(data_pipes[PIPES_READ]);
  exit(0);
}

void doParent(int data_pipes[]) {
  int c, rc;
  char input[INPUT_LEN];
  char output[INPUT_LEN];

  // NOTE: Write to other process with PIPES_WRITE
  memset(input, 0, sizeof(input));
  printf("\nNhap chuoi phep tinh: ");
  fgets(input, sizeof input, stdin);
  rc = write(data_pipes[PIPES_WRITE], &input, sizeof(input));

  // NOTE: Wait for other process send data
  wait(NULL);

  // NOTE: Read from other process with PIPES_READ
  memset(output, 0, sizeof(output));
  rc = read(data_pipes[PIPES_READ], &output, sizeof(output));
  printf("%s\n", trim(output));

  // NOTE: Close all pipes
  close(data_pipes[PIPES_WRITE]);
  close(data_pipes[PIPES_READ]);
  exit(0);
}

#endif // PIPE_H_INCLUDED