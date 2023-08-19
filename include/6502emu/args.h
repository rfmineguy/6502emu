#ifndef ARGS_H
#define ARGS_H

typedef struct {
  const char* input_file;
} args_t; 

args_t args_parse(int argc, char** argv);
void   args_usage();

#endif
