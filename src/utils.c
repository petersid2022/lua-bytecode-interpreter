#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"

inline bool parse_command_line_args(int argc, char **argv) {
  struct stat buf;

  if (argc != 2) {
    fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
    return false;
  }

  stat(argv[1], &buf);

  if (S_ISDIR(buf.st_mode)) {
    fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
    return false;
  }

  return true;
}

inline s_String_View sv_from_parts(const char *data, size_t count) {
  s_String_View sv;
  sv.count = count;
  sv.data = data;
  return sv;
}

inline s_String_View sv_chop_by_delim(s_String_View *sv, char delim) {
  size_t i = 0;
  while (i < sv->count && sv->data[i] != delim) {
    i += 1;
  }

  s_String_View result = sv_from_parts(sv->data, i);

  if (i < sv->count) {
    sv->count -= i + 1;
    sv->data += i + 1;
  } else {
    sv->count -= i;
    sv->data += i;
  }

  return result;
}

inline bool sv_cmp(s_String_View a, s_String_View b) {
  if (a.count != b.count) {
    return false;
  }
  return memcmp(a.data, b.data, a.count) == 0;
}

inline s_String_View sv_from_cstr(const char *cstr) {
  return sv_from_parts(cstr, strlen(cstr));
}

inline bool sv_ends_with(s_String_View sv, const char *cstr) {
  size_t cstr_count = strlen(cstr);
  if (sv.count >= cstr_count) {
    size_t ending_start = sv.count - cstr_count;
    s_String_View sv_ending = sv_from_parts(sv.data + ending_start, cstr_count);
    return sv_cmp(sv_ending, sv_from_cstr(cstr));
  }
  return false;
}

inline s_String_View sv_clone(s_String_View sv) {
  char *mem = arena_malloc(&arena, sv.count);
  memcpy(mem, sv.data, sv.count);
  return sv_from_parts(mem, sv.count);
}

inline s_String_View sv_from_sb(s_String_Builder *sb) {
  return sv_from_parts(sb->data, sb->count);
}

inline const char *sv_to_cstr(s_String_View sv) {
  char *s = arena_malloc(&arena, sv.count + 1);
  memcpy(s, sv.data, sv.count);
  s[sv.count] = '\0';
  return s;
}

inline s_String_View sv_chop_right(s_String_View sv, size_t count) {
  if (count > sv.count) {
    count = sv.count;
  }
  s_String_View result = sv_from_parts(sv.data + sv.count - count, count);
  sv.count -= count;
  return result;
}

inline s_String_View sv_chop_left(s_String_View sv, size_t count) {
  if (count > sv.count) {
    count = sv.count;
  }
  return sv_from_parts(sv.data + count, sv.count - count);
}

inline s_String_View sv_trim_left(s_String_View sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[i])) {
    i += 1;
  }

  return sv_from_parts(sv.data + i, sv.count - i);
}

inline s_String_View sv_trim_right(s_String_View sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
    i += 1;
  }

  return sv_from_parts(sv.data, sv.count - i);
}

inline s_String_View sv_trim(s_String_View sv) {
  return sv_trim_right(sv_trim_left(sv));
}

inline char sv_getch(s_String_View *sv) {
  assert(sv->count > 0);
  char ch = sv->data[0];
  sv->data += 1;
  sv->count -= 1;
  return ch;
}

inline char sv_find(s_String_View haystack, const char *needle) {
  size_t needle_len = strlen(needle);
  void *p = memmem(haystack.data, haystack.count, needle, needle_len);
  return p ? (int)((const char *)p - haystack.data) : -1;
}

inline void sb_append_cstr(s_String_Builder *sb, const char *str) {
  size_t len = strlen(str);
  reserve_memory(sb, sb->count + len);
  memcpy((char *)sb->data + sb->count, str, len * sizeof(*sb->data));
  sb->count += len;
}

inline void sb_append_bytes(s_String_Builder *sb, const char *data, size_t len) {
  if (len == 0) {
    return;
  }
  reserve_memory(sb, sb->count + len);
  memcpy((char *)sb->data + sb->count, data, len * sizeof(*sb->data));
  sb->count += len;
}

inline void sb_appendf(s_String_Builder *sb, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  int n = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  // reserve space for the null terminator
  reserve_memory(sb, sb->count + n + 1);

  const char *dest = sb->data + sb->count;

  va_start(args, fmt);
  vsnprintf((char *)dest, n + 1, fmt, args);
  va_end(args);

  sb->count += n;
}

inline void sb_append_null(s_String_Builder *sb) {
  reserve_memory(sb, sb->count + 1);
  memcpy((char *)sb->data + sb->count, "", sizeof(*sb->data));
  sb->count += 1;
}

inline void sb_append_null_at(s_String_Builder *sb, size_t count) {
  sb->count = count;
  memcpy((char *)sb->data + count, "", sizeof(*sb->data));
}

inline void *arena_malloc(s_Arena *arena, size_t size) {
  const size_t alignment = sizeof(uintptr_t);

  assert(arena);
  assert(arena->end);
  assert(size <= SIZE_MAX - (alignment - 1));

  size_t aligned_size = ((size + alignment - 1) / alignment) * alignment;
  if (aligned_size == 0) {
    aligned_size = alignment;
  }

  while (aligned_size > arena->end->capacity - arena->end->count && arena->end->next) {
    arena->end = arena->end->next;
  }

  if (aligned_size > arena->end->capacity - arena->end->count) {
    size_t capacity = aligned_size > ARENA_DEFAULT_REGION_CAPACITY ? aligned_size : ARENA_DEFAULT_REGION_CAPACITY;
    assert(capacity <= SIZE_MAX - sizeof(s_Region));
    size_t n = sizeof(s_Region) + capacity;
    s_Region *region = (s_Region *)malloc(n);
    assert(region);
    region->next = NULL;
    region->count = 0;
    region->capacity = capacity;
    arena->end->next = region;
    arena->end = region;
  }

  void *ptr = (void *)(arena->end->data + arena->end->count);
  arena->end->count += aligned_size;
  return ptr;
}

inline void *arena_calloc_array(s_Arena *arena, size_t count, size_t element_size) {
  if (count == 0) {
    return NULL;
  }

  assert(element_size > 0);
  assert(count <= SIZE_MAX / element_size);

  size_t size = count * element_size;
  void *ptr = arena_malloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}

inline void arena_startup(s_Arena *arena, size_t size) {
  assert(arena);
  assert(size > 0);
  assert(size <= SIZE_MAX - sizeof(s_Region));

  size_t capacity = size;
  s_Region *page = malloc(sizeof(s_Region) + capacity);
  assert(page);
  page->capacity = capacity;
  page->count = 0;
  page->next = NULL;
  arena->head = page;
  arena->end = page;
}

inline void arena_terminate(s_Arena *arena) {
  s_Region *page = arena->head;
  while (page) {
    s_Region *next = page->next;
    free(page);
    page = next;
  }
  arena->head = NULL;
  arena->end = NULL;
}

inline void arena_reset(s_Arena *arena) {
  assert(arena);
  s_Region *page = arena->head;
  while (page) {
    page->count = 0;
    page = page->next;
  }
  arena->end = arena->head;
}

inline void print_binary(const uint32_t value) {
  size_t width = 32;
  for (size_t i = 0; i < width; ++i) {
    putchar((value & (1UL << (width - i - 1))) ? '1' : '0');
  }

  printf("\n");
}

inline void print_luac_hex_dump(const s_String_View *contents) {
  for (size_t i = 0; i <= contents->count; i += 16) {
    printf("%.8zx:  ", i);
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < contents->count) {
        printf("%.2hhx ", contents->data[i + j]);
      } else {
        printf(" ");
      }

      if ((j + 1) % 8 == 0) {
        printf(" ");
      }
    }

    printf("\n");
  }
}

inline void skip_byte(s_String_View *sv) {
  sv->data++;
  sv->count--;
}

inline uint8_t poke_next_byte(s_String_View *sv) {
  uint8_t byte = (uint8_t)*sv->data++;
  sv->count--;
  return byte;
}

inline s_String_View poke_next_bytes(s_String_View *sv, size_t n) {
  s_String_View out = sv_from_parts(sv->data, n);
  sv->data += n;
  sv->count -= n;
  return out;
}

inline bool read_lua_file_to_memory(const char *file_name, s_String_Builder *contents) {
  size_t got = 0, want = 0;

  if (strlen(file_name) == 0) {
    fprintf(stderr, "ERROR: no file name was provided\n");
    return NULL;
  }

  FILE *fp = fopen(file_name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "ERROR: failed to open the file: %s\n", strerror(errno));
    contents = NULL;
    goto cleanup;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fprintf(stderr, "ERROR: failed to seek the end of file: %s\n", strerror(errno));
    contents = NULL;
    goto cleanup;
  }

  long size = ftell(fp);
  if (size < 0) {
    fprintf(stderr, "ERROR: failed to return the current position of stream: %s\n", strerror(errno));
    contents = NULL;
    goto cleanup;
  }
  want = (size_t)size;

  rewind(fp);

  reserve_memory(contents, want + 1);

  if ((got = fread((char *)contents->data, 1, want, fp)) < want) {
    fprintf(stderr, "ERROR: failed to read entire file: got %zu bytes instead of %zu\n", got, want);
    contents = NULL;
    goto cleanup;
  }

  sb_append_null_at(contents, got);

  unsigned char *bytes = (unsigned char *)contents->data;
  if (bytes[0] != '\x1b' || bytes[1] != '\x4c' || bytes[2] != '\x75' || bytes[3] != '\x61') {
    fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
    contents = NULL;
    goto cleanup;
  }

cleanup:
  if (fp) {
    fclose(fp);
  }

  return contents != NULL;
}
