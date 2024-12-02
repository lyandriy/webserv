#!/usr/bin/php
<?php

// Obtener el valor de 'user' desde los parámetros de la URL o línea de comandos
$user = $argv[1];  // En CLI, el primer argumento de la línea de comandos es $argv[1]

// Si no se pasa 'user', generamos un error
if (empty($user)) {
    fwrite(STDERR, "DON'T PANIC!! Este script tiene un error deliberado\n");
    exit(1);
}

// Define el HTML
$html = <<<HTML
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
HTML;

// Imprime el encabezado HTTP y el HTML
header("Content-Type: text/html");
echo $html;
?>
