#!/usr/bin/python3

import sys
import time
import os

html = html = f"""<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>¡Bienvenido!</title>
	<style>
		body {{ font-family: Arial, sans-serif; text-align: center; }}
	</style>
</head>
<body>
	<h1>¡Enhorabuena!PYTHON</h1>
	<h2>¡Has tenido la paciencia suficiente para esperar!</h2>
	<h3>El servidor que me ha devuelto no bloquea las peticiones, ¡mola, eh!</h3>
    <p>Eso es todo, menuda decepción...</p>
</body>
</html>"""

time.sleep(10)

sys.stdout.write(html)
