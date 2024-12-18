#!/usr/bin/python3

import os
import sys

user = os.environ.get('user')
if user != None:
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
	<h2>Has ejecutado un script que recibe tu login como header!</h2>
	<h3>Mola, eh!</h3>
</body>
</html>"""

else:
    html = f"""<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Tutorial</title>
	<style>
		body {{ font-family: Arial, sans-serif; text-align: center; }}
	</style>
</head>
<body>
	<h1 style="background-color: red;">¿A qué juegas?</h1>
	<h2>Has ejecutado un script que recibe tu login como header!</h2>
	<h3>Pero no has pasado ningún header de user! Por favor, en tu petición añade "user:tu_login"</h3>
</body>
</html>"""

sys.stdout.write(html)
# print(html, end="")


