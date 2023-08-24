#ifndef DBG_PARSE_H
#define DBG_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int id;
  int file;
  int line;
} dbg_file_line_t;

typedef struct {
  int id;
  char name[50];
  int start;
  int size;
  int addrsize;
  int type;
  char oname[50];
  int ooffs;
} dbg_file_seg_t;

typedef struct {
  // version
  int major, minor;

  // info
  int csym;
  int file;
  int lib;
  int line;
  int mod;
  int scope;
  int seg;
  int span;
  int sym;
  int type;
} dbg_file_info_t;

typedef struct {
  dbg_file_info_t  info;
  dbg_file_line_t* lines;
  dbg_file_seg_t*  segments;
} dbg_file_t;

dbg_file_t dbg_parse(const char*);

#ifdef __cplusplus
}
#endif

#endif
