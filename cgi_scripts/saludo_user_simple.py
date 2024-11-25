#!/usr/bin/python3

import os
import sys

user = os.environ.get('user')
corrector = user if user != None else "corrector guapete"
	

html = f"""<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>¡Bienvenido {corrector}!</title>
	<style>
		body {{ font-family: Arial, sans-serif; text-align: center; }}
	</style>
</head>
<body>
	<h1>Enhorabuena {corrector}!</h1>
	<h2>Has ejecutado un script que recibe tu login como parámetro!</h2>
	<h3>Mola, eh!</h3>
</body>
</html>"""



sys.stdout.write(f"Content-Type: text/html\r\n\r\n")
sys.stdout.write(html)

