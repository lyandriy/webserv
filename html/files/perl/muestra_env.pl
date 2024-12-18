#!/usr/bin/perl

use strict;
use warnings;

my $li_tags = "";
foreach my $key (keys %ENV) {
    my $value = $ENV{$key};
    $li_tags .= "<li>$key: $value</li>\n";
}

my $ul = "<ul>\n" . $li_tags . "</ul>\n";

my $html = qq(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>¡Bienvenido!</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
        ul {
            list-style-position: inside;
            padding: 0;
            display: inline-block;
        }
    </style>
</head>
<body>
    <h1>Enhorabuena!</h1>
    <h2>Has ejecutado un script que muestra los parámetros que has incluido en la request!</h2>
    $ul
</body>
</html>
);

print "Content-Type: text/html\n\n";
print $html;
