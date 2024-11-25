#!/usr/bin/env python3

import os
import sys

def main():
    # Obtener la longitud del contenido desde la variable de entorno
    content_length = os.environ.get("Content-Length")
    content_length = int(content_length) if content_length else 0

    # Leer el cuerpo de la solicitud desde stdin
    body = sys.stdin.read(content_length)

    # Imprimir la respuesta HTTP
    print("Content-Type: text/html\n")  # Encabezado HTTP
    print("<html>")
    print("<head><title>POST Response</title></head>")
    print("<body>")
    print("<h1>POST recibido con éxito</h1>")
    print(f"<p>Contenido recibido: {body}</p>")
    print("</body>")
    print("</html>")

if __name__ == "__main__":
    main()
