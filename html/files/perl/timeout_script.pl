#!/usr/bin/perl

use strict;
use warnings;
use Time::HiRes qw(sleep);

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

sleep(200);

print "Content-Type: text/html\n\n";
print $html;