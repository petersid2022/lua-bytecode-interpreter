#!/usr/bin/env python3

def binary_to_hex_grouped(binary_str: str):
    hex_str = ''
    for i in range(0, len(binary_str), 4):
        four_bit_chunk = binary_str[i:i+4]
        hex_str += f"{int(four_bit_chunk, 2):X}"
    return hex_str

with open("instructions.txt") as f:
    while True:
        chunk = f.read(32).strip()
        if not chunk:
            break
        hex_value = binary_to_hex_grouped(chunk).zfill(8)
        # Split into bytes and reverse them
        reversed_bytes = [hex_value[i:i+2] for i in range(0, 8, 2)][::-1]
        print(" ".join(reversed_bytes))
