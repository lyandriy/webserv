#!/usr/bin/python3

import os

user = os.environ.get('user', 'Usuario desconocido')
years = os.environ.get('years', 'No especificado') 
answer = os.environ.get('answer', 'No respondido')  

html = f"""<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Información del usuario</title>
    <style>
        body {{ font-family: Arial, sans-serif; text-align: center; }}
    </style>
</head>
<body>
    <h1>Hola {user}!</h1>
    <h2>Edad: {years} años</h2>
    <h3>Tu respuesta: {answer}</h3>
</body>
</html>"""

print(html)
