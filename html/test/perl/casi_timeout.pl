#!/usr/bin/perl

use strict;
use warnings;
use Time::HiRes qw(sleep);

# Define el HTML
my $html = q(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>¡Bienvenido!</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
    </style>
</head>
<body>
    <h1>¡Enhorabuena!</h1>
    <h2>¡Has tenido la paciencia suficiente para esperar!</h2>
    <h3>El servidor que me ha devuelto no bloquea las peticiones, ¡mola, eh!</h3>
    <p>Eso es todo, menuda decepción...</p>
</body>
</html>
);

# Espera por 20 segundos
sleep(20);

# Escribe el HTML en la salida estándar
print "Content-Type: text/html\n\n";  # Encabezado de respuesta HTTP
print $html;
