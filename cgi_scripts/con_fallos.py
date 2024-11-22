#!/usr/bin/python3

import sys

try:
    html = f"""<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>¡Bienvenido {user}!</title>
	<style>
		body {{ font-family: Arial, sans-serif; text-align: center; }}
	</style>
</head>
<body>
	<h1>Enhorabuena {user}!</h1>
	<h2>Has ejecutado un script que recibe tu login como parámetro!</h2>
	<h3>Mola, eh!</h3>
</body>
</html>"""


except NameError:
    sys.stderr.write("DON'T PANIC!! Este script tiene un error deliberado")
    sys.exit(1)
