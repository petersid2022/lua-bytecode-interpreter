#!/usr/bin/env python3

import sys
import re

for line in sys.stdin:
    if "Instructions" in line:
        match = re.search(r'Instructions:\s+(.*)', line)
        if match:
            numbers = match.group(1).split()
            for num in numbers:
                # Remove escape sequences from color formatting
                num = re.sub(r'\x1b\[[0-9;]*m', '', num)
                instruction = int(num, 16)
                opcode = instruction & 0x7f
                print(f"Instruction: {instruction:032b} - Opcode: {opcode:07b}")
