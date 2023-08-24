#include "6502emu/dbg_parse.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int read_file(FILE* f, size_t* o_size, char** o_contents) {
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (o_contents) {
    *o_contents = malloc(size);
    if (!(*o_contents)) {
      fprintf(stderr, "Failed to malloc\n");
      return 0;
    }
    fread(*o_contents, 1, size, f);
  }
  if (o_size)     *o_size = size;
  return 1;
}

dbg_file_t dbg_parse(const char* file) {
  dbg_file_t dbg = {0};
  FILE* f = fopen(file, "r");

#define STATE_NONE    1
#define STATE_VERSION 2
#define STATE_INFO    3
#define STATE_FILE    4
#define STATE_LINE    5
#define STATE_MOD     6
#define STATE_SEG     7
  int  state = STATE_NONE;
  char line[500];
  clearerr(f);

  while (1) {
    fgets(line, sizeof(line), f);
    if (feof(f)) break;
    // printf("Line: %s\n", line);

    // Begin tokenizing the line
    char* rest = line;
    char* token = NULL;
    char* tokenized_line[100]; int tokenized_line_count = 0;
    while ((token = strtok_r(rest, "\t,= \n", &rest))) {
      tokenized_line[tokenized_line_count++] = token;
    }

    // Show tokens
    {
    for (int i = 0; i < tokenized_line_count; i++) {
      printf("%d:%s   ", i, tokenized_line[i]);
    }
    printf("\n");
    }

#define STR_EQ(a, b) strcmp((a), (b)) == 0
    state = STATE_NONE;
    printf("==================\n");
    for (int i = 0; i < tokenized_line_count; i++) {
      if (strcmp(tokenized_line[0], "version") == 0) { state = STATE_VERSION; }
      if (strcmp(tokenized_line[0], "info")    == 0) { state = STATE_INFO; }
      if (strcmp(tokenized_line[0], "file")    == 0) { state = STATE_FILE; }
      if (strcmp(tokenized_line[0], "line")    == 0) { state = STATE_LINE; }
      if (strcmp(tokenized_line[0], "mod")     == 0) { state = STATE_MOD; }
      if (strcmp(tokenized_line[0], "seg")     == 0) { state = STATE_SEG; }

      if (state == STATE_VERSION) {
        // parse the major version
        if (STR_EQ(tokenized_line[i], "major")) {
          printf("(Version, major) %s\n", tokenized_line[i + 1]);
          i++;
        }
        // parse the minor version
        if (STR_EQ(tokenized_line[i], "minor")) {
          printf("(Version, minor) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
      if (state == STATE_INFO) {
        if (STR_EQ(tokenized_line[i], "csym")) {
          printf("(Info, csym) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "file")) {
          printf("(Info, file) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "lib")) {
          printf("(Info, lib) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "line")) {
          printf("(Info, line) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "mod")) {
          printf("(Info, mod) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "scope")) {
          printf("(Info, scope) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "seg")) {
          printf("(Info, seg) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "span")) {
          printf("(Info, span) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "sym")) {
          printf("(Info, sym) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "type")) {
          printf("(Info, type) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
      if (state == STATE_FILE) {
        if (STR_EQ(tokenized_line[i], "id")) {
          printf("(File, id) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "name")) {
          printf("(File, name) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "size")) {
          printf("(File, size) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "mtime")) {
          printf("(File, mtime) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "mod")) {
          printf("(File, mod) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
      if (state == STATE_LINE) {
        if (STR_EQ(tokenized_line[i], "id")) {
          printf("(Line, id) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "file")) {
          printf("(Line, file) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "line")) {
          printf("(Line, line) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
      if (state == STATE_MOD) {
        if (STR_EQ(tokenized_line[i], "id")) {
          printf("(Line, id) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "name")) {
          printf("(Line, name) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "file")) {
          printf("(Line, file) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
      if (state == STATE_SEG) {
        if (STR_EQ(tokenized_line[i], "id")) {
          printf("(Seg, id) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "name")) {
          printf("(Seg, name) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "start")) {
          printf("(Seg, start) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "size")) {
          printf("(Seg, size) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "addrsize")) {
          printf("(Seg, addrsize) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "type")) {
          printf("(Seg, type) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "oname")) {
          printf("(Seg, oname) %s\n", tokenized_line[i + 1]);
          i++;
        }
        if (STR_EQ(tokenized_line[i], "ooffs")) {
          printf("(Seg, ooffs) %s\n", tokenized_line[i + 1]);
          i++;
        }
      }
    }
    printf("==================\n");
  }

  return dbg;
}
