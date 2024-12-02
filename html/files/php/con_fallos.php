#!/usr/bin/perl

use strict;
use warnings;

# Definir el valor de 'user' (se puede obtener desde parámetros de línea de comandos)
my $user = $ARGV[0];

# Si no se pasa un 'user', generamos un error
if (not defined $user) {
    print STDERR "DON'T PANIC!! Este script tiene un error deliberado\n";
    exit(1);
}

# Define el HTML
my $html = qq(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>¡Bienvenido $user!</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
    </style>
</head>
<body>
    <h1>Enhorabuena $user!</h1>
    <h2>Has ejecutado un script que recibe tu login como parámetro!</h2>
    <h3>Mola, eh!</h3>
</body>
</html>
);

# Escribe el HTML en la salida estándar
print "Content-Type: text/html\n\n";  # Encabezado de respuesta HTTP
print $html;
