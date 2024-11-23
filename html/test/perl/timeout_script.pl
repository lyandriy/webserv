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
    <h1>¡Oh oh!</h1>
    <h2>¡Debería haberse producido un timeout!</h2>
</body>
</html>
);

# Espera por 200 segundos
sleep(200);

# Escribe el HTML en la salida estándar
print "Content-Type: text/html\n\n";  # Encabezado de respuesta HTTP
print $html;