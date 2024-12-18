#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()

large_text = "A" * 10001

print("Content-Type: text/plain")
print(f"Content-Length: {len(large_text)}")
print()

print(large_text)
