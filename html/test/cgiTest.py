#!/usr/bin/python3

import os
import sys

try:
    corrector = f'{sys.argv[1]}'
except:
    corrector = f'{os.environ.get("USER")}' 

html = f"""
<!DOCTYPE html>
<title>¡Bienvenido {corrector}!</title>
<h1>Enhorabuena {corrector}!</h1>
<h2>Has ejecutado un script que recibe tu login como parametro!</h2>
<h3>Mola, eh!</h3>
"""

print(html)
