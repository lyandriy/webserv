#!/usr/bin/php
<?php

// Define el HTML
$html = <<<HTML
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
HTML;

// Espera por 200 segundos
sleep(200);

// Imprime el encabezado HTTP y el HTML
header("Content-Type: text/html");
echo $html;
?>