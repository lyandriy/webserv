#!/usr/bin/perl

use strict;
use warnings;

my $user = $ENV{'USER'};

my $corrector = defined $user ? $user : "corrector guapete";

my $html = qq(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>¡Bienvenido $corrector!</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
    </style>
</head>
<body>
    <h1>Enhorabuena $corrector!</h1>
    <h2>Has ejecutado un script que recibe tu login como parámetro!</h2>
    <h3>Mola, eh!</h3>
</body>
</html>
);

print "Content-Type: text/html\n\n";
print $html;
