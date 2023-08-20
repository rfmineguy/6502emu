#ifndef ARGS_H
#define ARGS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char* input_file;
} args_t; 

args_t args_parse(int argc, char** argv);
void   args_usage();

#ifdef __cplusplus
}
#endif

#endif
