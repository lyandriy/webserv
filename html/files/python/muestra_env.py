#!/usr/bin/python3

import os
import sys

li_tags = ""
for key, value in os.environ.items():
    li_tags += f"<li>{key}: {value}</li>\n"
ul = "<ul>\n" + li_tags + "</ul>\n"

html = f"""<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>¡Bienvenido!</title>
	<style>
		body {{ font-family: Arial, sans-serif; text-align: center; }}
  ul {{
            list-style-position: inside;
            padding: 0;
            display: inline-block;
        }}
	</style>
</head>
<body>
	<h1>Enhorabuena!</h1>
	<h2>Has ejecutado un script que muestra los parámetros que has incluido en la request!</h2>
	{ul}
</body>
</html>"""

sys.stdout.write(html)

