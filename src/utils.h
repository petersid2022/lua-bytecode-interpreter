#ifndef UTILS_H__
#define UTILS_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern struct s_Arena arena;

enum {
  ARENA_DEFAULT_REGION_CAPACITY = (8 * 1024),
};

typedef struct s_String_View {
  size_t count;
  const char *data;
} s_String_View;

typedef struct s_String_Builder {
  size_t count;
  size_t capacity;
  const char *data;
} s_String_Builder;

typedef struct s_Region {
  size_t count;
  size_t capacity;
  struct s_Region *next;
  unsigned char data[];
} s_Region;

typedef struct s_Arena {
  s_Region *head;
  s_Region *end;
} s_Arena;

#define reserve_memory(da, expected_capacity)                                    \
  do {                                                                           \
    if ((expected_capacity) > (da)->capacity) {                                  \
      if ((da)->capacity == 0) {                                                 \
        (da)->capacity = 256;                                                    \
      }                                                                          \
      while ((expected_capacity) > (da)->capacity) {                             \
        (da)->capacity *= 1.5;                                                   \
      }                                                                          \
      (da)->data = arena_malloc(&(arena), (da)->capacity * sizeof(*(da)->data)); \
      assert((da)->data != NULL);                                                \
    }                                                                            \
  } while (0)

#define append(xs, x)                           \
  do {                                          \
    __auto_type expected_cap = (xs)->count + 1; \
    reserve_memory((xs), expected_cap);         \
    (xs)->data[(xs)->count++] = (x);            \
  } while (0)

#define arena_initialize()                                                  \
  do {                                                                      \
    arena_startup(&arena, ARENA_DEFAULT_REGION_CAPACITY * sizeof(uint8_t)); \
  } while (0)

#define arena_shutdown()     \
  do {                       \
    arena_terminate(&arena); \
  } while (0)

extern bool sv_cmp(s_String_View, s_String_View);
extern bool sv_ends_with(s_String_View, const char *);
extern char sv_find(s_String_View, const char *);
extern char sv_getch(s_String_View *);
extern const char *sv_to_cstr(s_String_View);
extern s_String_View sv_from_parts(const char *, size_t);
extern s_String_View sv_chop_by_delim(s_String_View *, char);
extern s_String_View sv_from_cstr(const char *);
extern s_String_View sv_clone(s_String_View);
extern s_String_View sv_from_sb(s_String_Builder *);
extern s_String_View sv_chop_right(s_String_View, size_t);
extern s_String_View sv_chop_left(s_String_View, size_t);
extern s_String_View sv_trim_left(s_String_View);
extern s_String_View sv_trim_right(s_String_View);
extern s_String_View sv_trim(s_String_View);

extern void sb_append_cstr(s_String_Builder *, const char *);
extern void sb_append_bytes(s_String_Builder *, const char *, size_t);
extern void sb_appendf(s_String_Builder *, const char *, ...) __attribute__((format(printf, 2, 3)));
extern void sb_append_null(s_String_Builder *);
extern void sb_append_null_at(s_String_Builder *, size_t);

extern void arena_reset(s_Arena *);
extern void arena_terminate(s_Arena *);
extern void arena_startup(s_Arena *, size_t);
extern void *arena_malloc(s_Arena *, size_t);
extern void *arena_calloc_array(s_Arena *, size_t, size_t);

extern void print_binary(const uint32_t);
extern void print_luac_hex_dump(const s_String_View *);

extern void skip_byte(s_String_View *);
extern uint8_t poke_next_byte(s_String_View *);
extern s_String_View poke_next_bytes(s_String_View *, size_t);

extern bool parse_command_line_args(int argc, char **argv);
extern bool read_lua_file_to_memory(const char *, s_String_Builder *);

#endif // UTILS_H__

// vim: ft=c
