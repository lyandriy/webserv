#!/usr/bin/env python3

# Importamos el módulo CGI
import cgi
import cgitb

# Habilitamos la salida de errores en el navegador (opcional para desarrollo)
cgitb.enable()

# Generamos un texto con más de 10,000 bytes
large_text = "A" * 10001  # Esto genera un texto compuesto por 10,001 caracteres "A"

# Enviamos los encabezados HTTP necesarios
print("Content-Type: text/plain")  # Indicamos que la respuesta es texto plano
print(f"Content-Length: {len(large_text)}")  # Indicamos la longitud del contenido
print()  # Línea en blanco para separar encabezados del cuerpo

# Enviamos el cuerpo de la respuesta
print(large_text)
