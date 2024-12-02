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
	<h1>¡Oh oh!</h1>
	<h2>¡Debería habere producido un timeout!</h2>
</body>
</html>"""

time.sleep(200)

sys.stdout.write(html)

