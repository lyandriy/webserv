#!/usr/bin/perl

use strict;
use warnings;

# Obtener el valor de 'user' desde las variables de entorno
my $user = $ENV{'USER'};

# Definir el contenido HTML dependiendo de si se encuentra el valor de 'user'
my $html;

if (defined $user) {
    $html = qq(
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
} else {
    $html = qq(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Tutorial</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
    </style>
</head>
<body>
    <h1 style="background-color: red;">¿A qué juegas?</h1>
    <h2>Has ejecutado un script que recibe tu login como parámetro!</h2>
    <h3>Pero no has pasado ningún parámetro! Por favor, en tu petición añade "?user=tu_login"</h3>
</body>
</html>
);
}

# Imprime el encabezado y el HTML
print "Content-Type: text/html\n\n";
print $html;
