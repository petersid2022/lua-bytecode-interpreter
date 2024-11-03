#!/usr/bin/env python3

import sys
import re
from textwrap import wrap

for line in sys.stdin:
    if "f->code" in line:
        match = re.search(r'f->code:\s+(.*)', line)
        opcode_str = match.group(1) if match else None
        if opcode_str:
            cleaned_code = re.sub(r'\x1b\[[0-9;]*m', '', opcode_str)
            instructions = wrap(cleaned_code, 32)
            for instruction in instructions:
                instruction_bin = int(instruction, 2)
                opcode = instruction_bin >> 25
                print(f"Instruction: {instruction_bin:032b} - Opcode: {opcode:07b}")
