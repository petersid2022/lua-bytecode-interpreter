#include <stdint.h>

#include "instructions.h"
#include "utils.h"

s_Arena arena;

int main(int argc, char **argv) {
  arena_initialize();

  s_String_Builder sb = {0};

  if (!parse_command_line_args(argc, argv)) {
    return 1;
  }

  if (!read_lua_file_to_memory(argv[1], &sb)) {
    return 1;
  }

  s_String_View sv = sv_from_sb(&sb);

  print_luac_hex_dump(&sv);
  print_luac_header(&sv);
  parse_luac_hex_dump(&sv);

  arena_shutdown();

  return 0;
}
