#!/usr/bin/php
<?php

// Recupera todas las variables de entorno
$li_tags = "";
foreach ($_ENV as $key => $value) {
    $li_tags .= "<li>$key: $value</li>\n";
}

// Crea el contenido HTML
$ul = "<ul>\n" . $li_tags . "</ul>\n";

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
HTML;

// Imprime el encabezado HTTP y el HTML
header("Content-Type: text/html");
echo $html;
?>
