#!/usr/bin/env python3

import json
import os
import sys

print("Content-Type: application/json")
print()

image_directory = "/home/lyandriy/server/gallery-images"

base_url = "http://localhost:8081/images/"

image_extensions = {".png", ".jpg", ".jpeg", ".gif", ".webp"}

images = [
]

if not os.path.isdir(image_directory):
    response = { "images": [] }
    print(json.dumps(response))
    sys.exit(0)

for filename in os.listdir(image_directory):
    ext = os.path.splitext(filename)[1].lower()
    if ext in image_extensions:
        images.append(base_url + filename)

response = {"images": images}

print(json.dumps(response))
